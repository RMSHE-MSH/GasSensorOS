# `DataTable` 类

## 1. 概述

`DataTable` 类提供了一种简单而高效的方式来管理二维数据表格，适用于嵌入式系统中的数据处理需求。该类提供了基本的表格操作功能, 支持动态调整表格的尺寸、插入、删除、查询和替换数据等基本操作。

### 1.1 功能特性
- 表格创建与初始化：支持创建指定行列数的表格，并可设定初始默认值。

- 单元格操作：支持单个单元格的数据插入、替换、清空操作。
- 行/列操作：支持整行或整列的数据插入、替换、清空、删除等操作。
- 表格操作：支持整表清空、删除表格、提取与替换子表格等操作。
- 查询功能：支持在表格内进行条件查询，并返回匹配结果的位置。

---

## 2. 成员函数

### 2.1 构造函数

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `DataTable(size_t rows, size_t cols, const std::string& defaultValue = "")` | **创建一个指定行列数的表格，单元格使用指定的默认值进行初始化。**<br/>`rows`: 表格的初始行数。<br/>`cols`: 表格的初始列数。<br/>`defaultValue`: 每个单元格的默认值，默认为空字符串。 |

### 2.2 插入与删除

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `void insertCell(size_t row, size_t col, const std::string& value)` | **在指定位置插入一个新单元格数据，表格会自动扩展以容纳新数据。**<br/>`row`: 要插入的行索引。<br/>`col`: 要插入的列索引。<br/>`value`: 要插入的字符串值。 |
| `void insertRow(const std::vector<std::string>& values = {}, size_t row = size_t_max)` | **在指定位置插入一行，如果 `values` 超出列数，则自动截断；不足时用空字符串填充。**<br/>`values`:  插入行的初始数据。<br/>`row`: 要插入行的位置索引。如果未指定，则在表格末尾插入。 |
| `void insertCol(const std::vector<std::string>& values = {}, size_t col = size_t_max)` | **在指定位置插入一列，如果 `values` 超出行数，则自动截断；不足时用空字符串填充。**<br/>`values`:  插入行的初始数据。<br/>`col`: 要插入列的位置索引。如果未指定，则在表格末尾插入。 |
| `void deleteRow(size_t row)`                                 | **删除指定行并调整表格大小。**<br/>`row`:  要删除行的位置索引。 |
| `void deleteCol(size_t col)`                                 | **删除指定列并调整表格大小。**<br/>`col`:  要删除列的位置索引。 |

### 2.3 替换与清空

| 函数名                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `void replaceCell(size_t row, size_t col, const std::string& value)**`** | **替换指定位置的单元格数据。**<br/>`row`: 要替换的行索引。<br/>`col`: 要替换的列索引。<br/>`value`: 新的字符串值。 |
| `void replaceRow(size_t row, const std::vector<std::string>& values)` | **替换整行数据，如果 `values` 长度不足，用空字符串填充。**<br/>`row`: 要替换的行索引。<br/>`values`: 替换行的初始数据。长度超过当前列数时自动截断，不足时用空字符串填充。 |
| `void replaceCol(size_t col, const std::vector<std::string>& values)` | **替换整列数据，如果 `values` 长度不足，用空字符串填充。**<br/>`col`: 要替换的列索引。<br/>`values`: 替换列的初始数据。长度超过当前行数时自动截断，不足时用空字符串填充。 |
| `void clearCell(size_t row, size_t col)`                     | **清空指定单元格的数据。**<br/>`row`: 要清空的行索引。<br/>`col`: 要清空的列索引。 |
| `void clearRow(size_t row)`                                  | **清空指定行的所有数据。**<br/>`row`: 要清空的行索引。       |
| `void clearCol(size_t col)`                                  | **清空指定列的所有数据。**<br/>`col`: 要清空的列索引。       |

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

| 函数名                                        | 描述                                                         |
| --------------------------------------------- | ------------------------------------------------------------ |
| `void resize(size_t newRows, size_t newCols)` | **调整表格的行列尺寸。**<br/>`newRows`: 新的行数。<br/>`newCols`: 新的列数。 |
| `void clear()`                                | **清空整个表格，将所有单元格重置为默认值。**                 |
| `void deleteTable()`                          | **删除整个表格并释放内存。**                                 |
| `void printTable() const`                     | **通过串口打印表格的所有内容。**                             |

---

## 示例

```cpp
#include <data_table.hpp>

int main() {
    DataTable table(3, 3, "");  // 创建一个3x3的表格，默认值为""
    
    table.insertCell(0, 0, "A");  // 在(0,0)位置插入"A"
    table.replaceRow(1, {"1", "2", "3"});  // 替换第1行数据
    table.printTable();  // 打印表格

    return 0;
}
```
