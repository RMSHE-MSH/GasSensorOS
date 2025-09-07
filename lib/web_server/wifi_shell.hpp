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
     * @brief 连接到指定WIFI
     *
     * @param flags 命令标志位（未使用）
     * @param parameters 命令参数: SSID, PASSWORD, timeout
     * @note wifi_connect <SSID> <PASSWORD> [timeout]
     */
    void wifi_connect(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        uint32_t timeout = 10000;  // 默认超时时间10秒

        // 若提供第三个参数，则解析为超时时间
        if (parameters.size() == 3) {
            timeout = static_cast<uint32_t>(std::stoul(parameters[2]));
        }

        // 创建 WiFiConnector 对象并连接
        WiFiConnector WIFI(parameters[0].c_str(), parameters[1].c_str(), timeout);
        WIFI.connect();
    }

   private:
};