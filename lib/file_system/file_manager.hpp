/**
 * @file file_manager.hpp
 * @date 04.01.2025
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
#include <fs_Interface.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief 文件管理器类，用于封装常见的文件操作和缓存管理。
 *
 * 该类封装了文件系统接口，提供了创建、删除、打开、读取、写入等常见文件操作。
 * 同时，它还提供了缓存管理，以提升文件操作性能，并确保数据同步到存储设备。
 */
class FileManager {
   public:
   private:
    FSInterface fs;
};
