/**
 * @file serial_warning.cpp
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

#include <Arduino.h>

#include <serial_warning.hpp>

// 根据警告等级返回对应的字符串表示，用于在日志输出中显示
const char* getWarningLevelString(WarningLevel level) {
    switch (level) {
        case WarningLevel::DEBUG:
            return "DEBUG";  // 返回"DEBUG"字符串
        case WarningLevel::INFO:
            return "INFO";  // 返回"INFO"字符串
        case WarningLevel::WARNING:
            return "WARNING";  // 返回"WARNING"字符串
        case WarningLevel::ERROR:
            return "ERROR";  // 返回"ERROR"字符串
        case WarningLevel::CRITICAL:
            return "CRITICAL";  // 返回"CRITICAL"字符串
        default:
            return "UNKNOWN";  // 默认情况，返回"UNKNOWN"
    }
}

DebugSettings debugSettings;

// 输出警告信息到串口
void outputWarning(const char* message) { Serial.println(message); }

/*测试代码
#include <Arduino.h>

#include "serial_warning.h"

void setup() {
    // 初始化串口通信，设置波特率为115200
    Serial.begin(115200);

    delay(3000);

    // 设置日志级别阈值
    debugSettings.logLevelThreshold = WarningLevel::INFO;

    // 测试警告系统，以下为不同级别的日志信息示例
    WARN(WarningLevel::DEBUG, "这是一个调试信息");
    WARN(WarningLevel::INFO, "初始化完成");
    WARN(WarningLevel::WARNING, "可能存在问题");
    WARN(WarningLevel::ERROR, "发生了一个错误, errCode: %d", 114514);
    WARN(WarningLevel::CRITICAL, "严重错误");
}

void loop() {
    // 主循环
}
*/