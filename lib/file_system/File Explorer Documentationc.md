# FileExplorer 类使用指南

## 📦概述
`FileExplorer` 是一个基于  LittleFS 文件系统的通用文件操作类，提供统一、高级的文件与目录管理接口，适用于嵌入式项目中的文件浏览、操作与搜索功能。

---

## ✅ 功能概览

- 文件操作：创建、读取、写入、复制、移动、删除
- 目录操作：创建、遍历、复制、删除（递归支持）
- 搜索功能：精确查找、模糊查找（支持相似度阈值）
- 树结构：构建文件树、输出目录结构
- 自动处理路径中的父目录
- 支持字符串和字节数据的读写

---

## 🚀快速开始

1. **初始化**  
   创建 `FileExplorer` 对象时自动挂载文件系统，析构时自动卸载。
   
   ```cpp
   FileExplorer file; // 默认初始化
   ```

---

## 核心功能

### 📂文件与目录操作
| 操作             | 方法                                                         | 参数说明                                 | 返回值            |
| ---------------- | ------------------------------------------------------------ | ---------------------------------------- | ----------------- |
| 创建文件         | `bool createFile(const std::string& filePath)`               | 文件的完整路径，包含文件名和扩展名。     | 成功返回 `true`   |
| 创建目录         | `bool createDir(const std::string& dirPath)`                 | 目录的完整路径                           | 成功返回 `true`   |
| 复制文件或目录   | `void copyPath(const std::string& sourcePath, const std::string& targetPath)` | 源文件或目录的路径, 目标文件或目录的路径 |                   |
| 移动文件或目录   | `void movePath(const std::string& sourcePath, const std::string& targetPath)` | 源文件或目录的路径, 目标文件或目录的路径 |                   |
| 重命名文件或目录 | `void renamePath(const std::string& path, const std::string& newName)` | 要重命名的文件或目录路径, 新名称         |                   |
| 删除文件或目录   | `void deletePath(const std::string& path)`                   | 要删除的文件或目录路径                   |                   |
| 检查路径是否存在 | `bool exists(const std::string& path)`                       | 文件或目录的路径                         | 存在则返回 `true` |

### 📝文件内容读写
| 操作       | 方法                                                         | 参数说明                                                    | 返回值                 |
| ---------- | ------------------------------------------------------------ | ----------------------------------------------------------- | ---------------------- |
| 读取文本   | `std::string readFileAsString(const std::string& filePath)`  | 文件路径                                                    | 文件内容的字符串表示   |
| 写入文本   | `bool writeFileAsString(const std::string& filePath, const std::string& data, const char* mode = "a")` | 文件路径, 要写入的字符串数据, 写入模式（默认为追加`"a"`）   | 写入成功则返回 `true`  |
| 读取字节流 | `std::vector<uint8_t> readFileAsBytes(const std::string& filePath)` | 文件路径                                                    | 文件内容的字节数组表示 |
| 写入字节流 | `bool writeFileAsBytes(const std::string& filePath, const std::vector<uint8_t>& data, const char* mode = "a")` | 文件路径, 要写入的字节数组数据, 写入模式（默认为追加`"a"`） | 写入成功则返回 `true`  |

#### 文件写入模式

| 模式 | 标志 | 说明               |
| ---- | ---- | ------------------ |
| 追加 | `a`  | 在文件末尾追加内容 |
| 覆写 | `w`  | 覆盖文件内容       |

### 🔍路径搜索与文件树

