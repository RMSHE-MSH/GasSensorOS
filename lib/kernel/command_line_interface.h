/**
 * @file command_line_interface.h
 * @date 01.01.2025
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2025 RMSHE. All rights reserved.
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

#include <command_table.hpp>
#include <string>
#include <string_edit.hpp>
#include <vector>

/**
 * @brief Command Line Interface (CLI) 类，用于处理命令行输入并执行相应的命令。
 */
class Command_Line_Interface {
   public:
    /**
     * @brief 构造函数，初始化内建命令。
     */
    Command_Line_Interface();

    /**
     * @brief CLI主循环，用于读取串口消息并处理。
     */
    void run();

   private:
    /**
     * @brief 从串口读取命令字符串。
     * @return 如果读取到有效命令返回true，否则返回false。
     */
    bool read_serial_message();

    /**
     * @brief 解析命令字符串，将其拆分为命令名称、标志和参数。
     */
    void parse_command();

    /**
     * @brief 重置命令数据。
     */
    void reset_command_data();

   private:
    COMMAND_TABLE cmd_table;  // 命令表对象，用于存储和执行命令
    StringSplitter splitter;  // 字符串分割工具

    std::string command_str;              // 原始命令字符串
    std::string cmd_name;                 // 解析后的命令名称
    std::vector<std::string> flags;       // 解析后的标志
    std::vector<std::string> parameters;  // 解析后的参数
};
