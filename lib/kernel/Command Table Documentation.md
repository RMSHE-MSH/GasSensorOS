# `COMMAND_TABLE` 类

## 简介

`COMMAND_TABLE` 类是用于管理和执行命令的工具类，它是 `GasSensorOS` 操作系统命令行接口（CLI）的核心组件。该类支持命令的动态添加、删除、验证、执行以及打印命令表等功能。通过它，用户可以高效地组织、管理和调用命令，极大地方便了操作系统的命令行交互和扩展。

## 功能概述

| 功能             | 说明                                                             |
| ---------------- | ---------------------------------------------------------------- |
| **添加命令**     | 允许将自定义命令及其标志、处理函数添加到命令表中。               |
| **删除命令**     | 允许删除已注册的命令。                                           |
| **验证命令**     | 检查命令及其标志是否合法，确保命令调用时标志正确有效。           |
| **执行命令**     | 根据命令名称和标志执行相应的处理函数，处理命令逻辑。             |
| **打印命令表**   | 以表格形式打印当前所有已注册的命令及其有效标志。                 |
| **内建命令支持** | 提供预定义好的内建命令（如 `help`, `osinfo` 等），简化常用操作。 |

---

## 类成员方法

| **方法名**                                                                                                                                                                      | **功能说明**                                                                      |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| `void add_cmd(const std::string& cmd_name, const std::vector<std::string>& flags, std::function<void(const std::vector<std::string>&, const std::vector<std::string>&)> func);` | 添加新的命令及其处理函数。如果命令已存在，打印警告并拒绝添加。                    |
| `void delete_cmd(const std::string& cmd_name);`                                                                                                                                 | 删除指定名称的命令。如果命令不存在，打印警告信息。                                |
| `bool verify_cmd(const std::string& cmd_name, const std::string& flag) const;`                                                                                                  | 验证命令及其标志是否合法。如果命令存在且标志有效，返回 `true`；否则返回 `false`。 |
| `void execute_cmd(const std::string& cmd_name, const std::vector<std::string>& flags, const std::vector<std::string>& parameters);`                                             | 执行指定命令。首先验证命令及其标志的合法性，如果合法，调用处理函数执行命令。      |
| `void print_commands_table();`                                                                                                                                                  | 以表格形式打印当前所有已注册命令及其有效标志。                                    |
| `void builtin_cmd();`                                                                                                                                                           | 将内建命令添加到命令表中。                                                        |

---

## 命令结构  

在 `COMMAND_TABLE` 类中，用户输入的命令必须是已在命令表中注册的命令，由以下三个部分组成：

| **组成部分** | **定义**                                         | **示例**           |
| ------------ | ------------------------------------------------ | ------------------ |
| **命令名称** | 必须提供且唯一的标识符，用于识别和执行对应命令。 | `help`、`osinfo`   |
| **命令标志** | 可选项，通常以短横线开头，用于调整命令执行行为。 | `-f`、`-s`         |
| **命令参数** | 可选项，用于为命令逻辑提供额外的具体数据。       | `param1`、`114514` |

### 命令的语法结构  

命令的完整语法结构如下：  

```
<命令名称> [命令标志...] [命令参数...]
```

| **元素**        | **说明**                               |
| --------------- | -------------------------------------- |
| `<命令名称>`    | 唯一标识符，必须提供。                 |
| `[命令标志...]` | 零个或多个标志，用于调整命令执行行为。 |
| `[命令参数...]` | 零个或多个参数，提供额外信息支持。     |

### 示例

- 示例 1：仅提供命令名称  
```plaintext
test1
```
- 示例 2：命令名称 + 标志  
```plaintext
test2 -f
```
- 示例 3：命令名称 + 标志 + 参数  
```plaintext
test3 -f param1
```
- 示例 4：命令名称 + 多标志 + 多参数  
```plaintext
test3 -f -s param1 param2
```
---

## 命令表结构

