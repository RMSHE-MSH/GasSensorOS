# `StringSplitter` 类

## 简介

`StringSplitter` 类提供了高级字符串分割功能，允许用户根据自定义配置选项来分割字符串。通过链式调用，用户可以灵活地配置这些选项，然后使用指定的分隔符(字符串)列表对字符串进行分割，并获取分割后的字符串集合。

## 功能特性

- **多分隔符支持**：允许使用多个分隔符(字符串)分割字符串。
- **高级分割功能**：支持`忽略空字符串`、`修剪空白字符`、`限制分割次数`、`忽略大小写`、`保留分隔符`这些高级分割配置。
- **链式配置**：灵活配置分割行为。

## 分割配置

| 名称         | 配置选项         | 类型     | 默认值  | 说明                                                         |
| ------------ | ---------------- | -------- | ------- | ------------------------------------------------------------ |
| 忽略空字符串 | `ignoreEmpty`    | `bool`   | `true`  | 是否忽略空字符串。如果为 `true`，空字符串将不会包含在分割结果中。 |
| 修剪空白字符 | `trimWhitespace` | `bool`   | `true`  | 是否修剪字符串两端的空白字符。如果为 `true`，每个分割后的字符串将自动修剪两端的空白字符。 |
| 限制分割次数 | `maxSplits`      | `size_t` | `0`     | 最大分割次数。`0` 表示不限制，即会分割到字符串末尾。         |
| 忽略大小写   | `ignoreCase`     | `bool`   | `false` | 是否忽略大小写。如果为 `true`，则不区分分隔符的大小写。      |
| 保留分隔符   | `keepSeparators` | `bool`   | `false` | 是否保留分隔符。如果为 `true`，分割后的结果中将保留分隔符。  |

---

## 快速开始

1. **创建实例**：

   ```c++
   StringSplitter splitter;
   ```

2. **配置选项**（可选）：

   ```c++
   splitter.ignoreEmpty(true)
           .trimWhitespace(true)
           .maxSplits(3)
           .ignoreCase(false)
           .keepSeparators(false);
   ```

3. **分割字符串**：

   ```c++
   std::vector<std::string> result = splitter.split("hello,world:split.", {",",":"});
   ```
4. **打印分割结果**：

    ```c++
    for (const auto& token : result) {
            Serial.print("[");
            Serial.print(token.c_str());
            Serial.print("]");
    }
    Serial.print("\n");
    ```
5. **Output**:

    ```yaml
    [hello][world][split.]
    ```


---

## 方法说明

### 构造函数

- `StringSplitter()`：初始化一个 `StringSplitter` 实例，使用默认配置。

### 分割字符串

- `std::vector<std::string> split(const std::string& str, const std::vector<std::string>& delimiters) const`：根据提供的分隔符列表分割字符串，并返回分割后的字符串向量。

### 链式配置方法

- `StringSplitter& ignoreEmpty(bool ignore)`：设置是否忽略空字符串。
- `StringSplitter& trimWhitespace(bool trim)`：设置是否修剪字符串两端的空白字符。
- `StringSplitter& maxSplits(size_t maxSplits)`：设置最大分割次数。
- `StringSplitter& ignoreCase(bool ignore)`：设置是否忽略大小写。
- `StringSplitter& keepSeparators(bool keep)`：设置是否保留分隔符。

### 恢复默认配置

- `void resetOptions()`：将分割配置恢复为默认设置。

---
## 测试代码

```c++
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
```
### Output:

```yaml
Test 1: Default Configuration
[hello][world][split]
Test 2: Not Ignore Empty
[hello][world][][split]
Test 2.1: Not Ignore Trim Whitespace
[  hello  ][  world  ][  ][  split  ]
Test 3: Max Splits
[one][two][three][four,five]
Test 4: Ignore Case
[AB][D][FGab][d][fg]
Test 5: Keep Separators
[one][,][two][,][three][,][four]
Test 6: Multiple Delimiters
[one,two][thre][our][five]
Test 7: Consecutive Delimiters
[one][two][three]
Test 8: Leading and Trailing Delimiters
[one][two][three]
Test 9
[How,][are.][you][with,][everything?]
```

---
