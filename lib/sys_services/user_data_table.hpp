/**
 * @file user_data_table.hpp
 * @date 28.12.2024
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2024 RMSHE. All rights reserved.
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
#include <random.h>

#include <data_table.hpp>
#include <iomanip>
#include <iostream>
#include <pbkdf2_hmac_sha512.hpp>
#include <sstream>
#include <string>
#include <vector>

// 用户数据管理类
class USER_DATA {
   public:
    // 构造函数，初始化迭代次数和盐长度
    USER_DATA(uint32_t iterations = 10000, size_t salt_len = 64) : iterations_(iterations), salt_len_(salt_len), pbkdf2(iterations, 64) {}

    /**
     * @brief 添加用户
     * @param username 用户名
     * @param password 密码
     */
    void add_user(const std::string& username, const std::string& password) {
        // 判断用户名是否存在
        size_t row;
        if (query_user(username, row)) {
            WARN(WarningLevel::INFO, "用户已存在");
            return;
        }

        // 生成盐
        std::vector<uint8_t> salt = trueRandom(salt_len_);
        std::string salt_str = vector_to_string(salt);

        // 生成派生密钥
        std::string derived_key = pbkdf2.derive_key(password, salt);

        // 写入数据表
        table.insertRow({username, derived_key, salt_str});
    }

    /**
     * @brief 删除用户
     * @param username 用户名
     */
    void delete_user(const std::string& username) {
        // 判断用户名是否存在,若存在则获取用户名所在行位置
        size_t row;
        if (!query_user(username, row)) {
            WARN(WarningLevel::INFO, "用户不存在");
            return;
        }
        table.deleteRow(row);  // 删除用户数据
    }

    /**
     * @brief 验证用户的用户名和密码是否匹配
     * @param username 待验证的用户名
     * @param password 待验证的密码
     * @return true: 用户名和密码匹配; false: 用户名不存在或密码不正确
     */
    bool verify_user(const std::string& username, const std::string& password) {
        size_t row;  // 存储用户名所在的行号
        // 查询用户名是否存在
        if (!query_user(username, row)) {
            WARN(WarningLevel::INFO, "用户不存在");  // 输出警告信息：用户不存在
            return false;                            // 用户不存在，返回验证失败
        }

        // 提取数据库中存储的派生密钥和盐值
        std::string stored_key = table.getCell(row, 1);   // 获取派生密钥
        std::string stored_salt = table.getCell(row, 2);  // 获取盐值

        // 使用提供的密码和存储的盐值重新生成派生密钥，并与存储的派生密钥比较
        std::string derived_key = pbkdf2.derive_key(password, string_to_vector(stored_salt));

        if (derived_key == stored_key) return true;  // 验证通过，返回 true
        return false;                                // 验证失败，返回 false
    }

    // 打印用户表
    void print() { table.printTable(); }

   private:
    /**
     * @brief 判断用户名是否存在
     * @param username 要查找的用户名
     * @param row 若用户名存在,则返回用户名所在行的位置
     * @return true: 存在; false: 不存在;
     */
    bool query_user(const std::string& username, size_t& row) {
        // 查询用户名并获取行号
        auto result = table.query(username);
        if (!result.empty()) {
            row = result[0].first;
            return true;  // 用户存在
        }
        row = -1;  // 用户不存在
        return false;
    }

    /**
     * @brief 将字节向量转换为字符串（每个元素为0到9之间的数字字符）
     * @param vec 输入的字节向量
     * @return std::string 返回转换后的字符串
     */
    std::string vector_to_string(const std::vector<uint8_t>& vec) {
        // 使用reserve提前分配内存，避免频繁的内存重新分配
        std::string result;
        result.reserve(vec.size());  // 预先分配足够的内存

        // 遍历字节向量，将每个字节转换为字符并拼接到结果字符串中
        for (auto byte : vec) result += '0' + byte;

        return result;
    }

    /**
     * @brief 将字符串转换为字节向量（每个字符为0到9之间的数字字符）
     * @param str 输入的字符串
     * @return std::vector<uint8_t> 返回转换后的字节向量
     */
    std::vector<uint8_t> string_to_vector(const std::string& str) {
        std::vector<uint8_t> result;
        result.reserve(str.size());  // 预分配足够的空间，减少动态分配

        // 遍历字符串，将每个字符转换为数字并添加到结果向量
        for (auto ch : str) {
            // 字符是 '0' 到 '9' 之间的数字，转化为 uint8_t 数字
            if (ch >= '0' && ch <= '9') {
                result.push_back(ch - '0');  // 字符 '0' 的 ASCII 值是 48，'1' 是 49，以此类推
            } else {
                // 如果字符不在 '0' 到 '9' 之间，可以根据需求进行错误处理
                WARN(WarningLevel::ERROR, "盐字符串包含无效字符,只能是0到9的数字字符");
            }
        }

        return result;
    }

   private:
    uint32_t iterations_;  // 迭代次数
    size_t salt_len_;      // 盐的长度

    // 创建 PBKDF2 对象，使用默认迭代次数和输出长度
    PBKDF2_HMAC_SHA512 pbkdf2;

    // 创建用户数据库表
    DataTable table{1, 3, ""};
};
