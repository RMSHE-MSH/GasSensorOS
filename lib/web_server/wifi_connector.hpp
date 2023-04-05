/**
 * @file wifi_connector.hpp
 * @date 05.04.2023
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
#include <WiFi.h>

// WiFi连接类
class WiFiConnector {
   public:
    // 定义回调函数类型，用于连接成功和连接失败的回调
    using OnConnectCallback = void (*)(const char* ssid, const IPAddress& ip);
    using OnFailCallback = void (*)(const char* ssid);

    // 构造函数，初始化参数，包括Wi-Fi网络名称、密码、是否打印连接报告、连接超时时间和连接成功和连接失败的回调函数
    WiFiConnector(const char* ssid, const char* password, bool print_report = false, uint32_t timeout = 10000, OnConnectCallback on_connect = nullptr,
                  OnFailCallback on_fail = nullptr)
        : ssid_(ssid), password_(password), print_report_(print_report), timeout_(timeout), on_connect_(on_connect), on_fail_(on_fail) {}

    // 连接Wi-Fi
    bool connect() {
        // 调用WiFi.begin()函数开始连接
        WiFi.begin(ssid_, password_);

        // 等待连接成功或超时
        unsigned long start_time = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start_time < timeout_) delay(connection_delay_);

        // 如果成功连接
        if (WiFi.status() == WL_CONNECTED) {
            // 获取IP地址
            IPAddress ip = WiFi.localIP();

            // 如果打印连接报告
            if (print_report_) {
                // 打印Wi-Fi名称和IP地址
                Serial.print(F("Connected to Wi-Fi: "));
                Serial.print(ssid_);
                Serial.print(F(", IP address: "));
                Serial.println(ip);
            }

            // 如果连接成功的回调函数不为空，调用回调函数
            if (on_connect_ != nullptr) on_connect_(ssid_, ip);

            // 返回连接成功
            return true;
        }

        // 如果连接失败
        if (print_report_) {
            // 打印Wi-Fi名称
            Serial.print(F("Failed to connect to Wi-Fi: "));
            Serial.println(ssid_);
        }

        // 如果连接失败的回调函数不为空，调用回调函数
        if (on_fail_ != nullptr) on_fail_(ssid_);

        // 返回连接失败
        return false;
    }

   private:
    const char* ssid_;                                   // Wi-Fi名称
    const char* password_;                               // Wi-Fi密码
    const bool print_report_;                            // 是否打印连接报告
    const uint32_t timeout_;                             // 连接超时时间
    static constexpr uint16_t connection_delay_ = 1000;  // 连接失败时的延迟
    OnConnectCallback on_connect_;                       // 连接成功的回调函数
    OnFailCallback on_fail_;                             // 连接失败的回调函数
};
