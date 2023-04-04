#pragma once
#include <array>
#include <cstring>
#include <iomanip>
#include <vector>

class RivestCipher4 {
   public:
    // 构造函数，接收一个字符串 key 作为参数
    explicit RivestCipher4(const std::string &key) {
        // 初始化 S 盒
        for (uint16_t i = 0; i < 256; ++i) S_box_[i] = static_cast<uint8_t>(i);

        // 对 S 盒进行置换
        uint32_t j = 0;
        for (uint16_t i = 0; i < 256; ++i) {
            // 用循环的方式扩展 key
            j = (j + S_box_[i] + key[i % key.size()]) & 0xFF;
            uint8_t tmp = S_box_[i];
            S_box_[i] = S_box_[j];
            S_box_[j] = tmp;
        }
    }

    // 加密函数，接收一个字符串 plaintext 作为参数
    std::string encrypt(const std::string &plaintext) {
        // 初始化一个输出字符串流
        std::ostringstream ciphertext_stream;
        // 使用十六进制输出，并用 0 填充未满两位的字节
        ciphertext_stream << std::hex << std::setfill('0');

        // 初始化两个状态变量 i 和 j
        uint8_t i = 0, j = 0;
        // 遍历明文字符串中的每个字节
        for (size_t index = 0; index < plaintext.size(); ++index) {
            // 更新状态变量 i 和 j
            i = (i + 1) & 0xFF;
            j = (j + S_box_[i]) & 0xFF;
            // 交换 S 盒中的两个元素
            std::swap(S_box_[i], S_box_[j]);
            // 计算 k 和 temp
            uint8_t t = (S_box_[i] + S_box_[j]) & 0xFF;
            uint8_t k = S_box_[t];
            uint8_t temp = static_cast<uint8_t>(plaintext[index]) ^ k;

            // 将加密后的字节写入输出流
            ciphertext_stream << std::setw(2) << static_cast<uint16_t>(temp);
        }

        // 返回加密后的字符串
        return ciphertext_stream.str();
    }

    // 解密函数，调用加密函数实现
    std::string decrypt(const std::string &ciphertext) { return encrypt(ciphertext); }

   private:
    // S 盒，使用 std::array 存储
    std::array<uint8_t, 256> S_box_;
};
