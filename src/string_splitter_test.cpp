#include <io.h>

#include <string_edit.hpp>

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

void setup() { Serial.begin(115200); }

void loop() {
    delay(3000);
    runTests();
}