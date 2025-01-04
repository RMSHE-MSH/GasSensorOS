/**
 * @file fs_Interface.hpp
 * @date 03.01.2025
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
#include <Arduino.h>
#include <LittleFS.h>

#include <cstring>
#include <string>
#include <vector>

/**
 * @brief 文件系统抽象接口层
 *
 * 提供通用的文件系统操作接口，屏蔽底层存储实现（如LittleFS，SD，FATFS等），
 * 使得上层应用可以无缝切换存储后端。通过该接口，可以进行文件操作、目录操作、文件系统管理等。
 */
class FSInterface {
   public:
    /**
     * @brief 挂载LittleFS文件系统
     * 该函数负责初始化并挂载LittleFS文件系统。若挂载失败，将打印错误信息。
     * @return true 挂载成功
     * @return false 挂载失败
     */
    bool mount() {
        if (!LittleFS.begin()) {
            WARN(WarningLevel::ERROR, "LittleFS文件系统挂载失败");
            return false;
        }
        return true;
    }

    /**
     * @brief 卸载LittleFS文件系统
     *
     * 该函数负责卸载文件系统，释放相应的资源。
     */
    void unmount() { LittleFS.end(); }

   public:
    /**
     * @brief 打开文件
     * @param path 文件路径
     * @param mode 打开模式;
     * "r":只读,要求文件必须存在;
     * "w":覆写,文件不存在则创建,存在则清空;
     * "a":追加,文件不存在则创建;
     * "r+":读写,要求文件必须存在;
     * "w+":覆写读,文件不存在则创建,存在则清空;
     * @return true 打开成功
     * @return false 打开失败
     */
    bool open(const std::string& path, const char* mode) {
        work_file = LittleFS.open(path.c_str(), mode);
        if (!work_file || work_dir.isDirectory()) {
            WARN(WarningLevel::ERROR, "打开文件失败: %s", path.c_str());
            work_file.close();
            return false;
        }
        return true;
    }

    /**
     * @brief 打开目录
     * @param path 目录路径
     * @return true 打开成功
     * @return false 打开失败
     */
    bool openDir(const std::string& path) {
        work_dir = LittleFS.open(path.c_str());
        if (!work_dir || !work_dir.isDirectory()) {
            WARN(WarningLevel::ERROR, "打开目录失败: %s", path.c_str());
            work_dir.close();
            return false;
        }
        return true;
    }

    /**
     * @brief 关闭文件
     * @return true 关闭成功
     * @return false 关闭失败
     */
    bool close() {
        work_file.close();
        if (work_file) WARN(WarningLevel::ERROR, "文件关闭失败: %s", work_file.name());
        return !work_file;
    }

    /**
     * @brief 从文件中读取数据
     * @param buffer 存储数据的缓冲区，函数将读取的数据写入这个内存区域。
     * @param size 要读取的字节数，即你期望从文件中读取多少字节。
     * @return 实际读取的字节数。如果文件内容少于请求的字节数，则返回实际读取的字节数；如果遇到错误或文件末尾，则返回-1。
     */
    size_t read(void* buffer, size_t size) {
        uint8_t* buf = static_cast<uint8_t*>(buffer);
        return work_file.read(buf, size);
    }

    /**
     * @brief 向文件中写入数据
     * @param buffer 指向数据源的指针。可以是任何类型的数据，但在函数内部会将其转换为 const uint8_t*（即字节流）
     * @param size 要写入的数据量，单位是字节。
     * @return 实际写入的字节数。
     */
    size_t write(const void* buffer, size_t size) { return work_file.write(reinterpret_cast<const uint8_t*>(buffer), size); }

    /**
     * @brief 获取文件大小(字节)
     * @return 文件大小，若文件不存在返回-1
     */
    size_t getSize() {
        if (work_file) return work_file.size();

        return -1;  // 如果文件不存在，则返回-1
    }

    /**
     * @brief 获取文件的最后修改时间
     * 以UNIX时间戳表示（自1970年1月1日以来的秒数）
     * @return 文件的最后修改时间,获取失败则返回0
     */
    uint32_t getLastWrite() {
        if (!work_file) {
            WARN(WarningLevel::ERROR, "没有打开文件");
            return 0;
        }
        return work_file.getLastWrite();
    }

