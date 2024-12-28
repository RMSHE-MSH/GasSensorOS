# **GasSensor `OperatingSystem`**

## 📋简介

`GasSensorOS` 是基于 `ESP32` 系列模组开发的嵌入式操作系统。项目包含多个模块，如数据结构容器（包括数据表和树形结构）、OLED 显示驱动、真随机数生成与加密算法、内核任务管理、I/O 操作、傅里叶变换工具，以及网络服务器等功能。项目使用 `C/C++` 语言编写，强调模块化、通用化设计和硬件抽象，便于开发高效的物联网应用。

在开发的过程中，项目将优先使用 `C++ STL`，并充分利用微控制器的计算性能，以构建通用的功能组件，而非依赖于特定硬件设计。这样使得组件具有跨平台通用性，能够同时适用于不同架构的 CPU，但可能在性能上不如基于特定硬件实现的组件。

**项目目前仍处于早期开发阶段**，并以 AGPL-3.0 开源协议发布。



![GasSensorOS](/OS_info/GasSensorOS.png)

---

## 🔄更新记录
### ✦ 功能更新情况

| 更新日期   | 版本号                 | 主要更新内容                                                 |
| :--------- | :--------------------- | :----------------------------------------------------------- |
| 2024.12.28 | Alpha.Mark3.2024.12.28 | 1. `pbkdf2_hmac_sha512.hpp` 用于从密码和盐中生成高安全性的派生密钥<br/>2. `user_data_table.hpp` 高安全性的多用户数据管理类，主要用于管理用户的添加、删除和验证操作。 |
| 2024.09.01 | Alpha.Mark3.2024.09.01 | 1. 原生适配的 SOC 更换为`ESP32-S3-WROOM-1`;<br/>2. 大规模重构外设相关的代码, 引入了大量新特性, 极大简化了 GPIO 的控制方式;<br/>3. 增加串口代码调试工具: `Serial Warning` 是一个轻量级的串口调试和异常处理工具，为 ESP32 嵌入式开发环境设计。该工具通过自动定位错误代码的源文件路径和行号，帮助开发者迅速准确地定位和修复问题。同时, 支持多级别警告，允许根据严重程度筛选并输出有价值的调试信息。 |
| 2024.08.08 | Alpha.Mark2.2024.08.08 | `string_similarity_evaluator` 用于评估两个密码的相似度, `string_strength_evaluator` 用于评估一个密码的强度. |
| 2024.08.07 | Alpha.Mark2.2024.08.06 | `StringSplitter` 类提供了高级字符串分割功能，允许用户根据自定义配置选项来分割字符串。通过链式调用，用户可以灵活地配置这些选项，然后使用指定的分隔符(字符串)列表对字符串进行分割，并获取分割后的字符串集合。 |
| 2024.08.05 | Alpha.Mark2.2024.08.05 | `DataTable` 类提供了一种简单而高效的方式来管理二维数据表格，适用于嵌入式系统中的数据处理需求。该类提供了基本的表格操作功能, 支持动态调整表格的尺寸、插入、删除、查询和替换数据等基本操作。 |
| ……         | ……                     | `first publish`                                              |

### ✦ BUG修复情况

| 编号 | 发现时间   | 位置                                                | 症状                                              | 链接 | 原因                                                         | 修复情况  |
| ---- | ---------- | --------------------------------------------------- | ------------------------------------------------- | ---- | ------------------------------------------------------------ | --------- |
| `02` | 2024.08.25 | `io_esp32.cpp`, `io_esp32_s3.cpp`                   | `ADC`, `PWM`和`中断`等外设控制目前不可用.         |      | 本项目将原生适配芯片更换为`ESP32-S3`, 正在大规模重构外设相关的代码库. | 正在重构🕒 |
| `01` | 2024.08.08 | `io.cpp` - `void GPIOs::gpioToggleBit(uint8_t pin)` | 该函数无法将指定输出引脚的电平反转(原本是可以的). |      | 将 `PlatformIO Espressif 32` 更新到 `6.8.1` 版本后出现此问题. | 正在处理🕒 |

---
## 🛠硬件适配情况

- **完全适配:** `ESP32-S3-WROOM-1`,`ESP32-WROOM-32E`, `ESP32-WROOM-32D`
- **经测试部分适配:**  `ESP-12F`, `ESP-01`
- **未测试部分适配:** `ESP8266EX`

---

## 🧩主要组件

> 这里仅列出目前开发得较完整和成熟的组件.

### `containers` 容器

| 名称                | 版本号     | 简介                                                                                                                                                                                                                                                                                                                                                     | 说明文档                                                                                                  |
| ------------------- | :--------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| `data_table.hpp`    | 05.08.2024 | `DataTable` 类提供了一种简单而高效的方式来管理二维数据表格，适用于嵌入式系统中的数据处理需求。该类提供了基本的表格操作功能, 支持动态调整表格的尺寸、插入、删除、查询和替换数据等基本操作。                                                                                                                                                               | [Data Table Documentation](/lib/containers/Data%20Table%20Documentation.md)                               |
| `tree.hpp`          | 26.02.2023 | 轻量级的通用树数据结构容器(模板库)。专门用于创建和管理树形数据结构。该库提供了一种灵活且高效的方式来处理层次结构数据，支持使用向量、多重集和映射等不同的存储模型。该库支持诸如获取树的深度、访问父节点和子节点以及管理子树大小等操作。它经过性能优化，大多数操作都能在对数时间内完成，适用于需要树形结构的场景，如组织层次结构数据或管理实体之间的关系。 | [Tree Structure Container Documentation](/lib/containers/Tree%20Structure%20Container%20Documentation.md) |
| `forward_queue.hpp` | 26.02.2023 | 双端队列                                                                                                                                                                                                                                                                                                                                                 |                                                                                                           |

