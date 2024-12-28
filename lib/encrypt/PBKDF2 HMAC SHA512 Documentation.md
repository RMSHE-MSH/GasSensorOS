# `PBKDF2_HMAC_SHA512` 类

PBKDF2_HMAC_SHA512 是一个基于 PBKDF2 和 HMAC-SHA512 算法设计的密钥派生类，专用于从密码和盐中生成高安全性的派生密钥，广泛应用于密码学领域.

## 功能概述

- **支持 PBKDF2 密钥派生**: 通过提供密码和盐生成指定长度的派生密钥.
- **使用 HMAC-SHA512 算法**: 利用高级哈希算法增强抗碰撞能力和安全性.
- **十六进制输出**: 可以将派生密钥以十六进制字符串形式返回.
- **灵活的参数配置**: 支持自定义迭代次数和输出长度.

## 环境依赖

- **mbedTLS 库**: 此类依赖 mbedTLS 提供的 HMAC 和 PBKDF2 接口, 请确保在项目中正确配置和链接 mbedTLS.

---

## 使用方法

### 1. 初始化类

```cpp
#include <pbkdf2_hmac_sha512.hpp>

// 使用默认参数初始化
PBKDF2_HMAC_SHA512 pbkdf2;

// 自定义迭代次数和输出长度
PBKDF2_HMAC_SHA512 pbkdf2_custom(20000, 32);
```

### 2. 生成派生密钥

#### 示例代码

```cpp
#include <pbkdf2_hmac_sha512.hpp>

std::string password = "mysecurepassword";                                     // 用户密码
std::vector<uint8_t> salt = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};  // 盐
unsigned int iterations = 10000;                                               // 迭代次数
size_t output_len = 64;                                                        // 输出密钥长度(字节)

void setup() {
    Serial.begin(115200);  // 初始化串口通信
    delay(4000);

    // 创建 PBKDF2 对象, 指定迭代次数和输出长度
    PBKDF2_HMAC_SHA512 pbkdf2(iterations, output_len);

    // 生成派生密钥
    std::string derived_key = pbkdf2.derive_key(password, salt);
}
```

### 3. 打印十六进制数据

类中提供了静态方法用于打印字节数据的十六进制表示.

```cpp
std::vector<uint8_t> data = {1, 1, 4, 5, 1, 4};
PBKDF2_HMAC_SHA512::print_hex(data);
```

输出:
```yaml
114514
```

---

## 类接口说明

### 构造函数

```cpp
PBKDF2_HMAC_SHA512(unsigned int iterations = 10000, size_t output_len = 64);
```

- **`iterations`**: PBKDF2 的迭代次数, 默认值为 10000, 可根据安全需求调整.
- **`output_len`**: 生成的密钥长度(字节), 默认值为 64.

### 成员函数

```cpp
std::string derive_key(const std::string &password, const std::vector<uint8_t> &salt) const
```

- **功能**: 根据输入的密码和盐生成派生密钥.
- **参数**:
  - `password`: 原始密码(字符串).
  - `salt`: 用于密钥派生的盐(字节数组).
- **返回值**: 生成的派生密钥(十六进制字符串).

```cpp
static void print_hex(const std::vector<uint8_t> &data)
```

- **功能**: 以十六进制形式打印输入的字节数组.
- **参数**:
  - `data`: 待打印的字节数组.

---

### 注意事项

1. **安全性**: 建议使用至少 16 字节的高质量随机盐, 并设置较高的迭代次数(建议至少 10000 次).

2. **错误处理**: 若密钥派生失败, 串口中会输出错误信息.

---

## 示例

以下是一个完整的使用示例:

```cpp
#include <pbkdf2_hmac_sha512.hpp>

void setup() {
    Serial.begin(115200);
    delay(4000);

    std::string password = "mysecurepassword";                                     // 用户密码
    std::vector<uint8_t> salt = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};  // 盐
    unsigned int iterations = 10000;                                               // 迭代次数
    size_t output_len = 64;                                                        // 输出密钥长度

    // 创建 PBKDF2 对象, 指定迭代次数和输出长度
    PBKDF2_HMAC_SHA512 pbkdf2(iterations, output_len);

    // 打印密码和盐
    Serial.println(("密码: " + password).c_str());
    Serial.print("盐: ");
    pbkdf2.print_hex(salt);

    // 生成并打印派生密钥
    std::string derived_key = pbkdf2.derive_key(password, salt);
    Serial.print("派生密钥 (PBKDF2-HMAC-SHA512): ");
    Serial.println(derived_key.c_str());
}
```

输出:
```yaml
密码: mysecurepassword
盐: 123456789abcdef0
派生密钥 (PBKDF2-HMAC-SHA512): 1c9e952f105d9f4bc04391feda210776f58684d936ef0bcc84665c6623503a7f5e30a2af9da8385bb66c1036049319532808393f276998bf572178233ab2cc9a
```