| 操作                           | 方法                                                         | 参数说明                                                     | 返回值                                    | 说明                                                         |
| ------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ----------------------------------------- | ------------------------------------------------------------ |
| 精确查找指定名称的文件或目录   | `std::vector<std::string> findPath(const std::string& targetName, const std::string& parentPath = "/")` | 目标文件或目录的名称, 起始路径 (默认为根路径"`/`")           | 返回所有匹配的完整路径列表                | 从指定的父路径开始，查找所有匹配目标名称的文件或目录，返回它们的完整路径。如果路径不存在，返回一个空的路径列表。 |
| 模糊查找指定名称的文件或目录   | `std::vector<std::string> searchPath(const std::string& targetName, const std::string& parentPath = "/", float similarityThreshold = 0.0f)` | 目标文件或目录的名称, 起始路径 (默认为根路径"`/`"), 相似度阈值（0.0~1.0, 默认为 0.0） | 返回相似度≥阈值的路径列表（按相似度降序） | 从指定的父路径开始，递归遍历文件树，基于相似度计算找出与目标名称符合相似性阈值的节点，并返回对应路径的列表。 |
| 获取指定目录中的所有子项       | `std::vector<std::string> listDir(const std::string& dirPath = "/")` | 目录路径                                                     | 返回子项名称列表                          |                                                              |
| 打印指定目录中的所有子项       | `void printDir(const std::string& dirPath = "/")`            | 目录路径                                                     |                                           | 在串口打印目录内容(实际上是调用 `listDir` 后进一步在串口输出结果而已) |
| 获取指定目录路径下的目录树结构 | `std::unique_ptr<Tree<std::string>> getTree(const std::string& dirPath = "/")` | 需要构建目录树的根路径，默认为根目录 "`/`"                   | 返回构建好的目录树                        |                                                              |
| 打印指定目录路径下的目录树结构 | `void printTree(const std::string& dirPath = "/")`           | 需要打印目录树的根路径，默认为根目录 "`/`"                   |                                           | 在串口打印目录树(实际上是调用 `getTree` 后进一步在串口输出结果而已) |

---

## 📂文件与目录操作示例

### 1. 创建文件
```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/RMSHE/hello_file.txt");

    // 通过串口打印文件树
    file.printTree();
}
```

串口输出:

```
├─/
│ └─RMSHE
│   └─hello_file.txt
```

### 2. 创建目录

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建目录
    file.createDir("/dir1");
    file.createDir("/dir1/dir1_1");

    // 通过串口打印文件树
    file.printTree();
}
```

串口输出:

```
├─/
│ └─dir1
│   └─dir1_1
```

### 3. 复制文件或目录

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建目录
    file.createDir("/dir1");
    // 在指定路径创建文件
    file.createFile("/dir2/hello_file.txt");

    // 通过串口打印文件树
    file.printTree();

    // 将 hello_file 复制到 dir1 目录
    file.copyPath("/dir2/hello_file.txt", "/dir1/hello_file.txt");
    // 将 dir1 目录复制到 dir2 目录
    file.copyPath("/dir1", "/dir2/dir1");

    // 通过串口打印文件树
    file.printTree();
}
```

串口输出:

```
├─/
│ ├─dir1
│ └─dir2
│   └─hello_file.txt

├─/
│ ├─dir1
│ │ └─hello_file.txt
│ └─dir2
│   ├─dir1
│   │ └─hello_file.txt
│   └─hello_file.txt
```

### 4. 移动文件或目录

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/dir1/file1.txt");
    file.createFile("/dir2/file2.txt");

    // 通过串口打印文件树
    file.printTree();

    // 将 file2 移动到 dir1 目录
    file.movePath("/dir2/file2.txt", "/dir1/file2.txt");
    // 将 dir1 目录移动到 dir2 目录
    file.movePath("/dir1", "/dir2/dir1");

    // 通过串口打印文件树
    file.printTree();
}
```

串口输出:

```
├─/
│ ├─dir1
│ │ └─file1.txt
│ └─dir2
│   └─file2.txt

├─/
│ └─dir2
│   └─dir1
│     ├─file1.txt
│     └─file2.txt
```

### 6. 重命名文件或目录

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/dir1/file1.txt");

    // 通过串口打印文件树
    file.printTree();

    // 将 file1.txt 重命名为 file2.md
    file.renamePath("/dir1/file1.txt", "file2.md");
    // 将 dir1 重命名为 dir2
    file.renamePath("/dir1", "dir2");

    // 通过串口打印文件树
    file.printTree();
}
```