`COMMAND_TABLE` 类的核心是 **命令表**（`command_table`），它是一个以 **命令名称** 为键的键值对容器，值由 **命令标志** 和 **命令处理函数** 组成，用于管理和存储已注册的命令。

命令表的具体结构如下：
```cpp
std::unordered_map<std::string,
                       std::pair<std::vector<std::string>, std::function<void(const std::vector<std::string>&, const std::vector<std::string>&)>>>
        command_table;
```

#### 命令表字段说明
| 名称             | 字段       | 类型                                                                                    | 描述                                                          |
| :--------------- | :--------- | :-------------------------------------------------------------------------------------- | :------------------------------------------------------------ |
| **命令名称**     | `cmd_name` | `std::string`                                                                           | 命令的唯一标识符。                                            |
| **命令标志**     | `flags`    | `std::vector<std::string>`                                                              | 命令的有效标志，用户通过它们调整命令行为（例如 `-f`，`-s`）。 |
| **命令处理函数** | `func`     | `std::function<void(const std::vector<std::string>&, const std::vector<std::string>&)>` | 实际执行命令的回调函数，处理标志和参数。                      |

---

## 使用步骤

### 1. 创建 `COMMAND_TABLE` 对象

首先，创建一个 `COMMAND_TABLE` 对象来管理命令。

```cpp
COMMAND_TABLE cmd_table;
```

### 2. 添加命令

使用 `add_cmd` 方法将新命令添加到命令表中。每个命令需指定命令名称、可选的命令标志和处理该命令的函数。

- 添加没有标志的命令 `test1`：

```cpp
cmd_table.add_cmd("test1", {}, std::bind(&CMD_FUNC::test1, &cmd_func, std::placeholders::_1, std::placeholders::_2));
```

- 添加带有单个标志 `-f` 的命令 `test2`：

```cpp
cmd_table.add_cmd("test2", {"-f"}, std::bind(&CMD_FUNC::test2, &cmd_func, std::placeholders::_1, std::placeholders::_2));
```

- 添加带有多个标志 `-f` 和 `-s` 的命令 `test3`：

```cpp
cmd_table.add_cmd("test3", {"-f", "-s"}, std::bind(&CMD_FUNC::test3, &cmd_func, std::placeholders::_1, std::placeholders::_2));
```

### 3. 执行命令

使用 `execute_cmd` 方法执行指定的命令，并传入命令的标志和参数列表。执行命令时，`COMMAND_TABLE` 会验证命令名称和标志的合法性。如果合法，调用处理函数执行命令逻辑。

```cpp
std::vector<std::string> flags = {"-f", "-s"};
std::vector<std::string> params = {"param1", "param2"};
cmd_table.execute_cmd("test3", flags, params);  // 执行 test3 命令
```

### • 初始化内建命令

内建命令（如 `help`、`osinfo`、`reboot` 等）是系统预定义好的命令，开箱即用。通过调用 `builtin_cmd` 方法，内建命令会自动添加到命令表中。

```cpp
cmd_table.builtin_cmd();
```

### • 删除命令

使用 `delete_cmd` 方法删除已注册的命令，只需提供命令名称即可删除对应命令。

```cpp
cmd_table.delete_cmd("test3");
```

### • 打印命令表

使用 `print_commands_table` 方法打印当前所有已注册命令及其有效标志，以表格形式展示。

```cpp
cmd_table.print_commands_table();
```

---

## 使用示例

以下示例展示了如何使用 `COMMAND_TABLE` 类来管理一个简单的命令行系统，包括一些自定义命令和内建命令。示例涵盖了创建命令表、添加命令（包括带标志和参数的命令）、执行命令，以及如何管理命令表的内容。

### 示例 1: 使用内建命令

