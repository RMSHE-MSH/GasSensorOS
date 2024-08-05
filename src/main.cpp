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
#include <data_table.hpp>
#include <fourier_transform.hpp>
#include <rivest_cipher_4.hpp>
#include <send_request.hpp>
#include <to_string.hpp>
#include <tree.hpp>
#include <wifi_connector.hpp>

GPIOs IO;
DataTable Table(6, 8);

void setup() {
    delay(3500);

    Serial.begin(115200);
    IO.gpioInit(2, GPIO_OUT_PP);
    IO.gpioResetBit(2);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Table.replaceCell(to_string(i + j), i, j);
        }
    }
    Serial.print(Table.getRowSize());
    Serial.print(",");
    Serial.print(Table.getColSize());
    Serial.print("\n");

    Table.printTable();
    Serial.print("\n");

    DataTable tab = Table.extractTable(0, 10, 2, 4);
    tab.printTable();
    Serial.print("\n");

    Table.replaceTable(tab, 0, 0);

    Table.printTable();
    Serial.print("\n");
}

void loop() {
    delay(3000);
    IO.gpioToggleBit(2);
}