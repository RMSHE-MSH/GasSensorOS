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
#include <type_traits>

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

   public:
    /**
     * @brief 在指定路径创建文件。
     * @param filePath 文件的完整路径，包含文件名和扩展名。
     */
    bool createFile(const std::string& filePath) {
        std::string dir_path = file.getDirectoryPath(filePath);
        if (!dir.createDir(dir_path)) return false;
        if (!file.createFile(filePath)) return false;
        return true;
    }

    /**
     * @brief 在指定路径创建目录。
     * @param dirPath 目录的完整路径。
     */
    bool createDir(const std::string& dirPath) {
        if (!dir.createDir(dirPath)) return false;
        return true;
    }

    bool exists(const std::string& path) { return fs.exists(path); }

   public:
    /**
     * @brief 复制指定路径的文件或目录到目标路径。
     * @param sourcePath 源路径，可以是文件或目录的完整路径。
     * @param targetPath 目标路径，将复制源文件或源目录到此路径。
     */
    void copyPath(const std::string& sourcePath, const std::string& targetPath) {
        // 判断是文件还是目录
        if (fs.isDirectory(sourcePath)) {
            dir.copyDir(sourcePath, targetPath);  // 复制目录
        } else {
            file.copyFile(sourcePath, targetPath);  // 复制文件
        }
    }

    /**
     * @brief 移动指定路径的文件或目录到目标路径。
     * @param sourcePath 源路径，可以是文件或目录的完整路径。
     * @param targetPath 目标路径，将源文件或源目录移动到此路径。
     */
    void movePath(const std::string& sourcePath, const std::string& targetPath) {
        // 判断是文件还是目录
        if (fs.isDirectory(sourcePath)) {
            if (!dir.copyDir(sourcePath, targetPath)) return;  // 复制目录
        } else {
            if (!file.copyFile(sourcePath, targetPath)) return;  // 复制文件
        }

        fs.deletePath(sourcePath);  // 删除源文件;
    }

    /**
     * @brief 重命名指定路径的文件或目录。
     * @param path 源路径，文件或目录的路径。
     * @param newName 新名称，目标文件或目录的名称。
     */
    void renamePath(const std::string& path, const std::string& newName) {
        // 构建目标路径：将原路径的目录部分与新的名称拼接
        std::string target_path = file.getDirectoryPath(path) + "/" + newName;

        // 判断是文件还是目录
        if (fs.isDirectory(path)) {
            if (!dir.copyDir(path, target_path)) return;  // 复制目录
        } else {
            if (!file.copyFile(path, target_path)) return;  // 复制文件
        }

        fs.deletePath(path);  // 删除源文件;
    }

    /**
     * @brief 递归删除指定路径的文件或目录
     * @param path 要删除的文件或目录路径
     */
    void deletePath(const std::string& path) { fs.deletePath(path); }

   public:
    std::string readFileAsString(const std::string& filePath) {
        std::string fileData = "";
        file.readFileAsString(filePath, fileData);
        return fileData;
    }

    std::vector<uint8_t> readFileAsBytes(const std::string& filePath) {
        std::vector<uint8_t> fileData = {};
        file.readFileAsBytes(filePath, fileData);
        return fileData;
    }

    bool writeFileAsString(const std::string& filePath, const std::string& data, const char* mode = "a") {
        return file.writeFileAsString(filePath, data, mode);
    }

    bool writeFileAsBytes(const std::string& filePath, const std::vector<uint8_t>& data, const char* mode = "a") {
        return file.writeFileAsBytes(filePath, data, mode);
    }

   public:
    /**
     * @brief 查找指定名称的文件或目录。
     *
     * 该函数从指定的父路径开始，查找所有匹配目标名称的文件或目录，返回它们的完整路径。
     * 如果路径不存在，返回一个空的路径列表。
     *
     * @param targetName 目标文件或目录的名称，用于查找匹配的文件或目录。
     * @param parentPath 起始路径，默认为根路径"/"。从此路径开始查找目标文件或目录。
     *
     * @return 返回一个包含所有找到路径的字符串向量。如果未找到任何匹配项，返回空向量。
     */
    std::vector<std::string> findPath(const std::string& targetName, const std::string& parentPath = "/") {
        auto file_tree = getTree(parentPath);  // 获取指定路径下的文件树

        // 查找所有匹配目标名称的节点
        std::vector<TreeNode<std::string>*> found_target_nodes = file_tree->findNode(targetName);

        std::vector<std::string> found_paths;            // 存储找到的路径列表
        found_paths.reserve(found_target_nodes.size());  // 提前为路径分配内存，提升性能

        // 遍历所有找到的目标节点
        for (auto& found_target_node : found_target_nodes) {
            std::string found_path = found_target_node->node_data;  // 初始化路径

            // 从目标节点向上遍历到根节点，构建完整路径
            while (true) {
                found_target_node = found_target_node->parent;                 // 移动到父节点
                if (found_target_node->node_data == "/") break;                // 遍历到根路径时停止
                found_path = found_target_node->node_data + "/" + found_path;  // 拼接当前节点和路径
            }

            // 根节点特殊处理
            found_path = "/" + found_path;

            // 将构建好的路径添加到结果列表中
            found_paths.push_back(found_path);
        }

        // 返回所有找到的完整路径
        return found_paths;
    }

    std::vector<std::string> listDir(const std::string& dirPath = "/") { return dir.listDir(dirPath); }

    void printDir(const std::string& dirPath = "/") {
        for (const auto& name : dir.listDir(dirPath)) {
            Serial.println(name.c_str());
        }
    }

    /**
     * @brief 获取指定目录路径下的目录树结构
     *
     * 该函数通过递归遍历指定路径下的文件和子目录，构建并返回一个目录树。
     * 通过调用 `dir.listDir` 获取目录内容，并使用 `TreeNode` 类来表示每一个目录项。
     *
     * @param dirPath 需要构建目录树的根路径，默认为根目录 "/"
     * @return std::unique_ptr<Tree<std::string>> 返回构建好的目录树
     */
    std::unique_ptr<Tree<std::string>> getTree(const std::string& dirPath = "/") {
        // 创建一个空的目录树对象，根节点为指定的目录路径
        auto file_tree = make_unique<Tree<std::string>>(dirPath);

        // 获取指定目录下的所有文件和子目录
        std::vector<std::string> root_dir_names = dir.listDir(dirPath);

        // 遍历目录下的每个文件和子目录
        for (const auto& name : root_dir_names) {
            // 获取当前项的完整路径
            std::string full_path = dirPath;
            if (dirPath.back() != '/') full_path += "/";  // 如果目录路径最后没有斜杠，添加斜杠
            full_path += name;

            // 在目录树中添加该目录作为根节点的子节点
            TreeNode<std::string>* parent_ptr = file_tree->root->addChild(name);

            // 如果是目录，递归构建子目录树
            if (fs.isDirectory(full_path)) {
                buildSubTree(full_path, *parent_ptr);  // 传递父节点指针
            }
        }

        // 返回构建好的目录树
        return file_tree;
    }

    void printTree(const std::string& dirPath = "/") {
        auto tree = getTree(dirPath);
        std::string list_str = tree_tool.getTreeString(*tree);
        Serial.println(list_str.c_str());
    }

   private:
    /**
     * @brief 递归构建子目录树
     *
     * 该函数递归遍历指定路径下的文件和子目录，并构建目录树。
     * 每遇到一个子目录，都会进一步递归构建其子目录树。
     *
     * @param dirPath 当前目录的路径
     * @param parent_ptr 当前目录的父节点
     */
    void buildSubTree(const std::string& dirPath, TreeNode<std::string>& parent_ptr) {
        // 获取当前目录下的所有文件和子目录
        std::vector<std::string> sub_dir_names = dir.listDir(dirPath);

        // 遍历当前目录下的每个文件和子目录
        for (const auto& name : sub_dir_names) {
            // 获取当前项的完整路径
            std::string full_path = dirPath;
            if (dirPath.back() != '/') full_path += "/";  // 如果目录路径最后没有斜杠，添加斜杠
            full_path += name;

            // 查找父节点，并将当前子目录作为其子节点添加
            TreeNode<std::string>* ptr = parent_ptr.addChild(name);

            // 如果该项是目录，则递归构建该子目录的树形结构
            if (fs.isDirectory(full_path)) {
                buildSubTree(full_path, *ptr);  // 递归处理该子目录
            }
        }
    }

   private:
    TreeTool tree_tool;

    FSInterface fs;
    DirectoryManager dir;
    FileManager file;
};
