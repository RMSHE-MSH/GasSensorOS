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
#include <sqlite3.h>

#include <chrono>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

struct Metadata {
    // ================== 基本属性 ==================
    std::string path;     // 文件完整路径，包含目录和文件名
    std::string type;     // 文件类型（如 "txt", "jpg" 等）
    size_t size = 0;      // 文件大小，单位为字节
    bool hidden = false;  // 是否为隐藏文件（默认不隐藏）

    // ================== UNIX时间戳 ==================
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

class Metadatabase {
   public:
    explicit Metadatabase(const std::string& db_path) : database_path(db_path) {
        if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
            std::cerr << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
            db = nullptr;
        } else {
            createTable();  // 初始化数据库表结构
        }
    }

    ~Metadatabase() {
        if (db) {
            sqlite3_close(db);
        }
    }

    // 插入或更新元数据
    bool upsertMetadata(const Metadata& meta) {
        // 更新缓存
        metadata_table[meta.path] = meta;

        // 插入或更新数据库
        const char* sql = R"(
            INSERT INTO Metadata (path, type, size, hidden, creation_time, modification_time, access_time, deletion_time, 
                                  owner_group, read_permissions, write_permissions, modification_permissions, 
                                  hash, description, version, author, diff_path)
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            ON CONFLICT(path) DO UPDATE SET
                type = excluded.type,
                size = excluded.size,
                hidden = excluded.hidden,
                modification_time = excluded.modification_time,
                access_time = excluded.access_time,
                deletion_time = excluded.deletion_time,
                owner_group = excluded.owner_group,
                read_permissions = excluded.read_permissions,
                write_permissions = excluded.write_permissions,
                modification_permissions = excluded.modification_permissions,
                hash = excluded.hash,
                description = excluded.description,
                version = excluded.version,
                author = excluded.author,
                diff_path = excluded.diff_path;
        )";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "无法预处理 SQL 语句: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        bindMetadataToStatement(meta, stmt);
        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        return result == SQLITE_DONE;
    }

    // 查询元数据
    Metadata queryMetadata(const std::string& path) {
        if (metadata_table.count(path)) {
            return metadata_table[path];
        }

        Metadata meta;
        const char* sql = "SELECT * FROM Metadata WHERE path = ?";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "无法预处理 SQL 语句: " << sqlite3_errmsg(db) << std::endl;
            return meta;
        }

        sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            populateMetadataFromRow(stmt, meta);
        }
        sqlite3_finalize(stmt);

        return meta;
    }

    // 删除元数据
    bool deleteMetadata(const std::string& path) {
        metadata_table.erase(path);

        const char* sql = "DELETE FROM Metadata WHERE path = ?";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "无法预处理 SQL 语句: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_STATIC);
        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        return result == SQLITE_DONE;
    }

   private:
    void createTable() {
        const char* sql = R"(
            CREATE TABLE IF NOT EXISTS Metadata (
                path TEXT PRIMARY KEY,
                type TEXT,
                size INTEGER,
                hidden INTEGER,
                creation_time INTEGER,
                modification_time INTEGER,
                access_time INTEGER,
                deletion_time INTEGER,
                owner_group TEXT,
                read_permissions TEXT,
                write_permissions TEXT,
                modification_permissions TEXT,
                hash TEXT,
                description TEXT,
                version INTEGER,
                author TEXT,
                diff_path TEXT
            );
        )";
        if (sqlite3_exec(db, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
            std::cerr << "创建表失败: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    void bindMetadataToStatement(const Metadata& meta, sqlite3_stmt* stmt) {
        // 绑定元数据到 SQLite 语句
        sqlite3_bind_text(stmt, 1, meta.path.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, meta.type.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 3, meta.size);
        sqlite3_bind_int(stmt, 4, meta.hidden);
        sqlite3_bind_int64(stmt, 5, std::chrono::duration_cast<std::chrono::seconds>(meta.creation_time.time_since_epoch()).count());
        sqlite3_bind_int64(stmt, 6, std::chrono::duration_cast<std::chrono::seconds>(meta.modification_time.time_since_epoch()).count());
        sqlite3_bind_int64(stmt, 7, std::chrono::duration_cast<std::chrono::seconds>(meta.access_time.time_since_epoch()).count());
        sqlite3_bind_int64(stmt, 8, std::chrono::duration_cast<std::chrono::seconds>(meta.deletion_time.time_since_epoch()).count());
        sqlite3_bind_text(stmt, 9, joinSet(meta.owner_group).c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 10, joinSet(meta.read_permissions).c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 11, joinSet(meta.write_permissions).c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 12, joinSet(meta.modification_permissions).c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 13, meta.hash.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 14, meta.description.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 15, meta.version);
        sqlite3_bind_text(stmt, 16, meta.author.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 17, meta.diff_path.c_str(), -1, SQLITE_STATIC);
    }

    void populateMetadataFromRow(sqlite3_stmt* stmt, Metadata& meta) {
        meta.path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        meta.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        meta.size = sqlite3_column_int64(stmt, 2);
        meta.hidden = sqlite3_column_int(stmt, 3) != 0;
        meta.creation_time = std::chrono::system_clock::time_point(std::chrono::seconds(sqlite3_column_int64(stmt, 4)));
        meta.modification_time = std::chrono::system_clock::time_point(std::chrono::seconds(sqlite3_column_int64(stmt, 5)));
        meta.access_time = std::chrono::system_clock::time_point(std::chrono::seconds(sqlite3_column_int64(stmt, 6)));
        meta.deletion_time = std::chrono::system_clock::time_point(std::chrono::seconds(sqlite3_column_int64(stmt, 7)));
        meta.owner_group = splitSet(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)));
        meta.read_permissions = splitSet(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9)));
        meta.write_permissions = splitSet(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
        meta.modification_permissions = splitSet(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11)));
        meta.hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        meta.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
        meta.version = sqlite3_column_int(stmt, 14);
        meta.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
        meta.diff_path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 16));
    }

    std::string joinSet(const std::set<std::string>& set) {
        std::ostringstream oss;
        for (const auto& item : set) {
            if (&item != &*set.begin()) oss << ",";
            oss << item;
        }
        return oss.str();
    }

    std::set<std::string> splitSet(const std::string& str) {
        std::set<std::string> result;
        std::istringstream iss(str);
        std::string item;
        while (std::getline(iss, item, ',')) {
            result.insert(item);
        }
        return result;
    }

   private:
    sqlite3* db = nullptr;
    std::string database_path;
    std::unordered_map<std::string, Metadata> metadata_table;  // 元数据缓存(文件路径与元数据的映射)
};