串口输出:

```
├─/
│ └─dir1
│   └─file1.txt

├─/
│ └─dir2
│   └─file2.md
```

### 7. 删除文件或目录

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/dir1/file1.txt");

    file.printTree();  // 通过串口打印文件树

    // 删除文件 file1.txt
    file.deletePath("/dir1/file1.txt");

    file.printTree();  // 通过串口打印文件树

    // 删除目录 dir1
    file.deletePath("/dir1");

    file.printTree();  // 通过串口打印文件树
}
```

串口输出:

```
├─/
│ └─dir1
│   └─file1.txt

├─/
│ └─dir1

├─/
```

### 8. 检查文件或目录是否存在

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/dir1/file1.txt");

    file.printTree();  // 通过串口打印文件树

    Serial.println(file.exists("/dir1"));            // 判断 dir1 目录是否存在
    Serial.println(file.exists("/dir1/file1.txt"));  // 判断 file1 文件是否存在
    Serial.println(file.exists("/dir1/file2.txt"));  // 判断 file2 文件是否存在
}
```

串口输出:

```
├─/
│ └─dir1
│   └─file1.txt

1
1
0
```

---

## 📝文件内容读写示例

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/file1.txt");

    // 向 file1 写入文本内容(覆盖文件内容)
    file.writeFileAsString("/file1.txt", "hello world!\n", "w");

    // 向 file1 追加字节数据(文件末尾追加内容)
    std::vector<uint8_t> bytes_data = {0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x5C, 0x6E};  // 要写入的字节数据
    file.writeFileAsBytes("/file1.txt", bytes_data, "a");

    // 将文件以字符串的形式读出
    Serial.print("\n\n1.将文件以字符串的形式读出\n");
    std::string str_data = file.readFileAsString("/file1.txt");
    Serial.print(str_data.c_str());

    // 将文件以字节数据的形式读出
    Serial.print("\n\n2.将文件以字节数据的形式读出\n");
    bytes_data = file.readFileAsBytes("/file1.txt");
    for (auto& i : bytes_data) {
        Serial.print(i);
        Serial.print(", ");
    }

    // 将文件以字节数据的形式读出并在打印时转换为字符
    Serial.print("\n\n3.将文件以字节数据的形式读出并在打印时转换为字符\n");
    bytes_data = file.readFileAsBytes("/file1.txt");
    for (auto& i : bytes_data) Serial.print((char)i);
}
```

串口输出:

```
1.将文件以字符串的形式读出
hello world!
hello world!\n

2.将文件以字节数据的形式读出
104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 33, 10, 104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 33, 92, 110, 

3.将文件以字节数据的形式读出并在打印时转换为字符
hello world!
hello world!\n
```

---

## 🔍路径搜索与文件树示例

### 1.精确查找文件或目录

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/world/world.txt");
    file.createFile("/world/work.txt");
    file.createFile("/world/vector/working.txt");

    file.createFile("/source/vector/world.txt");
    file.createFile("/source/vector/work.txt");
    file.createFile("/source/vector/working.txt");

    file.printTree();  // 通过串口打印文件树

    // 在根目录下查找 world.txt 文件
    Serial.println("1.在根目录下查找 world.txt 文件");
    std::vector<std::string> finded_list = file.findPath("world.txt", "/");
    for (auto& i : finded_list) Serial.println(i.c_str());  // 打印查找结果

    // 在 source 目录下查找 vector 目录
    Serial.println("2.在 source 目录下查找 vector 目录");
    finded_list = file.findPath("vector", "/source");
    for (auto& i : finded_list) Serial.println(i.c_str());  // 打印查找结果
}
```

串口输出:

