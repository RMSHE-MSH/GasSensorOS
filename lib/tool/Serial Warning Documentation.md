# `SerialWarning`

## 简介

`SerialWarning` 是一个轻量级的串口调试和异常处理工具，为 ESP32 嵌入式开发环境设计。该工具通过自动定位错误代码的源文件路径和行号，帮助开发者迅速准确地定位和修复问题。同时, 支持多级别警告，允许根据严重程度筛选并输出有价值的调试信息。

## 功能概述

  - **自动定位**: 自动定位错误代码的源文件路径和行号。
  - **多级警告**: 提供 `DEBUG`、`INFO`、`WARNING`、`ERROR`、`CRITICAL` 五个警告等级，便于区分和处理不同类型的问题。
  - **输出过滤**: 根据设置的日志级别阈值，仅输出高于或等于该级别的警告信息，避免不必要的信息干扰。
  - **格式化输出**: 支持可变参数，允许使用灵活的格式化字符串输出详细的调试信息。 

---

## 使用指南

### 1. 包含头文件

首先，在您的代码中包含 `SerialWarning` 类的头文件。

```cpp
#include "serial_warning.hpp"
```

### 2. 设置日志级别阈值

通过修改 `debugSettings.logLevelThreshold`，您可以控制输出的警告信息等级(默认为输出全部级别的警告信息)。例如，如果只想输出 `WARNING` 及以上的警告信息：

```cpp
debugSettings.logLevelThreshold = WarningLevel::WARNING;
```

### 3. 使用 `WARN` 宏输出警告信息

您可以使用 `WARN` 宏在代码中的任意位置输出警告信息。该宏会自动记录当前的文件名和行号，并输出格式化后的警告信息。

```cpp
WARN(WarningLevel::ERROR, "发生了一个错误，错误代码: %d", 114514);
```

上例将输出以下格式的信息：

```
文件路径:行号: ERROR: 发生了一个错误，错误代码: 114514
```

### 4. 日志等级枚举

下表显示了每种警告等级的用途：

| 等级     | 用途                                    |
| -------- | --------------------------------------- |
| DEBUG    | 调试信息，最详细的日志级别              |
| INFO     | 普通信息，用于记录常规操作              |
| WARNING  | 警告信息，用于提示潜在问题              |
| ERROR    | 错误信息，记录程序中的错误              |
| CRITICAL | 严重错误，可能导致MCU复位, 需要立即关注 |

### 5. 示例

以下是一个完整的使用示例：

```cpp
#include <Arduino.h>
#include "serial_warning.hpp"

void setup() {
    // 初始化串口通信，设置波特率为115200
    Serial.begin(115200);

    delay(3000);

    // 设置日志级别阈值
    debugSettings.logLevelThreshold = WarningLevel::INFO;

    // 测试警告系统，以下为不同级别的日志信息示例
    WARN(WarningLevel::DEBUG, "这是一个调试信息");
    WARN(WarningLevel::INFO, "初始化完成");
    WARN(WarningLevel::WARNING, "可能存在问题");
    WARN(WarningLevel::ERROR, "发生了一个错误, errCode: %d", 114514);
    WARN(WarningLevel::CRITICAL, "严重错误");
}

void loop() {
    // 主循环
}

```

OUTPUT:

```yaml
src/main.cpp:15: INFO: 初始化完成
src/main.cpp:16: WARNING: 可能存在问题
src/main.cpp:17: ERROR: 发生了一个错误, errCode: 114514
src/main.cpp:18: CRITICAL: 严重错误
```

在上例中，您将根据 `debugSettings.logLevelThreshold` 的设置，在串口监视器中看到不同级别的警告信息输出。

## 注意事项

- `SerialWarning` 设计为在嵌入式环境中高效运行。缓冲区大小和日志级别阈值设置可能需要根据具体应用进行调整。
- 请确保串口初始化（如 `Serial.begin(115200);`）在输出警告信息之前完成。
