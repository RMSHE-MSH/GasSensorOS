/**
 * @file metadata_manager.hpp
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
#include <sqlite3.h>

#include <chrono>
#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>

class MetadataManager {
   public:
    // 获取文件元数据
    Metadata getMetadata(const std::string& filePath);

    // 更新文件元数据
    bool updateMetadata(const std::string& filePath, const Metadata& metadata);

   private:
    struct Metadata {
        // ================== 基本属性 ==================
        std::string path;     // 文件完整路径，包含目录和文件名
        std::string type;     // 文件类型（如 "txt", "jpg" 等）
        size_t size = 0;      // 文件大小，单位为字节
        bool hidden = false;  // 是否为隐藏文件（默认不隐藏）

        // ================== 时间戳 ==================
        std::chrono::system_clock::time_point creation_time;      // 文件创建时间
        std::chrono::system_clock::time_point modification_time;  // 文件最后一次修改时间
        std::chrono::system_clock::time_point access_time;        // 文件最后一次访问时间
        std::chrono::system_clock::time_point deletion_time;      // 文件删除时间

        // ================== 权限和所有权 ==================
        std::set<std::string> owner_group = {"root"};    // 文件所属的用户组(完全控制权)
        std::set<std::string> read_permissions;          // 可读取文件的用户
        std::set<std::string> write_permissions;         // 可写入文件的用户
        std::set<std::string> modification_permissions;  // 可修改文件的用户

        // ================== 扩展属性 ==================
        std::string hash;         // 文件摘要
        std::string description;  // 文件描述

        // ================== 版本管理 ==================
        uint32_t version = 0;         // 文件版本号，默认版本为 0
        std::string author = "root";  // 最近修改者（默认是 root 用户）
        std::string diff_path;        // 差异文件路径，用于存储文件的增量变化
    };

    // 文件路径与元数据的映射
    std::unordered_map<std::string, Metadata> metadata_table;
};
