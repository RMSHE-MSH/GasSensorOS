/**
 * @file command_table.hpp
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

#pragma onces
#include <Arduino.h>

#include <data_table.hpp>
#include <file_explorer_shell.hpp>
#include <functional>
#include <iostream>
#include <serial_warning.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <wifi_connector.hpp>

class CMD_FUNC {
   public:
    // 帮助命令，输出简单的帮助信息
    void help(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        Serial.println("Hello help!");  // 输出帮助信息
    }

    // 打印操作系统信息
    void osinfo(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        Serial.println(
            "GasSensor OS\nVersion: Alpha.Mark3.2024.12.28\nUpdate: https://github.com/RMSHE-MSH/GasSensorOS\nPowered by "
            "RMSHE\nE-mail: asdfghjkl851@outlook.com");
    }

    void test(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        bool flag_f = false;
        bool flag_s = false;

        for (const auto& flag : flags) {
            if (flag == "-f")
                flag_f = true;
            else if (flag == "-s")
                flag_s = true;
        }

        std::string output = "test";

        if (flag_f) output += " -f";
        if (flag_s) output += " -s";

        for (const auto& param : parameters) output += " " + param;

        Serial.println(output.c_str());
    }

    // 连接WIFI: <wifi_connect> ["SSID"] ["PASSWORD"]
    void wifi_connect(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        WiFiConnector WIFI(parameters[0].c_str(), parameters[1].c_str(), true);
        WIFI.connect();
    }
};

class COMMAND_TABLE {
   public:
    /**
     * @brief 添加命令及其处理函数
     *
     * 该方法用于将新的命令及其相关信息（标志和处理函数）添加到命令表中。
     * 如果命令表中已经存在同名命令，会打印警告信息并拒绝添加新命令。
     *
     * @param cmd_name 命令名称（每个命令必须是唯一的）
     * @param flags 命令的标志（例如：-f，-s等，可以为空）
     * @param func 命令对应的处理函数，接受标志和参数并执行实际的命令处理
     */
    void add_cmd(const std::string& cmd_name, const std::vector<std::string>& flags,
                 std::function<void(const std::vector<std::string>&, const std::vector<std::string>&)> func) {
        // 如果命令已经存在，输出警告并返回
        if (command_table.find(cmd_name) != command_table.end()) {
            WARN(WarningLevel::WARNING, "命令已存在：%s", cmd_name.c_str());
            return;
        }
        // 将命令、标志和处理函数存入命令表
        command_table[cmd_name] = {flags, func};
    }

    /**
     * @brief 删除指定名称的命令
     *
     * 如果命令表中存在该命令，则从命令表中删除该命令。
     * 否则，打印警告信息。
     *
     * @param cmd_name 要删除的命令名称
     */
    void delete_cmd(const std::string& cmd_name) {
        // 查找命令是否存在于命令表中
        auto it = command_table.find(cmd_name);
        if (it != command_table.end()) {
            // 删除命令
            command_table.erase(it);
        } else {
            WARN(WarningLevel::WARNING, "命令不存在：%s", cmd_name.c_str());
        }
    }

    /**
     * @brief 验证命令及其标志是否合法
     *
     * 该方法检查命令名是否在命令表中存在。如果命令存在，再检查传入的标志
     * 是否为空或者是否包含在该命令的有效标志列表中。如果标志无效，返回 `false`。
     *
     * @param cmd_name 要验证的命令名称
     * @param flag 要验证的命令标志
     * @return 如果命令合法返回 `true`，否则返回 `false`
     */
    bool verify_cmd(const std::string& cmd_name, const std::string& flag) const {
        // 查找命令是否在命令表中
        auto it = command_table.find(cmd_name);
        if (it == command_table.end()) {
            WARN(WarningLevel::WARNING, "未找到匹配的命令名称: %s", cmd_name.c_str());
            return false;  // 如果命令未找到，返回 false
        }
        // 如果标志为空，或者标志存在于该命令的有效标志中，返回 true
        if (flag.empty() || std::find(it->second.first.begin(), it->second.first.end(), flag) != it->second.first.end()) {
            return true;
        } else {
            WARN(WarningLevel::WARNING, "命令标志错误: %s", flag.c_str());
            return false;  // 如果标志无效，返回 false
        }
    }

    /**
     * @brief 执行指定的命令
     *
     * 该方法通过遍历传入的命令标志列表，依次验证每个标志的合法性。如果所有标志
     * 都合法，就会执行与该命令相关联的处理函数。
     *
     * @param cmd_name 要执行的命令名称
     * @param flags 命令的标志列表（例如：["-f", "-s"]）
     * @param parameters 命令的其他参数（这些参数会传递给命令的处理函数）
     */
    void execute_cmd(const std::string& cmd_name, const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        if (!verify_cmd(cmd_name, "")) return;  // 如果有无效名称，停止执行

        // 遍历所有标志，验证每个标志是否合法
        for (auto flag : flags) {
            if (!verify_cmd(cmd_name, flag)) return;  // 如果有无效标志，停止执行
        }
        // 调用命令的处理函数并执行命令
        command_table[cmd_name].second(flags, parameters);
    }

    /**
     * @brief 添加内建命令（例如 help, osinfo）
     *
     * 该方法将内建命令（如 `help`、`osinfo` 等）及其处理函数添加到命令表中。
     * 内建命令是预定义的，用户无需手动添加。
     */
    void builtin_cmd() {
        // 使用 std::bind 将 CMD_FUNC 类的成员函数与命令处理函数绑定
        add_cmd("help", {}, std::bind(&CMD_FUNC::help, &cmd_func, std::placeholders::_1, std::placeholders::_2));
        add_cmd("osinfo", {}, std::bind(&CMD_FUNC::osinfo, &cmd_func, std::placeholders::_1, std::placeholders::_2));
        add_cmd("test", {"-f", "-s"}, std::bind(&CMD_FUNC::test, &cmd_func, std::placeholders::_1, std::placeholders::_2));
        add_cmd("wifi_connect", {}, std::bind(&CMD_FUNC::wifi_connect, &cmd_func, std::placeholders::_1, std::placeholders::_2));

        // 切换当前工作目录
        add_cmd("cd", {}, std::bind(&FileExplorerShell::cd, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 打印当前工作目录
        add_cmd("pwd", {}, std::bind(&FileExplorerShell::pwd, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 查看目录内容
        add_cmd("ls", {}, std::bind(&FileExplorerShell::ls, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 以树状图列出目录的内容
        add_cmd("tree", {}, std::bind(&FileExplorerShell::tree, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 创建目录
        add_cmd("mkdir", {}, std::bind(&FileExplorerShell::mkdir, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 删除目录或文件
        add_cmd("rm", {}, std::bind(&FileExplorerShell::rm, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 移动或重命名目录和文件
        add_cmd("mv", {"-f", "-v"}, std::bind(&FileExplorerShell::mv, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 复制目录和文件
        add_cmd("cp", {"-f", "-v"}, std::bind(&FileExplorerShell::cp, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 创建空文件
        add_cmd("touch", {}, std::bind(&FileExplorerShell::touch, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 查看文件内容
        add_cmd("cat", {}, std::bind(&FileExplorerShell::cat, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 向文件写入单行文本
        add_cmd("echo", {"-w", "-a"}, std::bind(&FileExplorerShell::echo, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 向文件写入格式化文本
        add_cmd("printf", {"-w", "-a"}, std::bind(&FileExplorerShell::printf, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 精确查找文件和目录
        add_cmd("find", {}, std::bind(&FileExplorerShell::find, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));

        // 模糊搜索文件和目录
        add_cmd("search", {}, std::bind(&FileExplorerShell::search, &file_explorer_shell, std::placeholders::_1, std::placeholders::_2));
    }

    /**
     * @brief 打印当前命令表的内容
     *
     * 该方法用于输出当前命令表中的所有命令及其有效标志。如果命令表为空，
     * 则输出提示信息。否则，将以表格形式打印每个命令的名称和标志。
     */
    void print_commands_table() {
        Serial.println("\n============================= Command Table =============================");
        // 如果命令表为空，打印提示信息
        if (command_table.empty()) {
            Serial.println("The command table is empty.");
            return;
        }

        // 打印表头
        char header[100];  // 为表头预留缓冲区
        sprintf(header, "| %-20s | %-25s |", "Command Name", "Flags");
        Serial.println(header);
        Serial.println("|----------------------|---------------------------|");

        // 打印每个命令及其对应的标志
        for (const auto& entry : command_table) {
            char line[100];  // 为每一行命令内容预留缓冲区
            if (entry.second.first.empty()) {
                // 如果没有标志，则显示 "None"
                sprintf(line, "| %-20s | %-25s |", entry.first.c_str(), "None");
            } else {
                // 拼接标志字符串
                String flags;
                for (size_t i = 0; i < entry.second.first.size(); ++i) {
                    flags += entry.second.first[i].c_str();
                    if (i < entry.second.first.size() - 1) {
                        flags += ", ";  // 在标志之间加入逗号分隔符
                    }
                }
                sprintf(line, "| %-20s | %-25s |", entry.first.c_str(), flags.c_str());
            }
            Serial.println(line);  // 打印每个命令行
        }

        // 打印表尾
        Serial.println("=========================================================================");
    }

   private:
    CMD_FUNC cmd_func;                      ///< 内建命令的处理函数对象，例如 help, osinfo 等
    FileExplorerShell file_explorer_shell;  ///< 文件资源管理器内建命令的处理函数对象

    // 命令表：以命令名称为键，值是一个包含命令标志和处理函数的对
    std::unordered_map<std::string,
                       std::pair<std::vector<std::string>, std::function<void(const std::vector<std::string>&, const std::vector<std::string>&)>>>
        command_table;
};
