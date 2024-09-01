/**
 * @file esp32_s3_pin_defi.h
 * @date 14.08.2024
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

#pragma once

#include <cstdint>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

// ADC 功能引脚标识
constexpr uint8_t ADC1 = 0xFD;  // 可以使用来作为模拟输入引脚(12-bit SAR ADC1)
constexpr uint8_t ADC2 = 0xFC;  // 可以使用来作为模拟输入引脚(12-bit SAR ADC2)

// 电容式触摸传感器引脚标识
constexpr uint8_t TOUCH1 = 0x11;
constexpr uint8_t TOUCH2 = 0x12;
constexpr uint8_t TOUCH3 = 0x13;
constexpr uint8_t TOUCH4 = 0x14;
constexpr uint8_t TOUCH5 = 0x15;
constexpr uint8_t TOUCH6 = 0x16;
constexpr uint8_t TOUCH7 = 0x17;
constexpr uint8_t TOUCH8 = 0x18;
constexpr uint8_t TOUCH9 = 0x19;
constexpr uint8_t TOUCH10 = 0x1A;
constexpr uint8_t TOUCH11 = 0x1B;
constexpr uint8_t TOUCH12 = 0x1C;
constexpr uint8_t TOUCH13 = 0x1D;
constexpr uint8_t TOUCH14 = 0x1E;

// 通信接口引脚标识
constexpr uint8_t UART = 0x1F;        // 支持 UART 通信接口
constexpr uint8_t I2C = 0x20;         // 支持 I2C 通信接口
constexpr uint8_t I2S = 0x21;         // 支持 I2S 通信接口
constexpr uint8_t LCD_CAMERA = 0x22;  // 支持 LCD 和摄像头接口
constexpr uint8_t RMT = 0x23;         // 支持红外遥控(RMT)信号的发射和接收

// SPI 功能引脚标识
constexpr uint8_t SPI0 = 0x24;  // 供 ESP32-S3 和加密 DMA (EDMA) 访问封装内或封装外 flash/PSRAM
constexpr uint8_t SPI1 = 0x25;  // 供 CPU 访问封装内或封装外 flash/PSRAM
constexpr uint8_t SPI2 = 0x26;  // 通用 SPI 控制器,具有单独的 DMA 通道
constexpr uint8_t SPI3 = 0x27;  // 通用 SPI 控制器,和部分外设共用一个 DMA 通道

constexpr uint8_t PCNT_SIG = 0x28;  // 脉冲计数器: 通过七种模式 捕捉脉冲并对脉冲边沿计数

constexpr uint8_t USB_OTG = 0x29;  // 支持 USB OTG 功能

constexpr uint8_t SDHOST = 0x2A;  // 支持 V3.0.1 标准 SD 内存卡

constexpr uint8_t PWM = 0xFB;  // 引脚支持硬件 PWM

constexpr uint8_t TWAI = 0x2B;  // 支持双线车载串口

// 受限或不推荐使用的引脚
constexpr uint8_t RESTRICTED = 0x2C;    // 受限制使用的管脚,一般具有重要功能,按需求调整使用
constexpr uint8_t NO_RECOMMEND = 0x2D;  // 不建议使用的管脚(用于与封装内 flash/PSRAM 通讯,不建议作其他用途)

constexpr uint8_t GPIO_NULL = 0xFF;  // 引脚禁用或无功能

// 引脚三态标识(tri_state)
constexpr uint8_t GPIO_PU = 0x30;     // 引脚上拉 (Pull-Up)
constexpr uint8_t GPIO_PD = 0x40;     // 引脚下拉 (Pull-Down)
constexpr uint8_t GPIO_FLOAT = 0x80;  // 引脚浮空 (No Pull)

// 引脚模式标识(mode)
constexpr uint8_t GPIO_PP = 0x00;      // 推挽模式 (Push-Pull)
constexpr uint8_t GPIO_OD = 0x10;      // 开漏模式 (Open-Drain)
constexpr uint8_t GPIO_ANALOG = 0xC0;  // 模拟模式 (Analog)
constexpr uint8_t GPIO_INTR = 0xC1;    // 中断模式 (Interrupt)

// 输入输出类型标识(io_type)
constexpr uint8_t GPIO_OUT = 0x02;     // 仅输出 (Output only)
constexpr uint8_t GPIO_IN = 0x01;      // 仅输入 (Input only)
constexpr uint8_t GPIO_IN_OUT = 0x03;  // 输入输出 (Input and Output)

// 自定义哈希函数，用于处理 std::unordered_set 中的集合哈希
// 此结构体用于生成一个哈希值，供unordered_set使用，以确保可以正确地处理集合中的元素组合
struct UnorderedSetHash {
    // 重载operator()，用于为unordered_set计算哈希值
    std::size_t operator()(const std::unordered_set<uint8_t> &s) const {
        std::size_t hash = 0;  // 初始化哈希值为0
        // 遍历集合中的每个元素，组合其哈希值
        for (uint8_t val : s) {
            // 使用了Boost库中的一个推荐哈希组合方法
            // 其中 0x9e3779b9 是一个魔术数（黄金分割比的整数部分），用于减少哈希冲突
            hash ^= std::hash<uint8_t>{}(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        // 返回最终生成的哈希值
        return hash;
    }
};

// 这些是被禁用的 GPIO 配置组合
const std::unordered_set<std::unordered_set<uint8_t>, UnorderedSetHash> invalid_GPIO_configs = {
    {GPIO_PD, GPIO_PP, GPIO_OUT},         // 下拉电阻 + 推挽模式 + 输出
    {GPIO_PD, GPIO_OD, GPIO_OUT},         // 下拉电阻 + 开漏模式 + 输出
    {GPIO_PD, GPIO_ANALOG, GPIO_OUT},     // 下拉电阻 + 模拟模式 + 输出
    {GPIO_PU, GPIO_ANALOG, GPIO_OUT},     // 上拉电阻 + 模拟模式 + 输出
    {GPIO_FLOAT, GPIO_ANALOG, GPIO_OUT},  // 浮空 + 模拟模式 + 输出
    {GPIO_ANALOG, GPIO_OUT},              // 模拟模式 + 输出
    {GPIO_ANALOG, GPIO_IN},               // 模拟模式 + 输入
    {GPIO_INTR, GPIO_OUT},                // 中断模式 + 输出
    // 添加更多的组合（可以扩展此列表以涵盖更多的配置组合）
};