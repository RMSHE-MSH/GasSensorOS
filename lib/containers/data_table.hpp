/**
 * @file data_table.hpp
 * @date 05.08.2024
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2023 RMSHE. All rights reserved.
 *
 * This program is free software : you can redistribute it and /or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.If not, see < https://www.gnu.org/licenses/>.
 *
 * Electronic Mail : asdfghjkl851@outlook.com
 */

#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class DataTable {
   public:
    using Row = std::vector<std::string>;
    using Table = std::vector<Row>;

    /**
     * @brief 构造函数：初始化数据表的行数和列数
     * @param rows 数据表的行数
     * @param cols 数据表的列数
     */
    DataTable(size_t rows, size_t cols, const std::string& defaultValue = "") : data(rows, Row(cols, defaultValue)) {}

    // 获取表格的行数
    size_t getRowSize() { return data.size(); }

    // 获取表格的列数
    size_t getColSize() { return data[0].size(); }

    /**
     * @brief 替换数据到指定单元格
     * @param row 替换数据的行索引
     * @param col 替换数据的列索引
     * @param value 要替换的字符串值
     */
    void replaceCell(const std::string& value, size_t row, size_t col) {
        if (!checkBounds(row, col)) return;
        data[row][col] = value;
    }

    /**
     * @brief 替换整行数据
     * @param row 替换数据的行索引
     * @param values 要替换的行数据
     */
    void replaceRow(const Row& values, size_t row) {
        if (!checkRowBounds(row)) return;  // 检查行索引是否越界

        size_t row_size = getRowSize();

        // 创建一个与表格行长度匹配的行数据
        Row newRow(row_size, "");

        // 将要替换的值复制到新行中，多余的部分自动截断，缺失的部分置空
        std::copy(values.begin(), values.begin() + std::min(values.size(), row_size), newRow.begin());

        // 将新行替换到指定行位置
        data[row] = newRow;
    }

    /**
     * @brief 替换整列数据
     * @param col 替换数据的列索引
     * @param values 要替换的列数据
     */
    void replaceCol(const std::vector<std::string>& values, size_t col) {
        if (!checkColBounds(col)) return;  // 检查列索引是否越界

        size_t col_size = getColSize();

        // 创建一个与表格行数匹配的新列数据
        std::vector<std::string> newCol(col_size, "");

        // 将要替换的值复制到新列中，多余的部分自动截断，缺失的部分置空
        std::copy(values.begin(), values.begin() + std::min(values.size(), col_size), newCol.begin());

        // 将新列数据替换到指定列位置
        for (size_t i = 0; i < col_size; ++i) {
            data[i][col] = newCol[i];
        }
    }

    /**
     * @brief 在指定位置插入一个新单元格数据
     * @param value 要插入的字符串值 (默认值为空字符串)
     * @param row 插入数据的行索引 (默认值为表格末尾)
     * @param col 插入数据的列索引 (默认值为表格末尾)
     *
     * 此函数用于在表格的指定位置插入一个单元格数据。如果指定的行或列超出当前表格范围，
     * 表格将自动扩展以容纳新数据。如果未指定行列参数，则默认插入到表格的最后一行和最后一列。
     * 插入操作会将目标位置的数据向后移动。
     */
    void insertCell(const std::string& value = "", size_t row = std::numeric_limits<size_t>::max(), size_t col = std::numeric_limits<size_t>::max()) {
        size_t row_size = getRowSize();
        size_t col_size = getColSize();

        // 如果指定的行超出当前表格范围，则扩展表格以容纳新行，否则在指定位置插入新行
        if (row >= row_size) {
            row = row_size;
            insertRow({}, row_size);  // 在末尾插入新行
        } else {
            insertRow({}, row);  // 在指定位置插入新行
        }

        // 如果指定的列超出当前表格范围，则扩展表格以容纳新列，否则在指定位置插入新列
        if (col >= col_size) {
            col = col_size;
            insertCol({}, col_size);  // 在末尾插入新列
        } else {
            insertCol({}, col);  // 在指定位置插入新列
        }

        // 在指定的单元格位置插入数据，并将目标位置之后的数据向后移动
        data[row][col] = value;
    }

    /**
     * @brief 在指定位置插入新行
     * @param values 新行的数据(为空时默认插入空行)
     * @param row 插入新行的位置(为空或超出表格行数时默认将新行追加到表格末尾)
     *
     * 此函数用于在数据表的指定行插入一行新数据。如果 values 参数为空，
     * 将插入一个空行。如果指定的行索引 row 超出表格范围，则新行将被追加
     * 到表格的最后一行。
     */
    void insertRow(const std::vector<std::string>& values = Row(), size_t row = std::numeric_limits<size_t>::max()) {
        size_t col_size = getColSize();  // 获取当前表格的列数

        std::vector<std::string> newRow(col_size, "");

        // 如果 values 非空，将其中的内容复制到新行中，多余部分截断
        if (!values.empty()) {
            std::copy(values.begin(), values.begin() + std::min(values.size(), col_size), newRow.begin());
        }

        // 如果行位置超出表格范围，将新行追加到末尾
        if (row >= getRowSize()) {
            data.emplace_back(std::move(newRow));
        } else {
            data.insert(data.begin() + row, std::move(newRow));
        }
    }

    /**
     * @brief 在指定位置插入新列
     * @param values 新列的数据(为空时默认插入空列)
     * @param col 插入新列的位置(为空或超出表格列数时默认将新列追加到表格末尾)
     *
     * 此函数用于在数据表的指定列插入一列新数据。如果 values 参数为空，
     * 将插入一个空列。如果指定的列索引 col 超出表格范围，则新列将被追加
     * 到表格的最后一列。
     */
    void insertCol(const std::vector<std::string>& values = std::vector<std::string>(), size_t col = std::numeric_limits<size_t>::max()) {
        size_t row_size = getRowSize();  // 获取当前表格的行数
        size_t col_size = getColSize();  // 获取当前表格的列数

        // 如果 col 参数超出范围或未指定，默认将新列追加到末尾
        if (col >= col_size) col = col_size;

        // 遍历每一行，在指定位置插入新列
        for (size_t i = 0; i < row_size; ++i) {
            std::string value = (i < values.size()) ? values[i] : "";  // 多余部分自动截断，缺失部分置空
            data[i].insert(data[i].begin() + col, std::move(value));   // 在指定位置插入数据
        }
    }

    /**
     * @brief 获取指定单元格的数据
     * @param row 获取数据的行索引
     * @param col 获取数据的列索引
     * @return 返回该单元格的字符串数据
     */
    std::string getCell(size_t row, size_t col) const {
        if (!checkBounds(row, col)) return "";  // 检查列索引是否越界
        return data[row][col];
    }

    /**
     * @brief 获取整行数据
     * @param row 获取数据的行索引
     * @return 返回该行的数据
     */
    std::vector<std::string> getRow(size_t row) const {
        if (!checkRowBounds(row)) return {};  // 检查行索引是否越界
        return data[row];
    }

    /**
     * @brief 获取整列数据
     * @param col 获取数据的列索引
     * @return 返回该列的数据
     */
    std::vector<std::string> getCol(size_t col) const {
        if (!checkColBounds(col)) return {};  // 检查列索引是否越界

        std::vector<std::string> column;
        for (const auto& row : data) {
            column.push_back(row[col]);
        }
        return column;
    }

    /**
     * @brief 清空指定单元格的数据
     * @param row 清空数据的行索引
     * @param col 清空数据的列索引
     */
    void clearCell(size_t row, size_t col) {
        if (!checkBounds(row, col)) return;
        replaceCell("", row, col);
    }

    /**
     * @brief 清空整行数据
     * @param row 清空数据的行索引
     */
    void clearRow(size_t row) {
        if (!checkRowBounds(row)) return;  // 检查行索引是否越界
        std::fill(data[row].begin(), data[row].end(), "");
    }

    /**
     * @brief 清空整列数据
     * @param col 清空数据的列索引
     */
    void clearCol(size_t col) {
        if (!checkColBounds(col)) return;  // 检查列索引是否越界
        for (auto& row : data) {
            row[col] = "";
        }
    }

    /**
     * @brief 清空整个数据表
     */
    void clear() {
        for (auto& row : data) std::fill(row.begin(), row.end(), "");
    }

    /**
     * @brief 删除指定位置的行并释放内存
     * @param row 要删除的行索引
     *
     * 如果指定的行索引有效，将删除该行，并释放对应的内存空间。
     */
    void deleteRow(size_t row) {
        if (!checkRowBounds(row)) return;  // 检查行索引是否越界
        data.erase(data.begin() + row);    // 删除指定的行
        data.shrink_to_fit();              // 释放未使用的内存
    }

    /**
     * @brief 删除指定位置的列并释放内存
     * @param col 要删除的列索引
     *
     * 如果指定的列索引有效，将删除该列，并释放对应的内存空间。
     * 由于每一行都有一个对应的列，因此需要对每一行都进行操作。
     */
    void deleteCol(size_t col) {
        if (!checkColBounds(col)) return;  // 检查列索引是否越界

        for (auto& row : data) {
            row.erase(row.begin() + col);  // 删除每一行中的指定列
            row.shrink_to_fit();           // 释放每行未使用的内存
        }

        data.shrink_to_fit();  // 释放整体数据表未使用的内存
    }

    /**
     * @brief 交换表格中两行的数据
     * @param row1 第一个要交换的行索引
     * @param row2 第二个要交换的行索引
     */
    void swapRows(size_t row1, size_t row2) {
        // 检查行索引是否有效
        if (!checkRowBounds(row1) || !checkRowBounds(row2)) return;

        // 使用 std::swap 直接交换两行的内容
        std::swap(data[row1], data[row2]);
    }

    /**
     * @brief 交换表格中两列的数据
     * @param col1 第一个要交换的列索引
     * @param col2 第二个要交换的列索引
     */
    void swapCols(size_t col1, size_t col2) {
        // 检查列索引是否有效
        if (!checkColBounds(col1) || !checkColBounds(col2)) return;

        // 遍历每一行，交换指定列的数据
        for (auto& row : data) {
            std::swap(row[col1], row[col2]);
        }
    }

    /**
     * @brief 重设表格大小
     * @param newRows 新的行数
     * @param newCols 新的列数
     */
    void resize(size_t newRows, size_t newCols) {
        // 先调整行数，如果减少行数则多余的行会被裁剪掉
        data.resize(newRows);

        // 调整每一行的列数，并释放多余的内存
        for (auto& row : data) {
            row.resize(newCols);  // 调整列数
            row.shrink_to_fit();  // 释放多余的列内存
        }

        // 最后释放多余的行内存
        data.shrink_to_fit();
    }

    /**
     * @brief 条件查询
     * @param value 查询的目标值
     * @param startRow 查询范围的起始行索引（包含）
     * @param endRow 查询范围的结束行索引（包含）
     * @param startCol 查询范围的起始列索引（包含）
     * @param endCol 查询范围的结束列索引（包含）
     * @return 返回匹配的单元格位置的向量 (row, col)
     */
    std::vector<std::pair<size_t, size_t>> query(const std::string& value, size_t startRow = 0, size_t endRow = std::numeric_limits<size_t>::max(),
                                                 size_t startCol = 0, size_t endCol = std::numeric_limits<size_t>::max()) {
        std::vector<std::pair<size_t, size_t>> results;

        // 获取表格的实际行数和列数
        size_t rowCount = getRowSize();
        size_t colCount = getColSize();

        // 如果表格为空，直接返回空结果
        if (rowCount == 0 || colCount == 0) return results;

        // 限制结束行和列在表格实际范围内
        endRow = std::min(endRow, rowCount - 1);
        endCol = std::min(endCol, colCount - 1);

        // 确保从较小的索引到较大的索引进行遍历
        size_t rowBegin = std::min(startRow, endRow);
        size_t rowEnd = std::max(startRow, endRow);
        size_t colBegin = std::min(startCol, endCol);
        size_t colEnd = std::max(startCol, endCol);

        // 遍历指定范围内的单元格
        for (size_t row = rowBegin; row <= rowEnd; ++row) {
            for (size_t col = colBegin; col <= colEnd; ++col) {
                // 查找匹配的值
                if (data[row][col] == value) {
                    results.emplace_back(row, col);  // 找到匹配项，添加到结果中
                }
            }
        }

        return results;  // 返回所有匹配的单元格位置
    }

    /**
     * @brief 从串口打印整个数据表
     */
    void printTable() const {
        for (const auto& row : data) {
            for (const auto& col : row) {
                Serial.print(col.c_str());
                Serial.print("\t");
            }
            Serial.print("\n");
        }
    }

    /**
     * @brief 从现有表格中提取一个子表格
     * @param startRow 子表格的起始行索引（包含）
     * @param endRow 子表格的结束行索引（包含）
     * @param startCol 子表格的起始列索引（包含）
     * @param endCol 子表格的结束列索引（包含）
     * @return 返回提取的子表格数据
     */
    DataTable extractTable(size_t startRow, size_t endRow, size_t startCol, size_t endCol) {
        size_t rowCount = getRowSize();
        size_t colCount = getColSize();

        // 限制结束行和列在表格实际范围内
        endRow = std::min(endRow, rowCount - 1);
        endCol = std::min(endCol, colCount - 1);

        // 确保从较小的索引到较大的索引进行遍历
        size_t rowBegin = std::min(startRow, endRow);
        size_t rowEnd = std::max(startRow, endRow);
        size_t colBegin = std::min(startCol, endCol);
        size_t colEnd = std::max(startCol, endCol);

        // 创建子表格
        DataTable subTable(rowEnd - rowBegin + 1, colEnd - colBegin + 1);
        for (size_t i = rowBegin; i <= rowEnd; ++i) {
            for (size_t j = colBegin; j <= colEnd; ++j) {
                subTable.data[i - rowBegin][j - colBegin] = data[i][j];
            }
        }

        return subTable;  // 返回子表格
    }

    /**
     * @brief 将一个子表格覆盖到当前表格的指定位置
     * @param subTable 要覆盖的子表格数据
     * @param startRow 覆盖子表格的起始行索引
     * @param startCol 覆盖子表格的起始列索引
     */
    void replaceTable(DataTable& subTable, size_t startRow, size_t startCol) {
        size_t rowCount = getRowSize();
        size_t colCount = getColSize();

        size_t subRowCount = subTable.getRowSize();
        size_t subColCount = subTable.getColSize();

        // 调整当前表格的大小以容纳子表格（如有必要）
        if (startRow + subRowCount > rowCount) {
            data.resize(startRow + subRowCount);
        }
        if (startCol + subColCount > colCount) {
            for (auto& row : data) {
                row.resize(startCol + subColCount);
            }
        }

        // 将子表格数据覆盖到指定位置
        for (size_t i = 0; i < subRowCount; ++i) {
            for (size_t j = 0; j < subColCount; ++j) {
                data[startRow + i][startCol + j] = subTable.data[i][j];
            }
        }
    }

    // 删除当前表格并释放内存
    void deleteTable() {
        data.clear();
        data.shrink_to_fit();  // 释放内存
    }

   private:
    Table data;

    // 边界检查: 检查给定的行列索引是否在表格的有效范围内(有效则返回true)
    bool checkBounds(size_t row, size_t col) const {
        if (row >= data.size() || col >= data[0].size()) return false;
        return true;
    }

    // 边界检查: 检查给定的行索引是否在表格的有效范围内(有效则返回true)
    bool checkRowBounds(size_t row) const {
        if (row >= data.size()) return false;
        return true;
    }

    // 边界检查: 检查给定的列索引是否在表格的有效范围内(有效则返回true)
    bool checkColBounds(size_t col) const {
        if (col >= data[0].size()) return false;
        return true;
    }
};
