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
#include <metadatabase.hpp>
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
};
