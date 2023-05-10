/**
 * @file wifi_connector.hpp
 * @date 06.04.2023
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
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <rivest_cipher_4.hpp>
#include <string>

class CCDXlogin {
   public:
    CCDXlogin(std::string username, std::string password) : username_(username), password_(password) {}

    void getTimestamp() {
        http.begin(
            "http://1.1.1.2/");
        int httpResponseCode = http.GET();
        Serial.println(httpResponseCode);

        String contentType = http.header("Content-Type");
        String date = http.header("Date");
        Serial.print("Content-Type: ");
        Serial.println(contentType);
        Serial.print("date: ");
        Serial.println(date);

        http.end();
    }

    void sendPostRequest() {
        // timestamp = static_cast<std::string>(String(millis()).c_str());
        timestamp = "1680798927892";
        RivestCipher4 RC4(timestamp);
        password_rc4 = RC4.encrypt(password_);

        if (http.begin(client, url.c_str())) {
            http.addHeader("Content-Type", "application/json");
            http.addHeader("User-Agent", user_agent.c_str());

            std::string login_data =
                "{\"opr\":\"pwdLogin\",\"userName\":" + username_ + ",\"pwd\":" + password_rc4 + ",\"rememberPwd\":\"0\",\"auth_tag\":" + timestamp + "}";

            httpResponseCode = http.POST(login_data.c_str());

            Serial.printf("[HTTP] Code: %d\n", httpResponseCode);

            if (httpResponseCode == HTTP_CODE_OK) {
                response = static_cast<std::string>(http.getString().c_str());
            } else {
                error_code = static_cast<std::string>(http.errorToString(httpResponseCode).c_str());

                Serial.printf("[HTTP] POST Failed. Error: %s\n", http.errorToString(httpResponseCode).c_str());
            }
            http.end();
        }
    }

   private:
    WiFiClient client;
    HTTPClient http;

    int httpResponseCode;
    std::string response;
    std::string error_code;

    std::string username_;
    std::string password_;
    std::string password_rc4;
    std::string timestamp;

    std::string url = "http://1.1.1.2/ac_portal/login.php";
    std::string user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36";
};
