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
#include <ESP32Ping.h>
#include <HTTPClient.h>
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
     * @param timeout 连接超时时间，默认为10000ms。(若在此时间内未连接成功，则视为失败。)
     */
    explicit WiFiConnector(uint32_t timeout = 10000) : timeout_(timeout) {}

    // 禁止拷贝和赋值（避免潜在的资源共享问题）
    WiFiConnector(const WiFiConnector&) = delete;
    WiFiConnector& operator=(const WiFiConnector&) = delete;

    ~WiFiConnector() {}

    /**
     * @brief 设置 Wi-Fi 连接的 SSID 和密码。
     * @param ssid 新的 Wi-Fi SSID。
     * @param password 新的 Wi-Fi 密码。
     */
    void setWifiCredentials(const std::string& ssid, const std::string& password);

    /**
     * @brief 设置 WiFi 连接的超时时间。
     * @param timeout 连接超时时间，默认为10000ms。(若在此时间内未连接成功，则视为失败。)
     */
    void setTimeout(uint32_t timeout);

    /**
     * @brief 连接到指定的 Wi-Fi 网络
     *
     * 此方法将启动Wi-Fi连接过程，并在连接成功保存WIFI信息。
     *
     * @return true 如果连接成功
     * @return false 如果连接失败或超时
     */
    bool connect();

    // 断开当前Wi-Fi连接
    void disconnect();

    /**
     * @brief 获取当前 Wi-Fi 状态
     * @return int
     */
    int getStatus() const;

    /**
     * @brief 获取设备当前在 Wi-Fi 网络中的 IP 地址。
     * @return std::string 以字符串形式返回本地 IP 地址。
     */
    std::string getIPAddress() const;

    /**
     * @brief 检测网络是否可用。
     * @details
     *        它首先检查本地 Wi-Fi 是否已连接。
     *        然后，根据输入目标是 URL 还是主机/IP，自动选择合适的检测方法：
     *        - 如果目标以 "http://" 或 "https://" 开头，则调用 checkHttpConnectivity()。
     *        - 否则，视为 主机名或 IP 地址，调用 checkPingConnectivity()。
     *
     * @param target 要检测的目标。默认为 "8.8.8.8"。
     *
     * @return true 网络连通性测试成功。
     * @return false Wi-Fi 未连接，或与目标的连通性测试失败。
     */
    bool checkNetwork(const std::string& target = "8.8.8.8");

    /**
     * @brief 自动连接到可用的Wi-Fi网络。
     * @details
     *        首先检查当前网络连接状态。如果已连接，则直接返回true。
     *        如果未连接，则尝试从预设的Wi-Fi列表文件中读取凭据，并依次尝试连接。
     *        当成功连接到一个Wi-Fi网络并能访问互联网时，停止尝试并返回true。
     * @return true 如果成功连接到互联网。
     * @return false 如果无法连接到任何有效的Wi-Fi网络。
     */
    bool autoConnect();

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
        // 快速返回：SSID 为空不执行任何持久化，避免产生无效记录
        if (ssid_.empty()) {
            WARN(WarningLevel::INFO, "Skip saving Wi-Fi because SSID is empty.");
            return;
        }

        // 统一的 WifiList 保存步骤（写临时文件 -> 校验 -> 原子替换）
        // 使用 lambda 局部封装，避免重复代码，同时便于将来扩展（例如增加校验、备份等）
        auto writeWifiList = [this](DataTable& table) -> bool {
            // 写入临时文件
            table.saveTable(wifi_list_temp_path_, "w");
            if (!file_.exists(wifi_list_temp_path_)) {
                WARN(WarningLevel::ERROR, "Failed to write WifiList file, path: %s", wifi_list_temp_path_.c_str());
                return false;
            }

            // 删除旧文件（存在则删）
            if (file_.exists(wifi_list_path_)) file_.deletePath(wifi_list_path_);

            // 原子替换：重命名临时文件为正式文件
            file_.renamePath(wifi_list_temp_path_, "wifi_list.csv");
            return true;
        };

        // 加载或初始化凭据表
        DataTable wifi_list(1, 2);
        if (!file_.exists(wifi_list_path_)) {
            // 凭据文件不存在：创建并写入首条记录
            wifi_list.replaceRow({ssid_, password_}, 0);
            writeWifiList(wifi_list);
            return;
        }

        // 文件存在：加载并处理
        wifi_list.loadTable(wifi_list_path_);

        // 查询是否已有相同 SSID
        std::vector<std::pair<size_t, size_t>> locations = wifi_list.query(ssid_);

        if (locations.empty()) {
            // 无相同 SSID：在第一行插入新条目
            wifi_list.insertRow({ssid_, password_}, 0);
            writeWifiList(wifi_list);
            return;
        }

        // 已存在相同 SSID，定位相同 SSID 的所在行
        size_t rowIndex = locations[0].first;

        // 如已在第一行则避免不必要的闪存写入；否则仅上移以提升优先级
        if (rowIndex == 0) return;

        // 将凭据上移到第一行（保持“最近成功连接优先”）
        wifi_list.deleteRow(rowIndex);
        wifi_list.insertRow({ssid_, password_}, 0);

        writeWifiList(wifi_list);
    }

    /**
     * @brief 使用 ICMP Ping 协议检测与指定主机的网络连通性。
     * @details 此函数向目标主机发送指定数量的 ICMP Echo 请求（Ping包），并等待响应。
     *
     * @param host 要 Ping 的目标主机名或 IP 地址 (例如 "www.google.com" 或 "8.8.8.8")
     * @param count 要发送的 Ping 包数量。默认为 4
     *
     * @return true 至少收到一个有效的 Ping 回复。
     * @return false Ping 操作失败。
     */
    bool checkPingConnectivity(const std::string& host, uint8_t count = 4) {
        if (Ping.ping(host.c_str(), count)) {
            return true;
        } else {
            WARN(WarningLevel::WARNING, "Ping test failed for host: ", host.c_str());
            return false;
        }
    }

    /**
     * @brief 使用 HTTP GET 请求检测与指定 URL 的网络连通性。
     * @details
     *        此函数尝试对给定的 URL 发起一个 HTTP GET 请求。
     *        只要能成功建立连接并收到任何有效的 HTTP 响应状态码（大于0），即视为连通。
     *
     * @param url 要测试的完整 URL (例如 "http://www.baidu.com")。
     * @return true HTTP GET 请求成功并获得有效的响应码。
     * @return false 请求失败。
     */
    bool checkHttpConnectivity(const std::string& url) {
        HTTPClient http;
        http.begin(url.c_str());
        const int httpCode = http.GET();

        // httpCode > 0 表示收到了有效的 HTTP 状态码 (如 200, 301, 404 等)。
        // httpCode < 0 表示客户端级别的错误 (如 -1 连接失败, -2 发送头失败等)。
        // 因此，只要 httpCode > 0，就意味着网络链路是通的，即使页面是 404 Not Found。
        if (httpCode > 0) {
            http.end();
            return true;
        } else {
            WARN(WarningLevel::WARNING, "HTTP test failed. URL: ", url.c_str(), ", Error: ", http.errorToString(httpCode).c_str());
            http.end();
            return false;
        }
    }

    /**
     * @brief 尝试连接到指定的Wi-Fi网络并检查互联网连接。
     * @param ssid Wi-Fi网络的SSID。
     * @param password Wi-Fi网络的密码。
     * @return true 成功连接到Wi-Fi并有互联网访问。
     * @return false 无法连接到Wi-Fi或无法访问互联网。
     */
    bool tryAndCheckConnect(const std::string& ssid, const std::string& password) {
        setWifiCredentials(ssid, password);  // 设置Wi-Fi凭据

        // 尝试连接Wi-Fi
        if (!connect()) {
            // Wi-Fi连接失败，断开当前连接，以便尝试下一个Wi-Fi
            disconnect();
            return false;
        }
        // 如果Wi-Fi连接成功，进一步检查互联网连接
        if (!checkNetwork()) {
            // 无互联网访问，断开当前连接，以便尝试下一个Wi-Fi
            disconnect();
            return false;
        }
        // 成功连接到Wi-Fi并有互联网访问
        return true;
    }

   private:
    std::string ssid_;                                          // Wi-Fi网络名称
    std::string password_;                                      // Wi-Fi密码
    uint32_t timeout_;                                          // 连接超时时间
    static constexpr uint16_t CONNECTION_RETRY_DELAY_MS = 500;  // 每次连接尝试之间的延迟时间

    FileExplorer file_;
    const std::string wifi_list_path_ = "/.os/wifi_list.csv";            // Wi-Fi列表文件路径
    const std::string wifi_list_temp_path_ = "/.os/wifi_list_temp.csv";  // Wi-Fi列表临时文件路径(用于安全替换)
};

