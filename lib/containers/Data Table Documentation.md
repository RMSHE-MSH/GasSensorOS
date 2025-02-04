# `DataTable` 类

## 1. 概述

`DataTable` 类提供了一种简单而高效的方式来管理二维数据表格，适用于嵌入式系统中的数据处理需求。该类提供了基本的表格操作功能, 支持动态调整表格的尺寸、插入、删除、查询和替换数据等基本操作。

### 1.1 功能特性
- `表格创建与初始化`：支持创建指定行列数的表格，并可设定初始默认值。

- `单元格操作`：支持单个单元格的数据插入、替换、清空操作。
- `行/列操作`：支持整行或整列的数据插入、替换、清空、删除等操作。
- `表格操作`：支持整表清空、删除表格、提取与替换子表格等操作。
- `查询功能`：支持在表格内进行条件查询，并返回匹配结果的位置。
- `保存和读取表格`: 支持以覆写或追加模式保存表格数据到CSV文件，同时可以从CSV文件加载数据并更新当前表格内容。

---

## 2. 成员函数

### 2.1 构造函数

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `DataTable(size_t rows, size_t cols, const std::string& defaultValue = "")` | **创建一个指定行列数的表格，单元格使用指定的默认值进行初始化。**<br/>`rows`: 表格的初始行数。<br/>`cols`: 表格的初始列数。<br/>`defaultValue`: 每个单元格的默认值，默认为空字符串。 |

### 2.2 插入与删除

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `void insertCell(std::string& value, size_t row, size_t col, const)` | **在指定位置插入一个新单元格数据，表格会自动扩展以容纳新数据。**<br/>`value`: 要插入的字符串值。<br/>`row`: 要插入的行索引。<br/>`col`: 要插入的列索引。 |
| `void insertRow(const std::vector<std::string>& values = {}, size_t row = size_t_max)` | **在指定位置插入一行，如果 `values` 超出列数，则自动截断；不足时用空字符串填充。**<br/>`values`:  插入行的初始数据。<br/>`row`: 要插入行的位置索引。如果未指定，则在表格末尾插入。 |
| `void insertCol(const std::vector<std::string>& values = {}, size_t col = size_t_max)` | **在指定位置插入一列，如果 `values` 超出行数，则自动截断；不足时用空字符串填充。**<br/>`values`:  插入列的初始数据。<br/>`col`: 要插入列的位置索引。如果未指定，则在表格末尾插入。 |
| `void deleteRow(size_t row)`                                 | **删除指定行并调整表格大小。**<br/>`row`:  要删除行的位置索引。 |
| `void deleteCol(size_t col)`                                 | **删除指定列并调整表格大小。**<br/>`col`:  要删除列的位置索引。 |

### 2.3 替换与清空

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `void replaceCell(const std::string& value, size_t row, size_t col)` | **替换指定位置的单元格数据。**<br/>`value`: 新的字符串值。<br/>`row`: 要替换的行索引。<br/>`col`: 要替换的列索引。 |
| `void replaceRow( const std::vector<std::string>& values, size_t row)` | **替换整行数据，如果 `values` 长度不足，用空字符串填充。**<br/>`values`: 替换行的初始数据。长度超过当前列数时自动截断，不足时用空字符串填充。<br/>`row`: 要替换的行索引。 |
| `void replaceCol(const std::vector<std::string>& values, size_t col)` | **替换整列数据，如果 `values` 长度不足，用空字符串填充。**<br/>`values`: 替换列的初始数据。长度超过当前行数时自动截断，不足时用空字符串填充。<br/>`col`: 要替换的列索引。 |
| `void clearCell(size_t row, size_t col)`                     | **清空指定单元格的数据。**<br/>`row`: 要清空的行索引。<br/>`col`: 要清空的列索引。 |
| `void clearRow(size_t row)`                                  | **清空指定行的所有数据。**<br/>`row`: 要清空的行索引。       |
| `void clearCol(size_t col)`                                  | **清空指定列的所有数据。**<br/>`col`: 要清空的列索引。       |
| `void copyTable(Table& table)`                               | **智能表格复制。**<br/>`table`: 输入输出参数，根据其空状态决定复制方向。<br/>功能逻辑：<br/>1. 当 `table` 为空时，将 `DataTable` 中的数据拷贝到 `table`。<br/>2. 当 `table` 非空时，将 `table` 拷贝回 `DataTable`。 |

### 2.4 访问与查询

| 函数名                                                       | 描述                                                         |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| `size_t getRowSize()`                                        | **获取表格行数。**                                           |
| `size_t getColSize()`                                        | **获取表格列数。**                                           |
| `std::string getCell(size_t row, size_t col) const`          | **返回指定位置的单元格数据。**<br/>`row`: 单元格的行索引。<br/>`col`: 单元格的列索引。<br/>`返回值`: 单元格中的字符串值。 |
| `std::vector<std::string> getRow(size_t row) const`          | **返回指定行的所有数据。**<br/>`row`: 单元格的行索引。<br/>`返回值`:  包含该行所有数据的字符串向量。 |
| `std::vector<std::string> getCol(size_t col) const`          | **返回指定列的所有数据。**<br/>`col`: 单元格的列索引。<br/>`返回值`:  包含该列所有数据的字符串向量。 |
| `std::vector<std::pair<size_t, size_t>> query(const std::string& value, size_t startRow = 0, size_t endRow = size_t_max, size_t startCol = 0, size_t endCol = size_t_max)` | **查询表格中指定范围内的所有匹配项。**<br/>`value`: 要查询的字符串值。<br/>`startRow`: 查询起始行索引。<br/>`endRow`: 查询结束行索引（包含）。<br/>`startCol`: 查询起始列索引。<br/>`endCol`: 查询结束列索引（包含）。<br/>`返回值`: 包含所有匹配单元格的行列对的向量。 |

