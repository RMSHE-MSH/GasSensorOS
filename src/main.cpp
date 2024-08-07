/**
 * @file main.cpp
 * @date 26.02.2023
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2023 RMSHE. All rights reserved.
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

#include <CPPSTL.h>
#include <esp32_pin_defi.h>
#include <io.h>
#include <kernel_tasks.h>
#include <random.h>
#include <systime.h>

#include <random>

// #include <code_interpreter.hpp>
#include <data_table.hpp>
#include <fourier_transform.hpp>
#include <rivest_cipher_4.hpp>
#include <send_request.hpp>
#include <string_edit.hpp>
#include <tree.hpp>
#include <wifi_connector.hpp>

GPIOs IO;

void vectorPrint(std::vector<std::string> result) {
    for (const auto& token : result) {
        Serial.print("[");
        Serial.print(token.c_str());
        Serial.print("]");
    }
    Serial.print("\n");
}

// 测试函数
void runTests() {
    StringSplitter splitter;

    // 测试1: 默认配置
    Serial.println("Test 1: Default Configuration");
    auto result = splitter.split("  hello  ,  world  ,  ,  split  ", {","});
    vectorPrint(result);
    splitter.resetOptions();

    // 测试2: 不忽略空字符串
    Serial.println("Test 2: Not Ignore Empty");
    splitter.ignoreEmpty(false);
    result = splitter.split("  hello  ,  world  ,  ,  split  ", {","});
    vectorPrint(result);
    splitter.resetOptions();

    // 测试2.1: 不修剪空白字符
    Serial.println("Test 2.1: Not Ignore Trim Whitespace");
    splitter.trimWhitespace(false);
    result = splitter.split("  hello  ,  world  ,  ,  split  ", {","});
    vectorPrint(result);
    splitter.resetOptions();

    // 测试3: 最大分割次数限制
    Serial.println("Test 3: Max Splits");
    splitter.maxSplits(3);
    result = splitter.split("one,two,three,four,five", {","});
    vectorPrint(result);
    splitter.resetOptions();

    // 测试4: 忽略大小写
    Serial.println("Test 4: Ignore Case");
    splitter.ignoreCase(true);
    result = splitter.split("ABCDEFGabcdefg", {"c", "E"});
    vectorPrint(result);
    splitter.resetOptions();

    // 测试5: 保留分隔符
    Serial.println("Test 5: Keep Separators");
    splitter.keepSeparators(true);
    result = splitter.split("one,two,three,four", {","});
    vectorPrint(result);
    splitter.resetOptions();

    // 测试6: 复杂用例，多个分隔符
    Serial.println("Test 6: Multiple Delimiters");
    std::vector<std::string> delimiters = {";", "e,f"};
    result = splitter.split("one,two;three,four;five", delimiters);
    vectorPrint(result);
    splitter.resetOptions();

    // 测试7: 连续分隔符
    Serial.println("Test 7: Consecutive Delimiters");
    result = splitter.split("one,,two,,,three", {","});
    vectorPrint(result);
    splitter.resetOptions();

    // 测试8: 开头和结尾的分隔符
    Serial.println("Test 8: Leading and Trailing Delimiters");
    result = splitter.split(",one,two,three,", {","});
    vectorPrint(result);
    splitter.resetOptions();

    Serial.println("Test 9");
    splitter.ignoreEmpty(true).trimWhitespace(true).maxSplits(0).ignoreCase(false).keepSeparators(false);
    result = splitter.split("How, are. you getting. along with, everything?", {" ", "getting. along"});
    vectorPrint(result);
}

void setup() {
    Serial.begin(115200);
    IO.gpioInit(4, GPIO_OUT_PP);
    IO.gpioResetBit(4);
}

void loop() {
    runTests();
    IO.gpioToggleBit(4);
    delay(3000);
}