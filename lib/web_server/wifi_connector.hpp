/**
 * @file wifi_connector.hpp
 * @date 05.04.2023
 * @date Upgrade1.07.09.2025
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
#include <WiFi.h>
#include <file_explorer.h>

#include <cstdint>
#include <data_table.hpp>
#include <serial_warning.hpp>
#include <string>
#include <vector>

/**
 * @class WiFiConnector
 * @brief 管理Wi-Fi连接的类
 * @note 连接过程是阻塞的，在连接过程中设备将停止执行其他任务，直到成功连接或超时。
 */
class WiFiConnector {
   public:
    /**
     * @brief 构造函数，初始化Wi-Fi连接所需的参数
     *
     * @param ssid Wi-Fi名称
     * @param password Wi-Fi密码
     * @param timeout 连接超时时间，默认为10000ms。(若在此时间内未连接成功，则视为失败。)
     */
    WiFiConnector(const std::string ssid, const std::string password, uint32_t timeout = 10000) : ssid_(ssid), password_(password), timeout_(timeout) {}

    // 禁止拷贝和赋值（避免潜在的资源共享问题）
    WiFiConnector(const WiFiConnector&) = delete;
    WiFiConnector& operator=(const WiFiConnector&) = delete;

    ~WiFiConnector() {}

    /**
     * @brief 连接到指定的 Wi-Fi 网络
     *
     * 此方法将启动Wi-Fi连接过程，并在连接成功保存WIFI信息。
     *
     * @return true 如果连接成功
     * @return false 如果连接失败或超时
     */
    bool connect() {
        // 确保工作在 STA 模式并清除之前的连接状态（避免残留）
        WiFi.mode(WIFI_STA);
        WiFi.disconnect(true);

        WiFi.begin(ssid_.c_str(), password_.c_str());  // 启动Wi-Fi连接

        // 记录开始连接的时间点，用于后续的超时判断
        unsigned long startTime = millis();

        // 循环等待，直到 Wi-Fi 连接成功或超时
        while (WiFi.status() != WL_CONNECTED) {
            // 如果已超时，停止尝试并返回失败
            if (millis() - startTime >= timeout_) return false;

            // 每次检查之间短暂延时，以让出 CPU 时间给其他系统任务，避免空转。
            delay(CONNECTION_RETRY_DELAY_MS);
        }

        // Wi-Fi 连接成功
        saveWifi();   // 保存Wi-Fi信息到文件
        return true;  // 返回连接成功
    }

    // 断开当前Wi-Fi连接
    void disconnect() { WiFi.disconnect(); }

    /**
     * @brief 获取当前 Wi-Fi 状态
     * @return int
     */
    int getStatus() const { return WiFi.status(); }

    /**
     * @brief 获取设备当前在 Wi-Fi 网络中的 IP 地址。
     * @return std::string 以字符串形式返回本地 IP 地址。
     */
    std::string getIPAddress() const { return std::string(WiFi.localIP().toString().c_str()); }

   private:
    /**
     * @brief 将当前成功连接的 Wi-Fi 凭据保存到文件。
     * @details
     * 此函数负责将当前成功的 Wi-Fi 连接信息（SSID 和密码）写入到一个 CSV 文件中。
     * 逻辑：
     *  - 如果凭据文件不存在 -> 创建并写入（覆盖模式）
     *  - 如果凭据文件存在 -> 加载并查询是否已有相同 SSID
     *      - 若无相同 SSID -> 采用追加模式写入新条目（高效）
     *      - 若有相同 SSID 且密码不同 -> 在内存中更新表并写入临时文件，随后替换源文件
     *      - 若有相同 SSID 且密码相同 -> 不作任何操作
     */
    void saveWifi() {
        // 如果Wi-Fi列表文件不存在，则创建一个新的文件
        if (!file_.exists(wifi_list_path_)) {
            DataTable wifi_list(1, 2);                    // 初始化一个数据表。
            wifi_list.replaceRow({ssid_, password_}, 0);  // 添加Wi-Fi信息
            wifi_list.saveTable(wifi_list_path_, "w");    // 写入WIFI信息
            return;
        }

        // 如果Wi-Fi列表文件存在，则读取文件并检查是否已存在相同的Wi-Fi信息
        DataTable wifi_list(1, 2);             // 初始化一个数据表 用于加载Wi-Fi信息
        wifi_list.loadTable(wifi_list_path_);  // 从文件中加载Wi-Fi信息

        // 查询是否存在相同的Wi-Fi名称
        std::vector<std::pair<size_t, size_t>> locations = wifi_list.query(ssid_);

        // 查询是否存在相同的Wi-Fi名称
        if (locations.empty()) {
            // 如果不存在相同的Wi-Fi名称，则追加新的Wi-Fi信息
            DataTable new_wifi(1, 2);  // 创建一个只包含新条目的临时表
            new_wifi.replaceRow({ssid_, password_}, 0);
            new_wifi.saveTable(wifi_list_path_, "a");  // 追加WIFI信息
            return;
        }

        // 如果存在相同的Wi-Fi名称，则检查密码是否需要更新。
        size_t rowIndex = locations[0].first;
        // 查询密码是否发生更改。如果密码未更改，则不进行任何操作。
        if (wifi_list.getCell(rowIndex, 1) == password_) return;

        // 更新密码并保存文件(先写入临时文件，成功后再重命名，防止凭据丢失)
        wifi_list.replaceRow({ssid_, password_}, rowIndex);
        wifi_list.saveTable(wifi_list_temp_path_, "w");  // 保存更新后的Wi-Fi信息(临时文件)

        // 如果临时文件写入失败，报错后直接返回
        if (!file_.exists(wifi_list_temp_path_)) {
            WARN(WarningLevel::ERROR, "Failed to write WifiList file, path: ", wifi_list_temp_path_.c_str());
            return;
        }

        // 替换文件：删除旧文件并重命名临时文件
        file_.deletePath(wifi_list_path_);                        // 删除旧文件
        file_.renamePath(wifi_list_temp_path_, "wifi_list.csv");  // 重命名临时文件
    }

   private:
    const std::string ssid_;                                    // Wi-Fi网络名称
    const std::string password_;                                // Wi-Fi密码
    const uint32_t timeout_;                                    // 连接超时时间
    static constexpr uint16_t CONNECTION_RETRY_DELAY_MS = 500;  // 每次连接尝试之间的延迟时间

    FileExplorer file_;
    const std::string wifi_list_path_ = "/.os/wifi_list.csv";            // Wi-Fi列表文件路径
    const std::string wifi_list_temp_path_ = "/.os/wifi_list_temp.csv";  // Wi-Fi列表临时文件路径(用于安全替换)
};
