/**
 * @file command_line_interface.cpp
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

#include <command_line_interface.h>

Command_Line_Interface::Command_Line_Interface() {
    cmd_table.builtin_cmd();  // 初始化内建命令表
}

void Command_Line_Interface::run() {
    if (read_serial_message()) {                             // 检查是否有新消息
        parse_command();                                     // 解析命令字符串
        cmd_table.execute_cmd(cmd_name, flags, parameters);  // 执行命令
    }
}

bool Command_Line_Interface::read_serial_message() {
    command_str.clear();  // 清空之前的命令缓存

    // 逐字节读取串口输入，直到无数据可读
    while (Serial.available()) {
        char c = static_cast<char>(Serial.read());

        // 忽略换行和回车
        if (c != '\n' && c != '\r') command_str += c;
    }

    // 如果命令为空，返回false
    return !command_str.empty();
}

void Command_Line_Interface::parse_command() {
    // 使用分割器将命令字符串按空格分割
    std::vector<std::string> tokens = splitter.split(command_str, {" "});

    // 如果分割结果为空，清空所有内容并发出警告
    if (tokens.empty()) {
        reset_command_data();
        WARN(WarningLevel::WARNING, "输入的命令为空");
        return;
    }

    // 设置命令名称为第一个部分
    cmd_name = tokens[0];

    // 遍历其余部分，分别解析标志和参数
    flags.clear();
    parameters.clear();
    for (size_t i = 1; i < tokens.size(); ++i) {
        if (!tokens[i].empty() && tokens[i][0] == '-') {
            flags.push_back(tokens[i]);  // 以'-'开头的为标志
        } else {
            parameters.push_back(tokens[i]);  // 其他为参数
        }
    }
}

void Command_Line_Interface::reset_command_data() {
    cmd_name.clear();
    flags.clear();
    parameters.clear();
}
