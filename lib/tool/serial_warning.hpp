/**
 * @file serial_warning.h
 * @date 16.08.2024
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2024 RMSHE. All rights reserved.
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

// 定义警告等级枚举，用于表示不同级别的日志信息
enum class WarningLevel {
    DEBUG,    // 调试信息，最详细的日志级别
    INFO,     // 普通信息，用于记录常规操作
    WARNING,  // 警告信息，用于提醒可能存在的问题
    ERROR,    // 错误信息，用于记录程序中的错误
    CRITICAL  // 严重错误信息，表示严重的问题需要立即关注
};

// 根据警告等级返回对应的字符串表示，用于在日志输出中显示
const char* getWarningLevelString(WarningLevel level);

// 全局设置结构体，用于存储日志系统的配置
struct DebugSettings {
    // 定义日志级别阈值，默认设置为DEBUG，意味着输出所有级别的日志信息
    WarningLevel logLevelThreshold = WarningLevel::DEBUG;
};

extern DebugSettings debugSettings;  // 声明全局设置变量

// 输出警告信息到串口
void outputWarning(const char* message);

// 日志记录的模板函数，用于格式化并输出警告信息
// 该函数接受可变参数，以便支持多种格式化字符串
template <typename... Args>
void debugWarning(WarningLevel level, const char* file, int line, const char* format, Args... args) {
    // 检查当前警告等级是否低于日志级别阈值，如果是则不输出
    if (level < debugSettings.logLevelThreshold) return;

    // 定义一个静态分配的字符数组作为缓冲区，避免动态内存分配，提高性能
    char buffer[256];

    // 将警告信息格式化为: "文件路径:行号: 警告等级: " 的形式
    int n = snprintf(buffer, sizeof(buffer), "%s:%d: %s: ", file, line, getWarningLevelString(level));

    // 将用户传递的格式化字符串追加到缓冲区中
    snprintf(buffer + n, sizeof(buffer) - n, format, args...);

    // 将格式化后的警告信息输出到串口
    outputWarning(buffer);
}

// 定义一个方便使用的宏，用于快速插入警告信息
// 该宏会自动传递当前的文件名和行号，简化使用
#define WARN(level, format, ...) debugWarning(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
