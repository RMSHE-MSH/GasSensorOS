/**
 * @file metadatabase.hpp
 * @date 25.01.2025
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
#include <data_table.hpp>
#include <serial_warning.hpp>
#include <set>
#include <string>
#include <unordered_map>

struct Metadata {
    // ================== 基本属性 ==================
    std::string path;     // 文件完整路径，包含目录和文件名
    std::string type;     // 文件类型（如 "txt", "jpg" 等）
    size_t size = 0;      // 文件大小，单位为字节
    bool hidden = false;  // 是否为隐藏文件（默认不隐藏）

    // ================== UNIX时间戳 ==================
    time_t creation_time = -1;      // 文件创建时间
    time_t modification_time = -1;  // 文件最后一次修改时间
    time_t access_time = -1;        // 文件最后一次访问时间
    time_t deletion_time = -1;      // 文件删除时间

    // ================== 权限和所有权 ==================
    std::set<std::string> owner_group = {"root"};    // 文件所属的用户组(完全控制权)
    std::set<std::string> read_permissions;          // 可读取文件的用户
    std::set<std::string> write_permissions;         // 可写入文件的用户
    std::set<std::string> modification_permissions;  // 可修改文件的用户

    // ================== 扩展属性 ==================
    std::string hash;         // 文件摘要
    std::string description;  // 文件描述
};

/**
 * @class Metadatabase
 * @brief 用于操作数据库中存储的元数据。
 *
 * 该类提供了对元数据表的操作接口，支持元数据的插入、更新、查询和删除。
 */
class Metadatabase {
   public:
   private:
    DataTable table(1, 2);
};