```cpp
#include <command_table.hpp>

void setup() {
    Serial.begin(115200);
    delay(4000); 

    // 创建一个 COMMAND_TABLE 对象
    COMMAND_TABLE cmd_table;

    // 初始化内建命令（如 'help', 'osinfo', 'test'）
    cmd_table.builtin_cmd();

    // 打印当前命令表内容
    cmd_table.print_commands_table();

    // 执行 'test' 命令，不带标志和参数
    cmd_table.execute_cmd("test", {}, {});

    // 执行 'test' 命令，带有标志 '-f'，不带参数
    cmd_table.execute_cmd("test", {"-f"}, {});

    // 执行 'test' 命令，带有标志 '-s' 和参数 '114514'
    cmd_table.execute_cmd("test", {"-s"}, {"114514"});

    // 执行 'test' 命令，带有标志 '-f' 和 '-s'，参数 '114514' 和 'parameter2'
    cmd_table.execute_cmd("test", {"-f", "-s"}, {"114514", "parameter2"});
}

void loop() {}
```
输出：

```yaml
============================= Command Table =============================
| Command Name | Flags  |
| ------------ | ------ |
| test         | -f, -s |
| help         | None   |
| osinfo       | None   |
=========================================================================
test
test -f
test -s 114514
test -f -s 114514 parameter2
```
### 示例 2: 创建并执行无标志和参数的自定义命令

以下示例展示了如何通过 `COMMAND_TABLE` 类创建一个自定义命令 `hello_world`，该命令无标志和参数，执行时会输出简单的问候信息。

```cpp
#include <command_table.hpp> 

// 定义命令函数类
class COMMAND_FUNCS {
   public:
    // 定义 hello_world 命令的处理函数
    void hello_world(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        Serial.println("Hello World!");  // 输出问候信息
    }
};

void setup() {
    Serial.begin(115200);
    delay(4000); 

    // 创建 COMMAND_TABLE 对象
    COMMAND_TABLE cmd_table;

    // 创建命令处理类对象
    COMMAND_FUNCS cmd_funcs;

    // 注册自定义命令 hello_world，无标志和参数
    cmd_table.add_cmd(
        "hello_world", // 定义命令名称
        {},  // 空标志列表，表示此命令不支持任何标志
        std::bind(&COMMAND_FUNCS::hello_world, &cmd_funcs, std::placeholders::_1, std::placeholders::_2)
    );

    // 打印当前命令表
    cmd_table.print_commands_table();

    // 执行 hello_world 命令
    cmd_table.execute_cmd("hello_world", {}, {});
}

void loop() {}
```
输出：

```yaml
============================= Command Table =============================
| Command Name | Flags |
| ------------ | ----- |
| hello_world  | None  |
=========================================================================
Hello World!
```
### 示例 3: 创建并执行有标志和参数的自定义命令

在此示例中，我们定义了一个名为 `test` 的命令，支持两个标志（`-f` 和 `-s`），并允许传递参数。命令根据标志和参数的组合，动态生成输出内容。

```cpp
#include <command_table.hpp> 

// 定义命令函数类
class COMMAND_FUNCS {
   public:
    // 定义 test 命令的处理函数(将在这个自定义函数中处理命令标志和命令参数)
    void test(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        bool flag_f = false;
        bool flag_s = false;
        for (const auto& flag : flags) {
            if (flag == "-f") 
                flag_f = true;
            else if (flag == "-s") 
                flag_s = true;
        }
        std::string output = "test";
        if (flag_f) output += " -f";
        if (flag_s) output += " -s";
        for (const auto& param : parameters) 
            output += " " + param;
        Serial.println(output.c_str());
    }
};

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 创建 COMMAND_TABLE 对象
    COMMAND_TABLE cmd_table;

    // 创建命令处理类对象
    COMMAND_FUNCS cmd_funcs;

    // 注册自定义命令 'test'，支持标志 '-f' 和 '-s'
    cmd_table.add_cmd(
        "test", // 定义命令名称
        {"-f", "-s"},  // 定义支持的标志列表
        std::bind(&COMMAND_FUNCS::test, &cmd_funcs, std::placeholders::_1, std::placeholders::_2)
    );

    // 打印当前命令表
    cmd_table.print_commands_table();

    // 执行各种组合的 'test' 命令
    cmd_table.execute_cmd("test", {}, {});                          // 无标志和参数
    cmd_table.execute_cmd("test", {"-f"}, {});                      // 带 '-f' 标志，无参数
    cmd_table.execute_cmd("test", {"-s"}, {"114514"});              // 带 '-s' 标志，参数为 '114514'
    cmd_table.execute_cmd("test", {"-f", "-s"}, {"114514", "parameter2"});  // 带 '-f' 和 '-s' 标志，多参数
}

void loop() {}
```
输出：