inline void WiFiConnector::setWifiCredentials(const std::string& ssid, const std::string& password) {
    ssid_ = ssid;
    password_ = password;
}

inline void WiFiConnector::setTimeout(uint32_t timeout) { timeout_ = timeout; }

inline bool WiFiConnector::connect() {
    // 确保工作在 STA 模式并清除之前的连接状态（避免残留）
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);

    WiFi.begin(ssid_.c_str(), password_.c_str());  // 启动Wi-Fi连接

    // 记录开始连接的时间点，用于后续的超时判断
    unsigned long startTime = millis();

    // 循环等待，直到 Wi-Fi 连接成功或超时
    while (WiFi.status() != WL_CONNECTED) {
        // 如果已超时，停止尝试并返回失败
        if (millis() - startTime >= timeout_) {
            WARN(WarningLevel::WARNING, "WiFi connection timed out.");
            return false;
        }

        // 每次检查之间短暂延时。
        delay(CONNECTION_RETRY_DELAY_MS);
    }

    // Wi-Fi 连接成功
    saveWifi();   // 保存Wi-Fi信息到文件
    return true;  // 返回连接成功
}

inline void WiFiConnector::disconnect() { WiFi.disconnect(); }

inline int WiFiConnector::getStatus() const { return WiFi.status(); }

