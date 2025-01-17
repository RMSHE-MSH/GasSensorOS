/**
 * @file file_explorer.hpp
 * @date 16.01.2025
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2025 RMSHE. All rights reserved.
 *
 * This program is free software : you can redistribute it and /or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.If not, see < https://www.gnu.org/licenses/>.
 *
 * Electronic Mail : asdfghjkl851@outlook.com
 */

#pragma once
#include <directory_manager.hpp>
#include <file_manager.hpp>
#include <fs_interface.hpp>
#include <tree.hpp>
#include <tree_tool.hpp>

class FileExplorer {
   public:
    /**
     * @brief 构造函数，初始化并挂载文件系统
     * 在创建 FileExplorer 对象时，会自动挂载文件系统。
     */
    FileExplorer() { fs.mount(); }

    /**
     * @brief 析构函数
     * 确保在文件管理器析构时卸载文件系统。
     */
    ~FileExplorer() { fs.unmount(); }

    void createFile(const std::string& filePath) {
        std::string dir_path = file.getDirectoryPath(filePath);
        dir.createDir(dir_path);
        file.createFile(filePath);
    }

    /**
     * @brief 获取指定目录及其子目录的完整目录树（包括文件和目录）
     *
     * 该函数从指定路径开始，递归获取该目录下的所有子目录和文件，并以树形结构表示。该树形结构包含目录和文件。
     *
     * @param dirPath [in] 要获取目录结构的起始目录路径，默认为根目录 "/"
     * @return Tree<std::string> 返回表示目录和文件结构的树形数据结构
     */
    std::unique_ptr<Tree<std::string>> getTree(const std::string& dirPath = "/") {
        // 创建一个空的目录树对象，根节点为指定的目录路径
        auto file_tree = make_unique<Tree<std::string>>(dirPath);

        // 获取根目录下的所有文件和子目录
        std::vector<std::string> root_dir_names = dir.listDir(dirPath);

        // 遍历根目录下的每个文件和子目录
        for (const auto& name : root_dir_names) {
            std::string full_path = dirPath + "/" + name;

            // 在目录树中添加该目录作为根节点的子节点
            file_tree->root->addChild(name);

            // 如果是目录，递归构建子目录树
            if (fs.isDirectory(full_path)) {
                // 递归构建该目录下的树形结构
                buildSubTree(full_path, name, *file_tree);
            }
        }

        // 返回构建好的目录树
        return file_tree;
    }

    void printTree() {
        auto tree = getTree();
        std::string list_str = tree_tool.getTreeString(*tree);
        Serial.println(list_str.c_str());
    }

   private:
    /**
     * @brief 递归构建指定目录及其子目录和文件的树形结构
     *
     * 该函数会递归遍历指定目录及其所有子目录，构建完整的目录树，并将文件和子目录添加到父目录节点中。
     *
     * @param dirPath [in] 当前目录的完整路径
     * @param parentDirName [in] 当前目录的名称，用于标识父节点
     * @param file_tree [in/out] 存储目录和文件树形结构的对象
     */
    void buildSubTree(const std::string& dirPath, const std::string& parentDirName, Tree<std::string>& file_tree) {
        // 获取当前目录下的所有文件和子目录
        std::vector<std::string> sub_dir_names = dir.listDir(dirPath);

        // 遍历当前目录下的每个文件和子目录
        for (const auto& name : sub_dir_names) {
            std::string full_path = dirPath + "/" + name;

            // 查找父节点，并将当前子目录作为其子节点添加
            file_tree.findNode(parentDirName)->addChild(name);

            // 如果该项是目录，则递归构建该子目录的树形结构
            if (fs.isDirectory(full_path)) {
                // 递归处理该子目录
                buildSubTree(full_path, name, file_tree);
            }
        }
    }

   private:
    TreeTool tree_tool;

    FSInterface fs;  // 底层文件系统接口
    DirectoryManager dir;
    FileManager file;
};
