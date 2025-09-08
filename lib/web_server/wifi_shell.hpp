/**
 * @file wifi_shell.hpp
 * @date 07.09.2025
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
#include <cstdint>
#include <string>
#include <wifi_connector.hpp>

class WifiShell {
   public:
    /**
     * @brief 连接到指定的 Wi-Fi 网络。
     * @param flags 命令标志位（未使用）
     * @param parameters 命令参数: SSID, PASSWORD, timeout
     * @note wifi_connect [SSID] [PASSWORD] [timeout]
     */
    void wifi_connect(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // 自动连接到可用的Wi-Fi网络
        if (parameters.empty()) {
            // 如果没有提供参数，则将自动尝试连接所有已保存的Wi-Fi凭据。
            if (!WIFI.autoConnect()) {
                WARN(WarningLevel::ERROR, "Auto-connect to Wi-Fi failed.");
            }
            return;
        }

        // 如果提供了SSID和PASSWORD，则按指定参数连接
        uint32_t timeout = 10000;  // 默认超时时间10秒

        // 若提供第三个参数，则解析为超时时间
        if (parameters.size() == 3) {
            timeout = static_cast<uint32_t>(std::stoul(parameters[2]));
            WIFI.setTimeout(timeout);
        }

        // 设置 Wi-Fi 凭据并连接
        WIFI.setWifiCredentials(parameters[0], parameters[1]);
        WIFI.connect();
    }

    /**
     * @brief 测试网络的连接性
     * @param flags 命令标志位（未使用）
     * @param parameters 命令参数: target(URL or IP)
     * @note ping [target]
     *       - target: 一个 URL (例如, http://www.baidu.com) 或一个主机/IP (例如, 8.8.8.8)。
     */
    void ping(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        std::string target = "8.8.8.8";  // 检测的目标。默认为 "8.8.8.8"。

        // 若用户提供 target 参数, 则使用用户提供值
        if (!parameters.empty()) target = parameters[0];

        if (WIFI.checkNetwork(target)) {
            std::cerr << "Ping successful: " << target << "\n";
        } else {
            WARN(WarningLevel::ERROR, "Ping failed: %s", target.c_str());
        }
    }

   private:
    WiFiConnector WIFI;  // 创建 WiFiConnector 对象
};