    /**
     * @brief 返回当前文件的文件名
     * @return 成功则返回文件名，失败则返回空字符串
     */
    std::string name() {
        if (!work_file) {
            WARN(WarningLevel::ERROR, "没有打开文件");
            return "";
        }
        return work_file.name();
    }

   public:
    /**
     * @brief 删除文件或空目录
     * @param path 文件或空目录路径
     * @return true 删除成功
     * @return false 删除失败
     */
    bool remove(const std::string& path) {
        if (LittleFS.remove(path.c_str())) {
            return true;
        } else {
            WARN(WarningLevel::ERROR, "删除失败: %s", path.c_str());
            return false;
        }
    }

    /**
     * @brief 重命名或移动文件
     * @param old_path 原路径
     * @param new_path 新路径
     * @return true 成功
     * @return false 失败
     */
    bool rename(const std::string& old_path, const std::string& new_path) {
        if (LittleFS.rename(old_path.c_str(), new_path.c_str())) {
            return true;
        } else {
            WARN(WarningLevel::ERROR, "重命名或移动失败: %s", old_path.c_str());
            return false;
        }
    }

    /**
     * @brief 创建空目录
     * @param path 目录路径
     * @return true 成功
     * @return false 失败
     */
    bool mkdir(const std::string& path) {
        if (LittleFS.mkdir(path.c_str())) {
            return true;
        } else {
            WARN(WarningLevel::ERROR, "目录创建失败: %s", path.c_str());
            return false;
        }
    }

    /**
     * @brief 删除空目录
     * @param path 目录路径
     * @return true 成功
     * @return false 失败
     */
    bool rmdir(const std::string& path) {
        if (LittleFS.rmdir(path.c_str())) {
            return true;
        } else {
            WARN(WarningLevel::ERROR, "目录删除失败: %s", path.c_str());
            return false;
        }
    }

    /**
     * @brief 判断是否为目录
     * 该函数判断当前操作的文件是否为目录。
     * @return true 是目录
     * @return false 不是目录
     */
    bool isDirectory() { return work_file.isDirectory(); }

    /**
     * @brief 获取当前目录下的下一个文件或目录的名称
     * @return 当前文件或目录的名称，若没有更多文件，返回空字符串
     */
    std::string dirNextName() {
        if (work_dir) {
            File file = work_dir.openNextFile();
            if (file) {
                return std::string(file.name());
            }
        }
        return "";
    }

    /**
     * @brief 获取当前打开目录下的所有文件或目录的名称列表
     * @return 文件名列表
     */
    std::vector<std::string> listFiles() {
        std::vector<std::string> list;
        if (work_dir) {
            File file = work_dir.openNextFile();
            while (file) {
                list.push_back(file.name());
                file = work_dir.openNextFile();
            }
        }
        return list;
    }

    /**
     * @brief 重置目录遍历指针
     * 该函数将当前目录指针重置到目录的开头。
     */
    void rewindDirectory() { work_dir.rewindDirectory(); }

    /**
     * @brief 检查指定路径的文件或目录是否存在
     * @param path 文件或目录路径
     * @return true 存在
     * @return false 不存在
     */
    bool exists(const std::string& path) { return LittleFS.exists(path.c_str()); }

    /**
     * @brief 检查当前打开的文件中是否还有数据可读取
     * @return 返回从当前读取位置到文件末尾之间剩余的字节数; 为 0 时表示文件已读到末尾，没有更多数据可读取。
     */
    int available() { return work_file.available(); }

    /**
     * @brief 获取文件系统的总空间
     * @return 文件系统的总空间(字节)
     */
    size_t totalSpace() { return LittleFS.totalBytes(); }

    /**
     * @brief 获取文件系统的已用空间
     * @return 文件系统的已用空间(字节)
     */
    size_t usedSpace() { return LittleFS.usedBytes(); }

    /**
     * @brief 格式化文件系统。
     * 这将删除所有文件和目录，恢复文件系统到初始状态
     * @return true 格式化成功
     * @return false 格式化失败
     */
    bool format() {
        if (LittleFS.format()) {
            return true;
        } else {
            WARN(WarningLevel::ERROR, "无法格式化文件系统");
            return false;
        }
    }

    /**
     * @brief 强制将缓存中的数据写入文件
     */
    void sync() { work_file.flush(); }

   private:
    File work_file;  // 当前操作的文件对象
    File work_dir;   // 当前操作的目录对象
};