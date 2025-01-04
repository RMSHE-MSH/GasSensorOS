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
#include <algorithm>
#include <cctype>
#include <fs_Interface.hpp>
#include <string>
#include <string_edit.hpp>
#include <vector>

class FileManager {
   public:
    /**
     * @brief 构造函数，初始化并挂载文件系统
     * 在创建 FileManager 对象时，会自动挂载文件系统。
     */
    FileManager() { fs.mount(); }

    /**
     * @brief 创建文件
     * 检查文件是否已存在，如果已存在则返回 false，避免覆盖现有文件。
     * 否则，创建一个新的空文件。
     * @param filePath 文件路径
     * @return 如果文件创建成功返回 true，否则返回 false
     */
    bool createFile(const std::string& filePath) {
        if (fs.exists(filePath)) {
            WARN(WarningLevel::ERROR, "文件已存在,操作终止: %s", filePath.c_str());
            return false;  // 文件已存在，不创建
        }
        fs.open(filePath, "w");
        fs.close();
        return true;
    }

    /**
     * @brief 删除文件
     * 检查文件是否存在，如果不存在则返回 false。如果文件存在，则执行删除操作。
     * @param filePath 文件路径
     * @return 如果文件删除成功返回 true，否则返回 false
     */
    bool deleteFile(const std::string& filePath) {
        if (!fs.exists(filePath)) {
            WARN(WarningLevel::ERROR, "文件不存在,操作终止: %s", filePath.c_str());
            return false;  // 文件不存在，无法删除
        }
        return fs.remove(filePath);
    }

    /**
     * @brief 重命名文件
     * 如果文件存在且新的文件名合法，尝试重命名文件。
     * @param filePath 文件路径
     * @param newName 新文件名
     * @return 如果文件重命名成功返回 true，否则返回 false
     */
    bool renameFile(const std::string& filePath, const std::string& newName) {
        if (!fs.exists(filePath)) {
            WARN(WarningLevel::ERROR, "文件不存在,操作终止: %s", filePath.c_str());
            return false;  // 文件不存在，无法重命名
        }

        // 获取文件目录路径
        std::string fileDirPath = getDirectoryPath(filePath);
        std::string name, extension;

        // 解析新文件名，获取主名和扩展名
        if (!fileNameAnalysis(newName, name, extension)) {
            WARN(WarningLevel::ERROR, "文件重命名失败: %s", filePath.c_str());
            return false;  // 文件名解析失败，重命名操作失败
        }

        // 组合新的文件路径并进行重命名
        return fs.rename(filePath, fileDirPath + "/" + name + "." + extension);
    }

    /**
     * @brief 读取文件内容为字符串
     * 该函数读取文件内容并返回一个字符串。如果文件不存在或读取失败，则返回空字符串。
     * @param filePath 文件路径
     * @return 返回文件内容字符串，失败时返回空字符串
     */
    std::string readFileAsString(const std::string& filePath) {
        // 检查文件是否存在
        if (!fs.exists(filePath)) {
            WARN(WarningLevel::ERROR, "文件不存在: %s", filePath.c_str());
            return "";  // 返回空字符串表示读取失败
        }

        // 尝试打开文件
        if (!fs.open(filePath, "r")) return "";  // 返回空字符串表示读取失败

        std::string result;
        const size_t bufferSize = 1024;  // 缓冲区大小
        char buffer[bufferSize];
        size_t bytesRead;

        // 持续读取文件内容，直到文件结束
        while ((bytesRead = fs.read(buffer, bufferSize)) > 0) {
            result.append(buffer, bytesRead);  // 将读取到的数据追加到结果字符串
        }

        fs.close();  // 关闭文件
        return result;
    }

    /**
     * @brief 读取文件内容为字节数组
     * 该函数读取文件内容并返回一个字节数组。如果文件不存在或读取失败，则返回空字节数组。
     * @param filePath 文件路径
     * @return 返回文件内容字节数组，失败时返回空字节数组
     */
    std::vector<uint8_t> readFileAsBytes(const std::string& filePath) {
        // 检查文件是否存在
        if (!fs.exists(filePath)) {
            WARN(WarningLevel::ERROR, "文件不存在: %s", filePath.c_str());
            return {};  // 返回空字节数组表示读取失败
        }

        // 尝试打开文件
        if (!fs.open(filePath, "r")) return {};  // 返回空字节数组表示读取失败

        std::vector<uint8_t> result;
        const size_t bufferSize = 1024;  // 缓冲区大小
        uint8_t buffer[bufferSize];
        size_t bytesRead;

        // 持续读取文件内容，直到文件结束
        while ((bytesRead = fs.read(buffer, bufferSize)) > 0) {
            result.insert(result.end(), buffer, buffer + bytesRead);  // 将读取到的数据追加到字节数组
        }

        fs.close();  // 关闭文件
        return result;
    }

