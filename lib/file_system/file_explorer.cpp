/**
 * @file file_explorer.cpp
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

#include <file_explorer.h>

#include <string_edit.hpp>
#include <type_traits>

/**
 * @brief 构造函数，初始化并挂载文件系统
 * 在创建 FileExplorer 对象时，会自动挂载文件系统。
 */
// FileExplorer::FileExplorer(const std::string& metadatabase_path) : meta(metadatabase_path) { fs.mount(); }

FileExplorer::FileExplorer(const std::string& metadatabase_path) { fs.mount(); }

/**
 * @brief 析构函数
 * 确保在文件管理器析构时卸载文件系统。
 */
FileExplorer::~FileExplorer() { fs.unmount(); }

/**
 * @brief 在指定路径创建文件。
 * @param filePath 文件的完整路径，包含文件名和扩展名。
 * @return 如果文件创建成功，返回 true；否则返回 false。
 */
bool FileExplorer::createFile(const std::string& filePath) {
    std::string dir_path = file.getDirectoryPath(filePath);
    if (!dir.createDir(dir_path)) return false;
    if (!file.createFile(filePath)) return false;
    // meta.onFileCreate(filePath);  // 文件创建时更新元数据
    return true;
}

/**
 * @brief 在指定路径创建目录。
 * @param dirPath 目录的完整路径。
 * @return 如果目录创建成功，返回 true；否则返回 false。
 */
bool FileExplorer::createDir(const std::string& dirPath) {
    if (!dir.createDir(dirPath)) return false;
    // meta.onFileCreate(dirPath);  // 目录创建时更新元数据
    return true;
}

/**
 * @brief 判断指定路径是否存在。
 * @param path 文件或目录的路径。
 * @return 如果路径存在，返回 true；否则返回 false。
 */
bool FileExplorer::exists(const std::string& path) { return fs.exists(path); }

/**
 * @brief 复制指定路径的文件或目录到目标路径。
 * @param sourcePath 源文件或目录的路径。
 * @param targetPath 目标文件或目录的路径。
 */
void FileExplorer::copyPath(const std::string& sourcePath, const std::string& targetPath) {
    // 判断是文件还是目录
    if (fs.isDirectory(sourcePath)) {
        dir.copyDir(sourcePath, targetPath);  // 复制目录
    } else {
        file.copyFile(sourcePath, targetPath);  // 复制文件
    }
    // meta.copyMetadata(sourcePath, targetPath);  // 复制元数据
}

/**
 * @brief 移动指定路径的文件或目录到目标路径。
 * @param sourcePath 源文件或目录的路径。
 * @param targetPath 目标文件或目录的路径。
 */
void FileExplorer::movePath(const std::string& sourcePath, const std::string& targetPath) {
    // 判断是文件还是目录
    if (fs.isDirectory(sourcePath)) {
        if (!dir.copyDir(sourcePath, targetPath)) return;  // 复制目录
    } else {
        if (!file.copyFile(sourcePath, targetPath)) return;  // 复制文件
    }
    // meta.moveMetadata(sourcePath, targetPath);  // 移动元数据
    fs.deletePath(sourcePath);  // 删除源文件;
}

/**
 * @brief 重命名指定路径的文件或目录。
 * @param path 要重命名的文件或目录路径。
 * @param newName 新名称。
 */
