# `USER_DATA`类

## 简介

`USER_DATA` 是一个用户数据管理类，主要用于管理用户的添加、删除和验证操作。通过结合 PBKDF2 哈希算法和真随机盐值（Salt）生成用户密码的派生密钥，不直接存储用户密码，确保用户数据的安全性。适用于需要存储和验证用户数据的嵌入式设备开发场景。

## 功能特点

- **用户添加**：支持用户名和密码的安全存储，使用盐值和 PBKDF2 算法生成派生密钥。
- **用户删除**：支持按用户名删除用户数据。
- **用户验证**：验证用户名和密码是否匹配，确保用户身份真实性。
- **用户数据打印**：提供串口调试接口，输出所有用户数据。

------

## 成员函数

### 1. 构造函数

```cpp
USER_DATA(uint32_t iterations = 10000, size_t salt_len = 64);
```

#### 参数

- `iterations`：PBKDF2 的迭代次数。次数越高，密码越安全，但处理速度也会越慢（默认值为 10000）。
- `salt_len`：程序自动生成真随机盐值的长度（默认值为 64 位）。



### 2. 添加用户

```cpp
void add_user(const std::string& username, const std::string& password);
```

- **功能**：添加新的用户。
- **注意**：若用户名已存在，将输出警告信息，操作不会执行。用户密码会经过 PBKDF2 哈希处理，并与随机生成的盐值绑定。

#### 参数

- `username`：用户名（字符串）。
- `password`：用户密码（字符串）。



### 3. 删除用户

```cpp
void delete_user(const std::string& username);
```

- **功能**：根据用户名删除用户数据。

- **注意**：若用户名不存在，将输出警告信息，操作不会执行。

#### 参数

  - `username`：要删除的用户名。



### 4. 验证用户

```cpp
bool verify_user(const std::string& username, const std::string& password);
```

- **功能**：验证用户名和密码是否匹配。

- **说明**: 验证时使用存储的盐值重新计算密钥，与数据库中的密钥进行比对

#### 返回值
  - `true`：用户名和密码匹配。
  - `false`：用户名不存在或密码不正确。

#### 参数

- `username`：用户名。
- `password`：待验证的密码。



### 5. 打印用户数据

```cpp
void print();
```

- **功能**：通过串口输出用户表的所有数据（用户名、派生密钥和盐值）。

------

## 注意事项

- **密码安全**：`USER_DATA` 不直接存储用户密码，而是存储经过 PBKDF2 处理后的派生密钥。

- **盐值唯一性**：每个用户的盐值是随机生成的，防止相同密码的用户生成相同的密钥。

- **错误提示**：操作失败时会通过串口输出提示信息。


------

## 示例程序

以下是完整的使用示例：

```cpp
#include <user_data_table.hpp>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 使用默认参数初始化用户数据库
    USER_DATA user_data;

    // 添加用户
    user_data.add_user("Alice", "password123");
    user_data.add_user("Bob", "mypassword");

    // 打印用户数据
    user_data.print();

    // 验证用户
    if (user_data.verify_user("Alice", "password123")) {
        Serial.println("验证通过");
    } else {
        Serial.println("验证失败");
    }

    // 删除用户
    user_data.delete_user("Bob");
    user_data.print();
}
```

输出:

```cpp
Alice	025385f9978155e5d373b66a932931bfce5a0d4a1229d6c0c0762b61a67f2a2fe237d44a79f2383880ea22d50bb7653a8aebcb613e4c864924b24e4dabbfcc76	3053605554026932637105616202654360009403802055371006439614671701	
Bob	0d815255f76cd30afbdc6508361470fb78ce8634ad1e9b9327e9595f25856faa83b7d2d5d24643688953b28a21fb16ecc8aa3d90a31f860630f46e264ed1a88f	6200841157174501250409645310552451011452714575111378156027355224	
验证通过
			
Alice	025385f9978155e5d373b66a932931bfce5a0d4a1229d6c0c0762b61a67f2a2fe237d44a79f2383880ea22d50bb7653a8aebcb613e4c864924b24e4dabbfcc76	3053605554026932637105616202654360009403802055371006439614671701
```