```yaml
============================= Command Table =============================
| Command Name | Flags  |
| ------------ | ------ |
| test         | -f, -s |
=========================================================================
test
test -f
test -s 114514
test -f -s 114514 parameter2
```
### 示例 4: 删除命令

以下示例展示了如何使用 `COMMAND_TABLE` 类的 `delete_cmd` 方法，从命令表中删除指定的命令。通过该示例可以了解命令删除后无法执行的行为，以及命令表的实时更新。

```cpp
#include <command_table.hpp> 

// 定义命令处理函数类
class COMMAND_FUNCS {
   public:
    // 命令 "test1" 的处理函数
    void test1(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        Serial.println("test1");
    }
    // 命令 "test2" 的处理函数
    void test2(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        Serial.println("test2");
    }
    // 命令 "test3" 的处理函数
    void test3(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        Serial.println("test3");
    }
};

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 创建命令表实例
    COMMAND_TABLE cmd_table;

    // 创建命令处理函数实例
    COMMAND_FUNCS cmd_funcs;

    // 向命令表添加三个命令：test1, test2, test3
    cmd_table.add_cmd("test1", {}, std::bind(&COMMAND_FUNCS::test1, &cmd_funcs, std::placeholders::_1, std::placeholders::_2));
    cmd_table.add_cmd("test2", {}, std::bind(&COMMAND_FUNCS::test2, &cmd_funcs, std::placeholders::_1, std::placeholders::_2));
    cmd_table.add_cmd("test3", {}, std::bind(&COMMAND_FUNCS::test3, &cmd_funcs, std::placeholders::_1, std::placeholders::_2));

    // 打印当前命令表内容
    cmd_table.print_commands_table();

    // 执行命令 "test2"
    cmd_table.execute_cmd("test2", {}, {});

    // 删除命令 "test2"
    cmd_table.delete_cmd("test2");

    // 再次打印命令表，验证命令 "test2" 已被移除
    cmd_table.print_commands_table();

    // 尝试执行已删除的命令 "test2"
    cmd_table.execute_cmd("test2", {}, {});
}

void loop() {}
```

输出:

```yaml
============================= Command Table =============================
| Command Name | Flags |
| ------------ | ----- |
| test3        | None  |
| test1        | None  |
| test2        | None  |
=========================================================================
test2

============================= Command Table =============================
| Command Name | Flags |
| ------------ | ----- |
| test3        | None  |
| test1        | None  |
=========================================================================
lib/kernel/command_table.hpp:129: WARNING: 未找到匹配的命令名称: test2
```

---
## 注意事项

1. **修改内建命令**:
   - 修改内建命令需要直接编辑 `command_table.hpp` 文件中的源代码。
   - 增删内建命令时，应更新 `CMD_FUNC` 类，在其中添加或移除相应的命令处理函数。
   - 同时，需要在 `COMMAND_TABLE` 类的 `builtin_cmd` 方法中注册新增的内建命令。
   - 总之方法与添加自定义命令相似。

2. **与命令行接口的区别**:
   - 如果您的需求并未涉及操作系统的命令行功能，则无需直接使用或修改 `COMMAND_TABLE` 类。
   - 此类仅实现命令行的核心功能（如命令注册、执行和验证），不提供具体的用户接口。
   - 用户界面相关功能已封装在 `command_line_interface` 文件中，建议直接使用该文件以更便捷地构建完整的命令行交互系统。

---
