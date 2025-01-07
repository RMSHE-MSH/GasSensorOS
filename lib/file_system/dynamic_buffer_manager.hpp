/**
 * @file dynamic_buffer_manager.hpp
 * @date 05.01.2025
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

class DynamicBufferManager {
   public:
    // 构造函数：根据文件大小初始化缓冲区大小
    explicit DynamicBufferManager(size_t fileSize) : fileSize(fileSize), currentBufferSize(computeBufferSize(fileSize)) {}

    // 根据文件大小计算适合的缓冲区大小
    size_t computeBufferSize(size_t size) {
        // 超小文件：最大4KB使用1KB缓冲区
        if (size <= 4 * SIZE_1KB) {
            return SIZE_1KB;
        }
        // 小文件：最大16KB使用2KB缓冲区
        if (size <= 16 * SIZE_1KB) {
            return SIZE_2KB;
        }
        // 中等文件：最大128KB使用8KB缓冲区
        if (size <= 128 * SIZE_1KB) {
            return SIZE_8KB;
        }
        // 大文件：最大512KB使用32KB缓冲区
        if (size <= 512 * SIZE_1KB) {
            return SIZE_32KB;
        }
        // 超大文件：超过512KB使用64KB缓冲区
        return SIZE_64KB;
    }

    // 获取当前缓冲区大小
    size_t getBufferSize() const { return currentBufferSize; }

    // 动态调整缓冲区大小：根据新的文件大小重新计算缓冲区
    void adjustBufferSize(size_t newFileSize) {
        // 更新文件大小并重新计算缓冲区
        fileSize = newFileSize;
        currentBufferSize = computeBufferSize(fileSize);
    }

    // 获取当前文件的大小
    size_t getFileSize() const { return fileSize; }

   private:
    size_t fileSize;           // 当前文件的大小
    size_t currentBufferSize;  // 当前的缓冲区大小

    // 定义不同文件大小范围的缓冲区大小（以字节为单位）
    static const size_t SIZE_1KB = 1 * 1024;    // 1KB
    static const size_t SIZE_2KB = 2 * 1024;    // 2KB
    static const size_t SIZE_8KB = 8 * 1024;    // 8KB
    static const size_t SIZE_32KB = 32 * 1024;  // 32KB
    static const size_t SIZE_64KB = 64 * 1024;  // 64KB
};
