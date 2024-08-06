/**
 * @file string_edit.hpp
 * @date 06.08.2024
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
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

/**
 * @brief 用于将整型数值转换为字符串数据类型
 * @param value 要转换数据类型的整型数据
 *
 * 使用示例:
 * int main() {
 *    int intValue = 42;
 *   uint8_t uint8Value = 255;
 *   uint16_t uint16Value = 65535;
 *
 *   std::string intStr = to_string(intValue);
 *    std::string uint8Str = to_string(uint8Value);
 *  std::string uint16Str = to_string(uint16Value);
 *
 *  // 在ESP32上使用Serial.print()输出结果
 *    Serial.begin(115200);
 * Serial.println(intStr.c_str());
 *   Serial.println(uint8Str.c_str());
 *  Serial.println(uint16Str.c_str());
 *
 *  return 0;
 *}
 */
template <typename T>
std::string to_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

class StringEdit {
   public:
    /**
     * @brief 删除字符串中的一个连续子串。
     *
     * @param original 原始字符串。
     * @param toRemove 要删除的子串。
     * @return std::string 删除指定子串后的字符串。
     */
    std::string removeSubstring(const std::string& original, const std::string& toRemove) {
        // 若原始字符串或要删除的子串为空，则直接返回原始字符串
        if (original.empty() || toRemove.empty()) return original;

        std::string result = original;       // 创建原始字符串的副本
        size_t pos = result.find(toRemove);  // 查找子串的起始位置

        // 如果找到子串，则删除之
        if (pos != std::string::npos) result.erase(pos, toRemove.length());

        return result;
    }

    // 对包含数字字符串的向量进行排序
    void sortNumericStrings(std::vector<std::string>& vector) {
        // 使用标准库中的sort函数进行排序
        // 自定义比较函数作为lambda表达式，将字符串转换为长整型后比较
        std::sort(vector.begin(), vector.end(), [](const std::string& a, const std::string& b) {
            // 将字符串转换为整型后比较
            return atoi(a.c_str()) < atoi(b.c_str());
        });
    }

    /**
     * @brief 将字符串根据指定的分隔符进行分割，并将结果存储在 vector 容器中.
     * @param input 输入字符串
     * @param separator 分隔符字符串
     * @return 包含分割后字符串的 vector
     */
    std::vector<std::string> split(const std::string& input, const std::string& separator) {
        std::vector<std::string> vecStr;

        // 提前为 vector 容器预留空间 (假设分割后至少有 2 个子字符串)
        vecStr.reserve(2);

        // 临时引用，用于遍历并分割字符串
        size_t startPos = 0;
        size_t splitIndex;

        // 循环查找分隔符，并将子字符串添加到 vecStr 容器中
        while ((splitIndex = input.find(separator, startPos)) != std::string::npos) {
            vecStr.emplace_back(input.substr(startPos, splitIndex - startPos));
            startPos = splitIndex + separator.length();
        }

        // 添加最后一个子字符串
        vecStr.emplace_back(input.substr(startPos));

        return vecStr;
    }
};

/**
 * @brief `StringSplitter`类提供了高级字符串分割功能，
 * 允许用户根据自定义配置选项来分割字符串。
 * 通过链式调用，用户可以灵活地配置这些选项，
 * 然后使用指定的分隔符(字符串)列表对字符串进行分割，
 * 并获取分割后的字符串集合。
 */
class StringSplitter {
   public:
    /**
     * @brief 分割选项结构体，包含所有分割配置项。
     * 该结构体定义了字符串分割时的各种配置选项，例如是否忽略空字符串、修剪空白字符、
     * 分割次数限制、是否忽略大小写、是否保留分隔符。
     */
    struct SplitOptions {
        bool ignoreEmpty;     // 是否忽略空字符串，如果为true则不会将空字符串作为分割结果的一部分
        bool trimWhitespace;  // 是否修剪字符串两端的空白字符，如果为true则修剪每个分割后的字符串
        size_t maxSplits;     // 最大分割次数，0表示不限制，即会分割到字符串末尾
        bool ignoreCase;      // 是否忽略大小写，如果为true则不区分分隔符的大小写
        bool keepSeparators;  // 是否保留分隔符，如果为true则在分割后的结果中保留分隔符

        /**
         * @brief 构造函数，初始化默认值。
         * 默认设置为：忽略空字符串，修剪空白字符，无分割次数限制，区分大小写，
         * 不保留分隔符。
         */
        constexpr SplitOptions() : ignoreEmpty(true), trimWhitespace(true), maxSplits(0), ignoreCase(false), keepSeparators(false) {}
    };

