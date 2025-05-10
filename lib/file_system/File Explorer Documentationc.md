# FileExplorer 类使用指南

## 概述
`FileExplorer` 类提供了一套用于管理文件系统的接口，支持文件/目录的创建、删除、复制、移动、重命名，以及文件内容的读写和路径搜索功能。适用于需要嵌入式文件系统管理（如SPIFFS、SD卡）的场景。

---

## 快速开始
1. **初始化**  
   创建 `FileExplorer` 对象时自动挂载文件系统，析构时自动卸载。
   ```cpp
   FileExplorer explorer; // 默认初始化
   ```

---

## 核心功能

### 文件与目录管理
| 方法                       | 参数说明                 | 返回值/行为                       |
| -------------------------- | ------------------------ | --------------------------------- |
| `createFile(path)`         | 完整文件路径（含文件名） | 创建文件及父目录，成功返回 `true` |
| `createDir(path)`          | 完整目录路径             | 创建目录，成功返回 `true`         |
| `copyPath(src,dst)`        | 源路径、目标路径         | 复制文件/目录到新位置             |
| `movePath(src,dst)`        | 源路径、目标路径         | 移动文件/目录（复制后删除源）     |
| `renamePath(path,newName)` | 原路径、新名称           | 重命名文件/目录                   |
| `deletePath(path)`         | 目标路径                 | 直接删除文件/目录                 |
| `exists(path)`             | 路径                     | 存在则返回 `true`                 |

### 文件内容读写
| 方法                           | 参数说明                                  | 示例                                                    |
| ------------------------------ | ----------------------------------------- | ------------------------------------------------------- |
| `readFileAsString(path)`       | 文件路径                                  | `string data = explorer.readFileAsString("/log.txt");`  |
| `writeFileAsString(path,data)` | 路径、字符串数据、模式（默认为追加`"a"`） | `explorer.writeFileAsString("/log.txt", "Hello", "w");` |
| `readFileAsBytes(path)`        | 文件路径                                  | 读取二进制文件                                          |
| `writeFileAsBytes(path,data)`  | 路径、字节数组、模式                      | 写入二进制数据                                          |

### 路径搜索与遍历
| 方法                                      | 参数说明                                  | 返回值                                    |
| ----------------------------------------- | ----------------------------------------- | ----------------------------------------- |
| `findPath(name, parentPath)`              | 目标名称、起始目录（默认根目录）          | 返回所有精确匹配的完整路径列表            |
| `searchPath(name, parentPath, threshold)` | 目标名称、起始目录、相似度阈值（0.0~1.0） | 返回相似度≥阈值的路径列表（按相似度降序） |
| `listDir(dirPath)`                        | 目录路径                                  | 返回子项名称列表                          |
| `printDir(dirPath)`                       | 目录路径                                  | 串口打印目录内容                          |
| `getTree(dirPath)`                        | 目录路径                                  | 返回目录树的智能指针                      |

---

## 使用示例

### 创建文件并写入内容
```cpp
if (explorer.createFile("/data/config.json")) {
  explorer.writeFileAsString("/data/config.json", "{\"mode\":1}", "w");
}
```

### 模糊搜索文件
```cpp
auto results = explorer.searchPath("log", "/", 0.4);
// 可能返回 ["/logs/2023.txt", "/backup/log_old.txt"]
```

### 遍历目录树
```cpp
auto tree = explorer.getTree("/projects");
explorer.printTree("/projects"); // 打印树形结构
```

---

## 注意事项
1. **路径格式**  
   - 使用绝对路径，以 `/` 开头（如 `"/docs/file.txt"`）。
   - 目录路径结尾无需斜杠（`"/data"` 而非 `"/data/"`）。

2. **错误处理**  
   - 大部分方法返回 `bool` 表示成功与否，建议检查返回值。
   - 操作前可调用 `exists(path)` 确认路径有效性。

3. **性能提示**  
   - `getTree()` 在深层目录中可能耗时，建议仅在需要完整结构时使用。
   - 频繁写入时考虑使用 `writeFile` 的追加模式（`"a"`）。

4. **依赖项**  
   - 需包含 `<file_explorer.h>` 和文件系统库（如SPIFFS、SD库）。
   - 确保文件系统已正确初始化（如 `SPIFFS.begin()`）。