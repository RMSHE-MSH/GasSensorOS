/**
 * @file pbkdf2_hmac_sha512.hpp
 * @date 26.12.2024
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
#include <mbedtls/md.h>
#include <mbedtls/pkcs5.h>

#include <iostream>
#include <serial_warning.hpp>
#include <stdexcept>
#include <string>
#include <vector>

class PBKDF2_HMAC_SHA512 {
   public:
    // 构造函数，初始化迭代次数和输出长度
    PBKDF2_HMAC_SHA512(unsigned int iterations = 10000, size_t output_len = 64) : iterations_(iterations), output_len_(output_len) {}

    // 生成派生密钥
    std::string derive_key(const std::string &password, const std::vector<uint8_t> &salt) const {
        // 输出密钥的容器
        std::vector<uint8_t> output(output_len_);

        // 创建并初始化 HMAC 上下文
        mbedtls_md_context_t ctx;
        mbedtls_md_init(&ctx);

        // 获取 SHA512 哈希算法信息
        const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA512);
        if (md_info == nullptr) {
            mbedtls_md_free(&ctx);  // 清理上下文
            WARN(WarningLevel::ERROR, "无法获取 SHA512 哈希算法信息");
            // throw std::runtime_error("无法获取 SHA512 哈希算法信息");
        }

        // 设置 HMAC 上下文，启用 HMAC-SHA512
        if (mbedtls_md_setup(&ctx, md_info, 1) != 0) {
            mbedtls_md_free(&ctx);  // 清理上下文
            WARN(WarningLevel::ERROR, "HMAC 上下文设置失败");
            // throw std::runtime_error("HMAC 上下文设置失败");
        }

        // 执行 PBKDF2 密钥派生
        int ret = mbedtls_pkcs5_pbkdf2_hmac(&ctx, reinterpret_cast<const unsigned char *>(password.data()), password.size(), salt.data(), salt.size(),
                                            iterations_, output_len_, output.data());

        // 清理上下文
        mbedtls_md_free(&ctx);

        if (ret != 0) {
            WARN(WarningLevel::ERROR, "PBKDF2 密钥派生失败, errCode: %d", ret);
            // throw std::runtime_error("PBKDF2 密钥派生失败，错误代码: " + std::to_string(ret));
        }

        // 将输出转换为十六进制字符串
        return bytes_to_hex(output);
    }

    // 静态函数，用于打印十六进制数据
    static void print_hex(const std::vector<uint8_t> &data) {
        for (uint8_t byte : data) {
            printf("%x", byte);
        }
        printf("\n");
    }

   private:
    unsigned int iterations_;  // PBKDF2 迭代次数
    size_t output_len_;        // 输出密钥的长度

    // 辅助函数：将字节数组转换为十六进制字符串
    static std::string bytes_to_hex(const std::vector<uint8_t> &data) {
        std::string hex_output;
        hex_output.reserve(data.size() * 2);  // 预先分配内存，避免重复分配

        for (uint8_t byte : data) {
            char buf[3];
            snprintf(buf, sizeof(buf), "%02x", byte);  // 转换为两位十六进制
            hex_output.append(buf);                    // 拼接到结果字符串
        }

        return hex_output;
    }
};

/*
void setup() {
    Serial.begin(115200);  // 初始化串口通信
    delay(4000);

    std::string password = "mysecurepassword";                                     // 用户密码
    std::vector<uint8_t> salt = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};  // 盐（随机生成）
    unsigned int iterations = 10000;                                               // 迭代次数，建议至少 10,000
    size_t output_len = 64;                                                        // 输出密钥长度（64 字节 = 512 位）

    // 创建 PBKDF2 对象，指定迭代次数和输出长度
    PBKDF2_HMAC_SHA512 pbkdf2(iterations, output_len);

    // 打印密码和盐
    Serial.println(("密码: " + password).c_str());
    Serial.print("盐: ");
    PBKDF2_HMAC_SHA512::print_hex(salt);

    // 生成并打印派生密钥
    std::string derived_key = pbkdf2.derive_key(password, salt);
    Serial.print("派生密钥 (PBKDF2-HMAC-SHA512): ");
    Serial.println(derived_key.c_str());
}
*/

