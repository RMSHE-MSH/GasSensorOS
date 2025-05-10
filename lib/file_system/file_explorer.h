/**
 * @file file_explorer.h
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
// #include <metadata_manager.h>
#include <string_similarity_evaluator.h>

#include <directory_manager.hpp>
#include <file_manager.hpp>
#include <fs_interface.hpp>
#include <tree.hpp>
#include <tree_tool.hpp>

class FileExplorer {
   public:
    FileExplorer(const std::string& metadatabase_path = "/.os/metadatabase.db");
    ~FileExplorer();

    bool createFile(const std::string& filePath);
    bool createDir(const std::string& dirPath);
    bool exists(const std::string& path);

    void copyPath(const std::string& sourcePath, const std::string& targetPath);
    void movePath(const std::string& sourcePath, const std::string& targetPath);
    void renamePath(const std::string& path, const std::string& newName);
    void deletePath(const std::string& path);

    std::string readFileAsString(const std::string& filePath);
    std::vector<uint8_t> readFileAsBytes(const std::string& filePath);
    bool writeFileAsString(const std::string& filePath, const std::string& data, const char* mode = "a");
    bool writeFileAsBytes(const std::string& filePath, const std::vector<uint8_t>& data, const char* mode = "a");

    std::vector<std::string> findPath(const std::string& targetName, const std::string& parentPath = "/");
    std::vector<std::string> searchPath(const std::string& targetName, const std::string& parentPath = "/", float similarityThreshold = 0.0f);

    std::vector<std::string> listDir(const std::string& dirPath = "/");
    void printDir(const std::string& dirPath = "/");

    std::unique_ptr<Tree<std::string>> getTree(const std::string& dirPath = "/");
    void printTree(const std::string& dirPath = "/");

   private:
    void buildSubTree(const std::string& dirPath, TreeNode<std::string>& parent_ptr);

   private:
    TreeTool tree_tool;
    StringSimilarityEvaluator sim;
    StringSplitter splitter;

    FSInterface fs;
    DirectoryManager dir;
    FileManager file;
    // MetadataManager meta;
};