    /**
     * @brief 构造函数，使用默认配置初始化。
     */
    StringSplitter() : options_() {}

    /**
     * @brief 恢复默认配置。
     */
    void resetOptions() { options_ = SplitOptions(); }

    // 提供链式调用的方法，允许用户在创建对象时灵活配置选项
    StringSplitter& ignoreEmpty(bool ignore) {
        options_.ignoreEmpty = ignore;
        return *this;
    }

    StringSplitter& trimWhitespace(bool trim) {
        options_.trimWhitespace = trim;
        return *this;
    }

    StringSplitter& maxSplits(size_t maxSplits) {
        options_.maxSplits = maxSplits;
        return *this;
    }

    StringSplitter& ignoreCase(bool ignore) {
        options_.ignoreCase = ignore;
        return *this;
    }

    StringSplitter& keepSeparators(bool keep) {
        options_.keepSeparators = keep;
        return *this;
    }

    /**
     * @brief 根据分隔符分割字符串。
     * @param str 需要分割的字符串
     * @param delimiters 分隔符列表
     * @return 分割后的字符串向量
     */
    std::vector<std::string> split(const std::string& str, const std::vector<std::string>& delimiters) const {
        std::vector<std::string> result;  // 存储分割后的结果
        size_t splits = 0;                // 已分割的次数
        size_t pos = 0;                   // 当前处理位置

        // 当分割次数未达到最大限制并且未处理完字符串时继续循环
        while ((splits < options_.maxSplits || options_.maxSplits == 0) && pos < str.size()) {
            size_t minPos = std::string::npos;
            std::string currentDelimiter;

            // 找到最近的分隔符
            for (const auto& delimiter : delimiters) {
                size_t tempPos = findDelimiter(str, delimiter, pos);
                if (tempPos < minPos) {
                    minPos = tempPos;
                    currentDelimiter = delimiter;
                }
            }

            // 如果未找到分隔符，则退出循环
            if (minPos == std::string::npos) {
                break;
            }

            // 提取分割出的字符串
            std::string token = str.substr(pos, minPos - pos);
            processToken(result, token);  // 处理并存储分割出的字符串

            // 如果需要保留分隔符，则将其加入结果
            if (options_.keepSeparators) {
                result.push_back(currentDelimiter);
            }

            // 更新当前处理位置和已分割次数
            pos = minPos + currentDelimiter.length();
            splits++;
        }

        // 处理最后一个分割出的字符串
        std::string lastToken = str.substr(pos);
        processToken(result, lastToken);

        return result;
    }

   private:
    SplitOptions options_;  // 分割选项实例

    /**
     * @brief 处理分割出的字符串，根据选项进行修剪和过滤。
     * @param result 存储处理后的字符串向量
     * @param token 需要处理的字符串
     */
    void processToken(std::vector<std::string>& result, std::string& token) const {
        if (options_.trimWhitespace) {
            token = trim(token);  // 修剪字符串两端的空白字符
        }
        if (!options_.ignoreEmpty || !token.empty()) {
            result.push_back(token);  // 如果不忽略空字符串或token不为空，则加入结果
        }
    }

    /**
     * @brief 修剪字符串两端的空白字符。
     * @param str 需要修剪的字符串
     * @return 修剪后的字符串
     */
    std::string trim(const std::string& str) const {
        const char* whitespace = " \t\n\r\f\v";            // 定义空白字符
        size_t start = str.find_first_not_of(whitespace);  // 找到第一个非空白字符的位置
        size_t end = str.find_last_not_of(whitespace);     // 找到最后一个非空白字符的位置
        return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }

    /**
     * @brief 在字符串中查找分隔符，忽略大小写。
     * @param str 源字符串
     * @param delimiter 分隔符
     * @param pos 查找的起始位置
     * @return 分隔符位置
     */
    size_t findDelimiter(const std::string& str, const std::string& delimiter, size_t pos) const {
        if (options_.ignoreCase) {
            // 忽略大小写查找分隔符
            auto it = std::search(str.begin() + pos, str.end(), delimiter.begin(), delimiter.end(),
                                  [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); });
            return (it == str.end()) ? std::string::npos : std::distance(str.begin(), it);
        } else {
            return str.find(delimiter, pos);
        }
    }

    /**
     * @brief 将字符串转换为小写。
     * @param str 需要转换的小写字符串
     * @return 转换后的字符串
     */
    static std::string toLower(const std::string& str) {
        std::string lowerStr = str;                                                     // 创建字符串副本
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);  // 转换为小写
        return lowerStr;
    }
};
