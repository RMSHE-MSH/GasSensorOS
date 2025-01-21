/**
 * @file directory_manager.hpp
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
#include <file_manager.hpp>
#include <fs_interface.hpp>
#include <serial_warning.hpp>
#include <string>
#include <string_edit.hpp>

class DirectoryManager {
   public:
    /**
     * @brief 创建指定路径的空目录
     *
     * 该函数会根据输入的目录路径逐层创建每一个目录。如果路径中某一部分已存在，则跳过该部分，
     * 如果路径中某一部分不存在，则会创建该部分。路径中的目录名必须合法。
     *
     * @param dirPath 要创建的目录路径，格式为“/dir1/dir2/.../dirN”。
     *
     * @return 如果所有目录创建成功，返回true；否则返回false。
     */
    bool createDir(const std::string& dirPath) {
        // 使用分隔符“/”分割目录路径
        std::vector<std::string> dir_names = splitter.split(dirPath, {"/"});

        std::string base_path = "";  // 当前目录路径初始化为空

        // 遍历每个目录名，逐层创建目录
        for (const auto& dir_name : dir_names) {
            // 检查目录名是否合法
            if (!isValidDirName(dir_name)) {
                WARN(WarningLevel::ERROR, "目录名包含非法字符: %s", dir_name.c_str());
                return false;  // 如果有非法目录名，则返回false
            }

            base_path = base_path + "/" + dir_name;  // 拼接当前目录路径

            // 如果当前目录不存在，则创建该目录
            if (!fs.exists(base_path)) {
                if (!fs.mkdir(base_path)) {
                    WARN(WarningLevel::ERROR, "无法创建目录: %s", base_path.c_str());
                    return false;  // 创建目录失败，返回false
                }
            }
        }

        // 所有目录创建成功，返回true
        return true;
    }

    /**
     * @brief 递归复制目录及其内容
     *
     * @param sourceDirPath 源目录路径
     * @param targetDirPath 目标目录路径
     * @return bool 复制成功返回true，失败返回false
     *
     * @details 该函数会递归复制源目录中的所有文件和子目录到目标目录。
     *          如果目标目录是源目录的子目录，函数将避免进行复制操作。
     *          函数内部会进行路径检查和创建目录操作，并在复制过程中保持源目录结构。
     *          复制文件时，会调用`copyFile`方法复制单个文件。
     */
    bool copyDir(const std::string& sourceDirPath, const std::string& targetDirPath) {
        // 检查源目录是否存在
        if (!fs.exists(sourceDirPath)) {
            WARN(WarningLevel::ERROR, "源目录不存在: %s", sourceDirPath.c_str());
            return false;
        }

        // 检查目标目录是否是源目录的子目录，避免死循环
        if (targetDirPath.find(sourceDirPath) == 0) {
            WARN(WarningLevel::ERROR, "目标目录是源目录的子目录，无法复制: %s -> %s", sourceDirPath.c_str(), targetDirPath.c_str());
            return false;
        }

        // 创建目标目录，如果目录创建失败，则返回错误
        if (!createDir(targetDirPath)) {
            WARN(WarningLevel::ERROR, "创建目标目录失败: %s", targetDirPath.c_str());
            return false;
        }

        // 获取源目录中的文件和子目录列表
        std::vector<std::string> items = listDir(sourceDirPath);
        for (const auto& item : items) {
            // 拼接源目录和目标目录的完整路径
            std::string sourceItem = sourceDirPath + "/" + item;
            std::string targetItem = targetDirPath + "/" + item;

            // 判断是文件还是目录
            if (fs.isDirectory(sourceItem)) {
                // 递归复制子目录
                if (!copyDir(sourceItem, targetItem)) {
                    return false;  // 如果复制子目录失败，直接返回错误
                }
            } else {
                // 复制文件
                if (!file.copyFile(sourceItem, targetItem)) {
                    return false;  // 如果复制文件失败，直接返回错误
                }
            }
        }

        return true;  // 所有文件和子目录复制成功
    }

    /**
     * @brief 删除指定路径的空目录及其空子目录
     *
     * 该函数递归地遍历目录并删除所有空目录。若目录包含文件或非空子目录，则跳过该目录。删除时会先检查目录是否为空，
     * 仅删除那些确实为空的目录。注意，该函数不会删除包含文件的目录。
     *
     * @param dirPath [in] 要删除的目录路径
     * @return `true` 删除成功，`false` 删除失败
     */
    bool deleteVoidDir(const std::string& dirPath) {
        // 检查目标路径是否存在并且为目录
        if (!fs.exists(dirPath) || !fs.isDirectory(dirPath)) {
            WARN(WarningLevel::ERROR, "目录不存在或路径不是目录: %s", dirPath.c_str());
            return false;  // 目录不存在或路径不是目录
        }

        // 打开目录并获取文件和子目录列表
        if (!fs.openDir(dirPath)) {
            WARN(WarningLevel::ERROR, "无法打开目录: %s", dirPath.c_str());
            return false;  // 打开目录失败
        }

        // 获取当前目录下的所有文件和子目录
        std::vector<std::string> names = fs.listFiles();

        // 递归删除子目录
        for (const auto& name : names) {
            std::string subDirPath = dirPath + "/" + name;

            // 递归删除子目录
            if (!deleteVoidDir(subDirPath)) {
                fs.close();    // 关闭目录
                return false;  // 删除子目录失败，退出
            }
        }

        // 删除当前目录（仅当目录为空时删除）
        bool result = false;
        if (!hasFiles(dirPath)) {
            result = fs.rmdir(dirPath);
            // 删除空目录
            if (!result) WARN(WarningLevel::ERROR, "删除目录失败: %s", dirPath.c_str());
        }

        // 关闭目录
        fs.close();

        return result;  // 返回删除结果
    }

    /**
     * @brief 递归删除指定目录
     *
     * 该函数会删除指定路径目录,包括这个目录中的一切内容。
     *
     * @param path 要删除的文件或目录路径
     * @return 如果删除成功返回true，失败时返回false
     */
    bool deleteDir(const std::string& dirPath) { return fs.deletePath(dirPath); }

    /**
     * @brief 获取指定目录下的所有文件或目录的名称列表
     * @param dirPath 目录路径
     * @return 文件名列表
     */
    std::vector<std::string> listDir(const std::string& dirPath) {
        fs.openDir(dirPath);
        return fs.listFiles();
    }

   public:
    /**
     * @brief 获取目录路径中的目录名部分
     *
     * 该函数从给定的目录路径中提取出目录名部分。它会查找最后一个路径分隔符（'/' 或 '\\'），
     * 并返回该分隔符之后的子字符串作为目录名。如果没有找到路径分隔符，函数会返回一个空字符串。
     * [示例: "/test" -> "test"; "/dir/test" -> "test";]
     *
     * @param filePath 输入的完整目录路径
     * @return 返回目录名部分。如果未找到路径分隔符，返回空字符串。
     */
    std::string getDirName(const std::string& dirPath) {
        // 查找最后一个路径分隔符的位置（支持 UNIX 和 Windows 路径格式）
        size_t pos = dirPath.find_last_of("/\\");

        // 如果没有找到路径分隔符，返回空字符串
        if (pos == std::string::npos) return "";

        // 返回从分隔符后的字符开始的子字符串，作为目录名
        return dirPath.substr(pos + 1);
    }

    /**
     * @brief 检查目录名是否合法
     *
     * 目录名不能包含非法字符，包括："/\\:*?\"<>|"
     *
     * @param dirName[in] 待检查的目录名
     * @return true 目录名合法
     * @return false 目录名不合法
     */
    bool isValidDirName(const std::string& dirName) {
        static const std::string invalidChars = "/\\:*?\"<>|";
        return dirName.find_first_of(invalidChars) == std::string::npos;
    }

    /**
     * @brief 判断目录及其子目录下是否存在文件
     *
     * 该函数递归地遍历指定目录及其子目录，检查是否包含任何文件。若发现有文件，则立即返回 `true`，
     * 否则返回 `false`。该函数能有效避免对已知无文件的目录进行不必要的检查，提升性能。
     *
     * @param dirPath [in] 要检查的目录路径
     * @return `true` 目录或其子目录下存在文件
     * @return `false` 目录及其子目录下没有文件
     */
    bool hasFiles(const std::string& dirPath) {
        // 检查目录是否存在并且是一个目录
        if (!fs.exists(dirPath) || !fs.isDirectory(dirPath)) return false;  // 目录不存在或路径不是目录

        // 打开目录并获取文件列表
        if (!fs.openDir(dirPath)) return false;  // 打开目录失败

        // 获取当前目录下的所有文件和子目录
        std::vector<std::string> files = fs.listFiles();

        // 遍历目录中的每一个项
        for (const auto& file : files) {
            std::string fullPath = dirPath + "/" + file;

            // 打开当前文件
            if (!fs.open(fullPath, "r")) continue;  // 打开文件失败，跳过该项

            // 判断该项是否为文件
            if (!fs.isDirectory()) {
                fs.close();   // 关闭文件
                return true;  // 发现文件，立即返回
            }

            // 递归检查子目录
            if (hasFiles(fullPath)) {
                fs.close();   // 关闭文件
                return true;  // 发现文件后立即返回
            }

            fs.close();  // 关闭文件
        }

        // 所有子目录都为空，返回 false
        return false;
    }

   private:
    StringSplitter splitter;

    FSInterface fs;  // 底层文件系统接口实例
    FileManager file;
};
