/**
 * @file metadata_manager.h
 * @date 24.01.2025
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
#include <cstdint>
#include <directory_manager.hpp>
#include <file_manager.hpp>
#include <fs_interface.hpp>
#include <metadatabase.hpp>
#include <string>
#include <unordered_map>

/**
 * @brief 元数据管理类，用于管理文件(注:这里的"文件"包含目录)的元数据。
 */
class MetadataManager {
   public:
    MetadataManager(const std::string& metadatabase_path);

    void onFileCreate(const std::string& path);
    void onFileRead(const std::string& path);
    void onFileWrite(const std::string& path);
    void onFileDelete(const std::string& path);

    bool getMetadata(const std::string& path, Metadata& metadata);
    void upsertMetadata(const std::string& path, const Metadata& metadata);
    void copyMetadata(const std::string& sourcePath, const std::string& targetPath);
    void moveMetadata(const std::string& sourcePath, const std::string& targetPath);
    void deleteMetadata(const std::string& path);
    void loadMetadataToBuffer();

    size_t getSize(const std::string& path);
    std::string getType(const std::string& path);
    void setHide(const std::string& path);

   private:
    FSInterface fs;
    DirectoryManager dir;
    FileManager file;

    Metadatabase metadb;                                        ///< 元数据数据库对象
    std::unordered_map<std::string, Metadata> metadata_buffer;  ///< 元数据缓存，以文件路径为索引
};