```
├─/
│ ├─source
│ │ └─vector
│ │   ├─work.txt
│ │   ├─working.txt
│ │   └─world.txt
│ └─world
│   ├─vector
│   │ └─working.txt
│   ├─work.txt
│   └─world.txt

1.在根目录下查找 world.txt 文件
/source/vector/world.txt
/world/world.txt
2.在 source 目录下查找 vector 目录
source/vector
```

### 2.模糊查找文件或目录

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/world/world.txt");
    file.createFile("/world/work.txt");
    file.createFile("/world/vector/working.txt");

    file.createFile("/source/vector/world.os");
    file.createFile("/source/vector/work.os");
    file.createFile("/source/vector/working.os");

    file.printTree();  // 通过串口打印文件树

    // 在根目录下模糊查找 "txt", 相似度阈值设为"0"
    Serial.println("1.在根目录下模糊查找 \"txt\"");
    std::vector<std::string> finded_list = file.searchPath("txt", "/", 0.0);
    for (auto& i : finded_list) Serial.println(i.c_str());  // 打印查找结果
}
```

串口输出:

```
├─/
│ ├─source
│ │ └─vector
│ │   ├─work.os
│ │   ├─working.os
│ │   └─world.os
│ └─world
│   ├─vector
│   │ └─working.txt
│   ├─work.txt
│   └─world.txt

1.在根目录下模糊查找 "txt"
// 按相似度从高到低排序
/world/work.txt
/world/world.txt
/world/vector/working.txt

/source/vector
/world
/world/vector
/source
/source/vector/work.os
/source/vector/world.os
/source/vector/working.os
```

### 3.获取指定目录中的所有子项

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/source/vector/world.os");
    file.createFile("/source/vector/work.os");
    file.createFile("/source/vector/working.os");

    file.printTree();  // 通过串口打印文件树

    // 获取 vector 目录中的子项
    std::vector<std::string> file_list = file.listDir("/source/vector/");
    for (auto& i : file_list) Serial.println(i.c_str());  // 打印目录子项
}
```

串口输出:

```
├─/
│ └─source
│   └─vector
│     ├─work.os
│     ├─working.os
│     └─world.os

work.os
working.os
world.os
```

### 4.获取指定目录路径下的目录树结构

```cpp
#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/source/vector/world.os");
    file.createFile("/source/vector/work.os");
    file.createFile("/source/vector/working.os");

    // 获取 source 目录下的树结构
    std::unique_ptr<Tree<std::string>> tree = file.getTree("/source");

    // 初始化 TreeTool 工具
    TreeTool tree_tool;

    // 调用 TreeTool 获取树结构的字符串表示
    std::string list_str = tree_tool.getTreeString(*tree);

    // 打印文件树的字符串表示
    Serial.println(list_str.c_str());
}
```

串口输出:

```
├─/source
│ └─vector
│   ├─work.os
│   ├─working.os
│   └─world.os
```

> `TreeTool` 是通用树数据结构容器 `tree.hpp` 的补丁工具类，提供了递归获取树结构的功能。其主要作用是帮助用户以树形结构字符串的形式输出树，便于调试和查看树的层级关系。技术文档: [Tree Tool Documentation](/lib/containers/Tree%20Tool%20Documentation.md)

---

## ⚠️注意事项
1. **路径格式**  
   - 使用绝对路径，以 `/` 开头（如 `"/docs/file.txt"`,  `/` 表示根目录）。
   - 目录路径结尾无需斜杠（`"/data"` 而非 `"/data/"`）。
   - 慎用`deletePath("/");`, 这个操作将清除所有文件。
2. **错误处理**  
   - 大部分方法返回 `bool` 表示成功与否，建议检查返回值。
   - 操作前可调用 `exists(path)` 确认路径有效性。
3. **性能提示**  
   - `getTree()` 在深层目录中可能耗时，建议仅在需要完整结构时使用。

---