### `encrypt` 安全组件

| 名称                              | 版本号     | 简介                                                         | 说明文档                                                     |
| --------------------------------- | :--------- | :----------------------------------------------------------- | ------------------------------------------------------------ |
| `pbkdf2_hmac_sha512.hpp`          | 26.12.2024 | PBKDF2_HMAC_SHA512 是一个基于 PBKDF2 和 HMAC-SHA512 算法设计的密钥派生类，专用于从密码和盐中生成高安全性的派生密钥. | [PBKDF2 HMAC SHA512 Documentation](/lib/encrypt/PBKDF2%20HMAC%20SHA512%20Documentation.md) |
| `random.cpp`                      | 26.02.2023 | 生成真随机数                                                 |                                                              |
| `rivest_cipher_4.hpp`             | 05.04.2023 | RivestCipher4加密算法                                        |                                                              |
| `string_similarity_evaluator.cpp` | 29.03.2024 | 用于评估两个字符串(密码)的相似度                             |                                                              |
| `string_strength_evaluator.cpp`   | 29.03.2024 | 用于评价一个字符串(密码)的强度, 其中得分越高则密码越安全; <br/>`长度评价`：字符串长度是最基本的复杂度指标之一。<br/>`字符多样性`：分析字符串中不同字符的种类，包括小写字母、大写字母、数字、特殊字符等。<br/>`重复性检查`：检测字符串中相同字符或子串重复出现的频率，频繁重复可能降低密码的复杂度。<br/>`熵的计算`：熵可以量化字符串的不可预测性和随机性。 |                                                              |

### `kerel` 内核组件

| 名称          | 版本号     | 简介                                                                                        | 说明文档 |
| ------------- | :--------- | :------------------------------------------------------------------------------------------ | -------- |
| `io.cpp`      | 26.02.2023 | I/O控制库。主要功能包括ADC（模数转换）、GPIO（通用输入输出）管理，以及PWM（脉宽调制）控制。 |          |
| `systime.cpp` | 26.02.2023 | 简单的系统时间管理功能，主要用于获取和更新系统的当前时间。                                  |          |

### `general_dsp` 通用数字信号处理组件
| 名称                    | 版本号     | 简介                                                                                                                                                                                                 | 说明文档 |
| ----------------------- | :--------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------- |
| `fourier_transform.hpp` | 05.04.2023 | 实现快速傅里叶变换 (FFT) 和离散傅里叶变换 (DFT) 的功能。提供了用于处理和分析信号频域信息的工具，可以执行正变换和逆变换。通过这些工具，用户可以将时域数据转换为频域数据，并对频率成分进行分析或操作。 |          |

### `tool` 工具组件

| 名称                 | 版本号     | 简介                                                         | 说明文档                                                     |
| -------------------- | :--------- | :----------------------------------------------------------- | ------------------------------------------------------------ |
| `string_edit.hpp`    | 06.08.2024 | `StringSplitter` 类提供了高级字符串分割功能，允许用户根据自定义配置选项来分割字符串。通过链式调用，用户可以灵活地配置这些选项，然后使用指定的分隔符(字符串)列表对字符串进行分割，并获取分割后的字符串集合。 | [String Edit Documentation](/lib/tool/String%20Edit%20Documentation.md) |
| `serial_warning.hpp` | 16.08.2024 | `SerialWarning` 是一个轻量级的串口调试和异常处理工具。该工具通过自动定位错误代码的源文件路径和行号，帮助开发者迅速准确地定位和修复问题。同时, 支持多级别警告，允许根据严重程度筛选并输出有价值的调试信息。 | [Serial Warning Documentation](/lib/tool/Serial%20Warning%20Documentation.md) |

### `web_server` 网络组件

| 名称                 | 版本号     | 简介                                                                                                                                                                          | 说明文档 |
| -------------------- | :--------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------- |
| `wifi_connector.hpp` | 05.04.2023 | 基于Arduino平台的Wi-Fi连接管理类，用于简化Wi-Fi网络的连接过程。                                                                                                               |          |
| `send_request.hpp`   | 06.04.2023 | 用于实现登录功能的类。通过网络请求从服务器获取时间戳，用于加密用户的密码。然后，将加密后的密码和用户名通过HTTP POST请求发送到服务器进行身份验证，并处理服务器返回的响应结果。 |          |

### `sys_services` 系统服务组件
| 名称                 | 版本号     | 简介                                                                                                                                                                          | 说明文档 |
| -------------------- | :--------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------- |
| `user_data_table.hpp` | 28.12.2024 | 多用户数据管理类，主要用于管理用户的添加、删除和验证操作。通过结合 PBKDF2 哈希算法和真随机盐值（Salt）生成用户密码的派生密钥，不直接存储用户密码，确保用户数据的安全性。 | [User Data Table Documentation](/lib/sys_services/User%20Data%20Table%20Documentation.md) |

---