void FileExplorer::renamePath(const std::string& path, const std::string& newName) {
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
 * @brief 删除指定路径的文件或目录。
 * @param path 要删除的文件或目录路径。
 */
void FileExplorer::deletePath(const std::string& path) { fs.deletePath(path); }

/**
 * @brief 以字符串形式读取文件内容。
 * @param filePath 文件的路径。
 * @return 文件内容的字符串表示。
 */
std::string FileExplorer::readFileAsString(const std::string& filePath) {
    std::string fileData = "";
    file.readFileAsString(filePath, fileData);
    return fileData;
}

/**
 * @brief 以字节数组形式读取文件内容。
 * @param filePath 文件的路径。
 * @return 文件内容的字节数组表示。
 */
std::vector<uint8_t> FileExplorer::readFileAsBytes(const std::string& filePath) {
    std::vector<uint8_t> fileData = {};
    file.readFileAsBytes(filePath, fileData);
    return fileData;
}

/**
 * @brief 以字符串形式写入数据到文件。
 * @param filePath 文件的路径。
 * @param data 要写入的字符串数据。
 * @param mode 写入模式，默认以追加模式写入。
 * @return 如果写入成功，返回 true；否则返回 false。
 */
bool FileExplorer::writeFileAsString(const std::string& filePath, const std::string& data, const char* mode) {
    return file.writeFileAsString(filePath, data, mode);
}

/**
 * @brief 以字节数组形式写入数据到文件。
 * @param filePath 文件的路径。
 * @param data 要写入的字节数组数据。
 * @param mode 写入模式，默认以追加模式写入。
 * @return 如果写入成功，返回 true；否则返回 false。
 */
bool FileExplorer::writeFileAsBytes(const std::string& filePath, const std::vector<uint8_t>& data, const char* mode) {
    return file.writeFileAsBytes(filePath, data, mode);
}

/**
 * @brief 精确查找指定名称的文件或目录。
 *
 * 该函数从指定的父路径开始，查找所有匹配目标名称的文件或目录，返回它们的完整路径。
 * 如果路径不存在，返回一个空的路径列表。
 *
 * @param targetName 目标文件或目录的名称，用于查找匹配的文件或目录。
 * @param parentPath 起始路径，默认为根路径"/"。从此路径开始查找目标文件或目录。
 *
 * @return 返回一个包含所有找到路径的字符串向量。如果未找到任何匹配项，返回空向量。
 */
std::vector<std::string> FileExplorer::findPath(const std::string& targetName, const std::string& parentPath) {
    auto file_tree = getTree(parentPath);  // 获取指定路径下的文件树

    // 查找所有匹配目标名称的节点
    std::vector<TreeNode<std::string>*> found_target_nodes = file_tree->findNode(targetName);

    std::vector<std::string> found_paths;            // 存储找到的路径列表
    found_paths.reserve(found_target_nodes.size());  // 提前为路径分配内存，提升性能

    // 遍历所有找到的目标节点
    for (auto& found_target_node : found_target_nodes) {
        std::string found_path = tree_tool.getPath(found_target_node).erase(0, 1);  // 获取节点的完整路径(并删除字符串首多余的"/")
        found_paths.push_back(found_path);                                          // 将构建好的路径添加到结果列表中
    }

    // 返回所有找到的完整路径
    return found_paths;
}

/**
 * @brief 模糊查找指定名称的文件或目录。
 *
 * 此函数从指定的父路径开始，递归遍历文件树，基于相似度计算找出与目标名称
 * 符合相似性阈值的节点，并返回对应路径的列表。
 *
 * @param targetName 要搜索的目标名称。
 * @param parentPath 搜索的起始路径，默认为根目录 "/"
 * @param similarityThreshold 相似度的最低阈值，默认为 0.0f
 * @return std::vector<std::string> 符合条件的路径列表，按相似度从高到低排序。
 */
std::vector<std::string> FileExplorer::searchPath(const std::string& targetName, const std::string& parentPath, float similarityThreshold) {
    // 获取指定路径下的文件树
    auto fileTree = getTree(parentPath);

    // 如果文件树为空，直接返回空结果
    if (!fileTree) return {};

    // 用于存储符合条件的节点及其相似度
    std::vector<std::pair<TreeNode<std::string>*, float>> matchingNodes;

    // 遍历文件树，计算每个节点名称与目标名称的相似度
    for (const auto& nodeData : fileTree->traversalDFS()) {
        sim.replaceString(targetName, nodeData.first);      // 设置比较的字符串
        float similarity = sim.evaluateStringSimilarity();  // 计算相似度

        // 如果相似度超过阈值，将节点加入匹配列表
        if (similarity >= similarityThreshold) {
            matchingNodes.emplace_back(nodeData.second, similarity);
        }
    }

    // 如果没有找到任何匹配项，返回空结果
    if (matchingNodes.empty()) return {};

    // 按相似度从高到低排序
    std::sort(matchingNodes.begin(), matchingNodes.end(),
              [](const std::pair<TreeNode<std::string>*, float>& a, const std::pair<TreeNode<std::string>*, float>& b) { return a.second > b.second; });

    std::vector<std::string> resultPaths;       // 准备返回结果的路径列表
    resultPaths.reserve(matchingNodes.size());  // 预分配内存以提高性能

    // 遍历匹配节点，获取对应路径并存储到结果列表中
    for (const auto& matchingNode : matchingNodes) {
        std::string nodePath = tree_tool.getPath(matchingNode.first);

        // 如果路径以 '/' 开头，则去掉根目录符号
        if (!nodePath.empty() && nodePath[0] == '/') nodePath.erase(0, 1);

        resultPaths.push_back(std::move(nodePath));  // 使用移动语义减少拷贝
    }

    return resultPaths;
}

/**
 * @brief 列出指定目录中的所有子项。
 * @param dirPath 目录路径。
 * @return 子项名称的列表。
 */
std::vector<std::string> FileExplorer::listDir(const std::string& dirPath) { return dir.listDir(dirPath); }

/**
 * @brief 打印指定目录中的所有子项。
 * @param dirPath 目录路径。
 */
void FileExplorer::printDir(const std::string& dirPath) {
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
std::unique_ptr<Tree<std::string>> FileExplorer::getTree(const std::string& dirPath) {
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

void FileExplorer::printTree(const std::string& dirPath) {
    auto tree = getTree(dirPath);
    std::string list_str = tree_tool.getTreeString(*tree);
    Serial.println(list_str.c_str());
}

/**
 * @brief 递归构建子目录树
 *
 * 该函数递归遍历指定路径下的文件和子目录，并构建目录树。
 * 每遇到一个子目录，都会进一步递归构建其子目录树。
 *
 * @param dirPath 当前目录的路径
 * @param parent_ptr 当前目录的父节点
 */
void FileExplorer::buildSubTree(const std::string& dirPath, TreeNode<std::string>& parent_ptr) {
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