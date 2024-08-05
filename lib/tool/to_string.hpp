/**
 * @file to_string.hpp
 * @date 04.08.2024
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
 *
 * 功能: 用于将整型数值转换为字符串数据类型;
 */

#pragma once
#include <sstream>
#include <string>

template <typename T>
std::string to_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

/*使用示例
int main() {
    int intValue = 42;
    uint8_t uint8Value = 255;
    uint16_t uint16Value = 65535;

    std::string intStr = to_string(intValue);
    std::string uint8Str = to_string(uint8Value);
    std::string uint16Str = to_string(uint16Value);

    // 在ESP32上使用Serial.print()输出结果
    Serial.begin(115200);
    Serial.println(intStr.c_str());
    Serial.println(uint8Str.c_str());
    Serial.println(uint16Str.c_str());

    return 0;
}
*/
