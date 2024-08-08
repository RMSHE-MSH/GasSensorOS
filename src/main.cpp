/**
 * @file main.cpp
 * @date 26.02.2023
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

#include <CPPSTL.h>
#include <esp32_pin_defi.h>
#include <io.h>
#include <kernel_tasks.h>
#include <random.h>
#include <systime.h>

#include <random>

// #include <code_interpreter.hpp>
#include <string_similarity_evaluator.h>
#include <string_strength_evaluator.h>

#include <data_table.hpp>
#include <fourier_transform.hpp>
#include <rivest_cipher_4.hpp>
#include <send_request.hpp>
#include <string_edit.hpp>
#include <tree.hpp>
#include <wifi_connector.hpp>

std::string str1 = "HelloWorld!";
std::string str2 = "HalloWord!";

GPIOs IO;
StringStrengthEvaluator SSE;
StringSimilarityEvaluator SsE(str1, str2);

void setup() {
    Serial.begin(115200);
    IO.gpioInit(2, GPIO_OUT_PP);
    IO.gpioResetBit(2);
}

void loop() {
    delay(3000);

    Serial.println(SSE.evaluateString(str1));
    Serial.println(SSE.evaluateString(str2));
    Serial.println(SsE.evaluateStringSimilarity());

    IO.gpioToggleBit(2);
}