    /**
     * @brief 向文件写入字符串数据
     * 该函数将字符串数据写入指定的文件。如果文件不存在，默认会创建文件；如果文件存在，按指定的模式写入。
     * @param filePath 文件路径
     * @param data 要写入的数据（字符串类型）
     * @param mode 文件打开模式，默认为追加模式（"a"）。如果为"w"，则会覆盖文件。
     * @return 返回写入是否成功，成功返回 true，失败返回 false。
     */
    bool writeFileAsString(const std::string& filePath, const std::string& data, const char* mode = "a") {
        // 验证文件打开模式是否合法
        if (std::string(mode) != "w" && std::string(mode) != "a") {
            WARN(WarningLevel::ERROR, "文件打开模式非法,仅支持(w:覆写,a:追加): %s", mode);
            return false;  // 返回 false 表示非法模式
        }

        // 尝试打开文件进行写入
        if (!fs.open(filePath, mode)) return false;  // 打开文件失败

        // 获取要写入的数据大小
        size_t dataSize = data.size();
        size_t bytesWritten = fs.write(data.c_str(), dataSize);

        fs.close();                       // 关闭文件
        return bytesWritten == dataSize;  // 确保写入字节数与数据大小一致
    }

    /**
     * @brief 向文件写入字节数组数据
     * 该函数将字节数组数据写入指定的文件。如果文件不存在，默认会创建文件；如果文件存在，按指定的模式写入。
     * @param filePath 文件路径
     * @param data 要写入的数据（字节数组类型）
     * @param mode 文件打开模式，默认为追加模式（"a"）。如果为"w"，则会覆盖文件。
     * @return 返回写入是否成功，成功返回 true，失败返回 false。
     */
    bool writeFileAsBytes(const std::string& filePath, const std::vector<uint8_t>& data, const char* mode = "a") {
        // 验证文件打开模式是否合法
        if (std::string(mode) != "w" && std::string(mode) != "a") {
            WARN(WarningLevel::ERROR, "文件打开模式非法,仅支持(w:覆写,a:追加): %s", mode);
            return false;  // 返回 false 表示非法模式
        }

        // 尝试打开文件进行写入
        if (!fs.open(filePath, mode)) return false;  // 打开文件失败

        // 获取要写入的数据大小
        size_t dataSize = data.size();
        size_t bytesWritten = fs.write(data.data(), dataSize);

        fs.close();                       // 关闭文件
        return bytesWritten == dataSize;  // 确保写入字节数与数据大小一致
    }

   private:
    /**
     * @brief 获取文件路径中的目录部分
     *
     * 该函数从给定的文件路径中提取出目录路径部分。它会查找最后一个路径分隔符（'/' 或 '\\'），
     * 并返回该分隔符之前的子字符串作为目录路径。如果没有找到路径分隔符，函数会返回一个空字符串。
     * [示例: "/test.txt" -> ""; "/dir/test.txt" -> "/dir";]
     *
     * @param filePath 输入的完整文件路径
     * @return 返回文件的目录路径部分。如果未找到路径分隔符，返回空字符串。
     */
    std::string getDirectoryPath(const std::string& filePath) {
        // 查找最后一个路径分隔符的位置（支持 UNIX 和 Windows 路径格式）
        size_t pos = filePath.find_last_of("/\\");

        // 如果没有找到路径分隔符，返回空字符串
        if (pos == std::string::npos) return "";

        // 返回路径分隔符之前的部分作为目录路径
        return filePath.substr(0, pos);
    }

    // 检查文件名是否合法（不包含非法字符）
    bool isValidFileName(const std::string& fileName) {
        // 定义常见非法字符
        const std::string invalidChars = "/\\:*?\"<>|";

        // 遍历文件名，检查是否包含非法字符
        for (char ch : fileName) {
            if (invalidChars.find(ch) != std::string::npos) {
                return false;  // 如果包含非法字符，返回 false
            }
        }
        return true;  // 文件名合法
    }

    /**
     * @brief 文件名解析函数：解析文件的主名和扩展名，同时检查文件名是否合法。
     * @param fileName[in] 完整的文件名（例如 "example.txt"）。
     * @param name[out] 输出文件的主名部分（例如 "example"）。
     * @param extension[out] 输出文件的扩展名部分（例如 "txt"）。
     * @return 如果文件名合法返回 true，否则返回 false。
     */
    bool fileNameAnalysis(const std::string& fileName, std::string& name, std::string& extension) {
        // 1. 检查文件名是否合法
        if (!isValidFileName(fileName)) {
            WARN(WarningLevel::ERROR, "文件名包含非法字符: %s", fileName.c_str());
            return false;  // 如果文件名非法，返回 false
        }

        // 2. 查找最后一个 '.' 的位置，分割主名和扩展名
        size_t dotPos = fileName.rfind('.');
        if (dotPos == std::string::npos) {
            // 如果没有找到 '.'，说明没有扩展名
            name = fileName;    // 主名就是整个文件名
            extension.clear();  // 扩展名为空
        } else {
            // 提取主名（文件名的前部分）
            name = fileName.substr(0, dotPos);
            // 提取扩展名（文件名的后部分，去掉 '.')
            extension = fileName.substr(dotPos + 1);
        }
        // 返回文件名解析成功
        return true;
    }

   private:
    FSInterface fs;  // 底层文件系统接口
    StringSplitter splitter;
};
