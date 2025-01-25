/**
 * @file version_manager.hpp
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

class VersionManager {
   private:
    std::map<std::string, std::vector<FileVersion>> versionHistory;  // 每个文件的版本历史
    std::string basePath;                                            // 版本存储的基础路径

   public:
    VersionManager(const std::string& basePath) : basePath(basePath) {}

    // 创建版本（支持快照和差异存储）
    bool createVersion(const std::string& filePath, bool isSnapshot);

    // 恢复指定版本
    bool rollbackToVersion(const std::string& filePath, int versionNumber);

    // 撤销最近一次修改
    bool undoLastChange(const std::string& filePath);

    // 删除旧版本以释放空间
    bool cleanupOldVersions(const std::string& filePath, int maxVersions);
};