inline std::string WiFiConnector::getIPAddress() const { return std::string(WiFi.localIP().toString().c_str()); }

inline bool WiFiConnector::checkNetwork(const std::string& target) {
    // 1. 前置条件检查：确认基础网络连接（Wi-Fi）已就绪。
    if (WiFi.status() != WL_CONNECTED) {
        WARN(WarningLevel::WARNING, "Wi-Fi is not connected. Current status: %d", WiFi.status());
        return false;
    }

    // 2. 分发：根据目标格式，选择合适的检测策略。
    // 使用 find 检查前缀是更惯用的方法。
    if (target.find("http://") == 0 || target.find("https://") == 0) {
        // 输入是 URL，委托给 HTTP 检测函数。
        return checkHttpConnectivity(target);
    } else {
        // 输入是 主机/IP，委托给 Ping 检测函数。
        return checkPingConnectivity(target);
    }
}

/**
 * @brief 自动连接到可用的Wi-Fi网络。
 * @details
 *        此函数首先检查设备当前的网络连接状态。如果设备已经连接到互联网，则直接返回成功。
 *        如果未连接，它将尝试从预设的Wi-Fi列表文件中读取存储的Wi-Fi凭据（SSID和密码）。
 *        然后，它会遍历列表中的每个Wi-Fi网络，依次尝试连接。
 *        对于每个尝试的Wi-Fi网络，它会先尝试建立Wi-Fi连接，如果连接成功，则进一步检查是否能够访问互联网。
 *        一旦成功连接到一个Wi-Fi网络并且能够访问互联网，函数将停止尝试剩余的SSID并返回true。
 *        如果在遍历完所有Wi-Fi列表中的凭据后，仍然无法连接到互联网，则返回false。
 * @return true 如果成功连接到互联网。
 * @return false 如果无法连接到任何有效的Wi-Fi网络。
 * @note 此函数是阻塞的，在尝试连接每个Wi-Fi网络时会等待连接结果。
 */
inline bool WiFiConnector::autoConnect() {
    // 1. 检查当前网络连接状态
    if (checkNetwork()) return true;

    // 2. 读取Wi-Fi列表文件
    DataTable wifi_list(1, 2);  // 初始化一个数据表，用于存储从文件中加载的Wi-Fi凭据
    wifi_list.loadTable(wifi_list_path_);

    // 3. 遍历Wi-Fi列表并尝试连接
    for (size_t i = 0; i < wifi_list.getRowSize(); ++i) {
        std::string current_ssid = wifi_list.getCell(i, 0);
        std::string current_password = wifi_list.getCell(i, 1);

        // 检查SSID是否为空，跳过空条目以避免无效连接尝试
        if (current_ssid.empty()) continue;

        // 尝试连接当前Wi-Fi并检查互联网连接
        if (tryAndCheckConnect(current_ssid, current_password)) {
            return true;  // 成功连接并能访问互联网，停止尝试
        }
    }

    // 遍历完所有Wi-Fi仍无法连接到互联网
    return false;
}
