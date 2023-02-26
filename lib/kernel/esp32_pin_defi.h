/**
 * @file esp32_pin_defi.h
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

#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

constexpr uint8_t INPUT_OK = 0xff;   // 可以使用来作为输入引脚
constexpr uint8_t OUTPUT_OK = 0xfe;  // 可以使用来作为输出引脚
constexpr uint8_t ADC1_OK = 0xfd;    // 可以使用来作为模拟输入引脚(SAR ADC1)
constexpr uint8_t ADC2_OK = 0xfc;    // 可以使用来作为模拟输入引脚(SAR ADC2)
constexpr uint8_t PWM_OK = 0xfb;     // 该引脚支持硬件PWM

// 引脚三态
constexpr uint8_t GPIO_OD = 0x10;  // 引脚开漏
constexpr uint8_t GPIO_PU = 0x04;  // 引脚上拉
constexpr uint8_t GPIO_PD = 0x08;  // 引脚下拉

// 输出类型
constexpr uint8_t GPIO_OUT_PP = 0x02;  // 推挽输出
constexpr uint8_t GPIO_OUT_OD = 0x12;  // 开漏输出

// 输入类型
constexpr uint8_t GPIO_IN_FLOATING = 0x01;  // 浮空输入
constexpr uint8_t GPIO_IN_PU = 0x05;        // 上拉输入
constexpr uint8_t GPIO_IN_PD = 0x09;        // 下拉输入

constexpr uint8_t GPIO_IN_ANALOG = 0xC0;  // 模拟输入