### 2.5 高级操作

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `void swapRows(size_t row1, size_t row2)`                    | **交换表格中的两行数据。**<br/>`row1`: 第一个要交换的行索引。<br/>`row2`: 第二个要交换的行索引。 |
| `void swapCols(size_t col1, size_t col2)`                    | **交换表格中的两列数据。**<br/>`col1`: 第一个要交换的列索引。<br/>`col2`: 第二个要交换的列索引。 |
| `DataTable extractTable(size_t startRow, size_t endRow, size_t startCol, size_t endCol)` | **提取表格的一个子集并返回一个新表格**。<br/>`startRow`: 子表格的起始行索引。<br/>`endRow`: 子表格的结束行索引（包含）。<br/>`startCol`: 子表格的起始列索引。<br/>`endCol`: 子表格的结束列索引（包含）。<br/>`返回值`: 包含提取数据的 `DataTable` 实例。 |
| `void replaceTable(DataTable& subTable, size_t startRow, size_t startCol)` | **使用子表格数据替换主表格中指定区域的数据。**<br/>`subTable`: 包含替换数据的 `DataTable` 实例。<br/>`startRow`: 替换起始行索引。<br/>`endRow`: 替换起始列索引。 |

### 2.6 其他操作

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `void resize(size_t newRows, size_t newCols)`                | **调整表格的行列尺寸。**<br/>`newRows`: 新的行数。<br/>`newCols`: 新的列数。 |
| `void clear()`                                               | **清空整个表格，将所有单元格重置为默认值。**                 |
| `void deleteTable()`                                         | **删除整个表格并释放内存。**                                 |
| `void printTable() const`                                    | **通过串口打印表格的所有内容。**                             |
| `std::string getTableString()`                               | **获取表格的字符串表示。**                                   |
| `void saveTable(const std::string& filePath, const char* mode = "w")` | **将 `DataTable` 的内容保存为 CSV 格式文件。**<br/>`filePath`: 要保存的文件路径。<br/>`mode`: 文件打开模式，支持 `w`:覆写 和 `a`:追加，默认为覆写模式。<br/>在覆写模式下，会创建一个新的文件，且如果文件已存在则不会覆盖写入，避免误删除数据。在追加模式下，会在文件末尾追加数据。 |
| `void loadTable(const std::string& filePath)`                | **从指定 CSV 格式文件加载表格数据。**<br/>`filePath`: 文件路径，指向包含表格数据的文本文件。 |

---

## 例1: 创建表格&插入和替换数据

```cpp
#include <data_table.hpp>

int main() {
    DataTable table(3, 3, "");  // 创建一个3x3的表格，默认值为""
    
    table.insertCell(0, 0, "A");  // 在(0,0)位置插入"A"
    table.replaceRow({"1", "2", "3"}, 1);  // 替换第1行数据
    table.printTable();  // 打印表格

    return 0;
}
```

## 例2: 保存和加载CSV文件

```c++
#include <data_table.hpp>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 创建一个7行13列的DataTable，默认值为"*"，用于初始化表格。
    DataTable table(7, 13, "*");

    // 替换第6行的内容，提供的行数据包括13个元素
    // 这里的替换行数据是：{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "N"}
    table.replaceRow({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "N"}, 6);

    // 替换第0行的内容，提供的行数据包括6个元素
    // 这里的替换行数据是：{"dog", "cat", "1145", "@", "%", "#"}
    table.replaceRow({"dog", "cat", "1145", "@", "%", "#"}, 0);

    // 将修改后的表格保存到路径"/.os/table.csv"的CSV文件中
    table.saveTable("/.os/table.csv");

    // 创建一个新的DataTable对象table_load，初始化时只有1行1列，默认值为空字符串
    DataTable table_load(1, 1, "");

    // 从指定路径"/.os/table.csv"加载CSV文件并填充到table_load表格中
    table_load.loadTable("/.os/table.csv");

    // 打印加载的表格数据到串口
    table_load.printTable();
}
```

OUTPUT:
```c++
+-----+-----+------+---+---+---+---+---+---+---+---+---+---+
| dog | cat | 1145 | @ | % | # |   |   |   |   |   |   |   |
+-----+-----+------+---+---+---+---+---+---+---+---+---+---+
| *   | *   | *    | * | * | * | * | * | * | * | * | * | * |
| *   | *   | *    | * | * | * | * | * | * | * | * | * | * |
| *   | *   | *    | * | * | * | * | * | * | * | * | * | * |
| *   | *   | *    | * | * | * | * | * | * | * | * | * | * |
| *   | *   | *    | * | * | * | * | * | * | * | * | * | * |
| A   | B   | C    | D | E | F | G | H | I | J | K | L | N |
+-----+-----+------+---+---+---+---+---+---+---+---+---+---+
```

---
