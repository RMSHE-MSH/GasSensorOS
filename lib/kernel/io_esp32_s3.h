/**
 * @file io_esp32_s3.h
 * @date 10.08.2024
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

#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/ledc.h>

#include <cstdint>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

constexpr bool ADC_OUT_RAW_VALUE = false;  // ADC模拟值输出
constexpr bool ADC_OUT_VOLTAGE = true;     // ADC电压输出

class ADC {
    friend class GPIOs;

   public:
    // 获取模拟引脚读取的模拟数值
    int32_t gpioReadRawValue();

    // 获取模拟引脚读取的电压
    uint32_t gpioReadVoltage();

    // 置模拟输出类型(可输出 模拟值[0, 4095] 或 电压[0, 3300]mv)
    void setAnalogOutputMode(bool mode = ADC_OUT_RAW_VALUE);

   private:
    // 读取模拟数值和电压
    void gpioReadAnalogBit(uint8_t pin);

    // 模拟引脚读取数值输出模式(可输出 模拟值[0, 4095](默认) 或 电压[0, 3300]mv)
    bool OUTPUT_MODE = ADC_OUT_RAW_VALUE;

    int32_t raw_value = 4095;  // ADC 数值(0-4095)
    uint32_t voltage = 3300;   // ADC 电压(0-3300mv)
};

class GPIOs : public ADC {
    friend class ADC;

   public:
    typedef struct GPIO_Type {
        int32_t status = -1;            // GPIO逻辑电平和模拟电压状态(-1表示未知)
        uint8_t tri_state = GPIO_NULL;  // 引脚三态
        uint8_t mode = GPIO_NULL;       // 引脚模式
        uint8_t io_type = GPIO_NULL;    // 引脚输入输出类型

        // 默认的无参数构造函数, 它将状态初始化为-1, 引脚三态, 模式和输入输出类型 初始化为GPIO_NULL;
        GPIO_Type() : status(-1), tri_state(GPIO_NULL), mode(GPIO_NULL), io_type(GPIO_NULL) {}
        // 有参数的构造函数，它允许将状态和模式以参数的形式传递给该构造函数，并将它们初始化为构造函数中的参数;
        GPIO_Type(int _status, uint8_t _tri_state, uint8_t _mode, uint8_t _io_type)
            : status(_status), tri_state(_tri_state), mode(_mode), io_type(_io_type) {}
    } GPIO_Type;

   public:
    // 初始化一个GPIO引脚
    void gpioInit(uint8_t pin, const std::unordered_set<uint8_t> &configs);

    // 替换指定引脚的 GPIO 配置
    void gpioReplaceConfig(uint8_t pin, const std::unordered_set<uint8_t> &configs);

    // 初始化一组GPIO引脚
    void gpioInitGroup(const std::unordered_set<uint8_t> &pins, const std::unordered_set<uint8_t> &configs);

    // 读取一个引脚的状态；
    int32_t gpioReadDataBit(uint8_t pin);

    // 读取一组引脚的状态；
    std::map<uint8_t, int32_t> gpioReadDataGroup();

    // 读取一个GPIO引脚的配置
    GPIO_Type gpioReadConfigBit(uint8_t pin);

    // 读取一组GPIO的工作模式
    std::unordered_map<uint8_t, GPIO_Type> gpioReadConfigGroup();

    // 对一个GPIO引脚进行写入操作
    void gpioWriteBit(uint8_t pin, uint8_t value);

    // 对一组GPIO引脚进行写入操作
    void gpioWriteGroup(std::unordered_map<uint8_t, uint8_t> value);

    // 将一个GPIO引脚置为高电平
    void gpioSetBit(uint8_t pin);

    // 将一个GPIO引脚置为低电平
    void gpioResetBit(uint8_t pin);

    // 反转一个GPIO引脚的电平
    void gpioToggleBit(uint8_t pin);

    // 反转一组输出GPIO的引脚电平
    void gpioToggleGroup();

    // 从一组GPIO中移除指定的引脚
    void gpioRemoveBit(uint8_t pin);

    // 清空一组GPIO；
    void gpioRemoveGroup();

   private:
    // 解析GPIO配置参数
    bool parse_GPIO_config(const std::unordered_set<uint8_t> &configs, uint8_t &tri_state, uint8_t &mode, uint8_t &io_type);

    // 验证 GPIO 配置的有效性
    bool validate_GPIO_config(uint8_t &tri_state, uint8_t &mode, uint8_t &io_type);

    // 应用配置到指定的GPIO引脚
    void set_GPIO_config(uint8_t &pin, uint8_t tri_state, uint8_t mode, uint8_t io_type);

    // 用来储存GPIO引脚对应的状态和配置(元素无序且唯一)
    std::unordered_map<uint8_t, GPIO_Type> GPIO_Registry;
};

constexpr uint32_t DUTY_CYCLE_MIN = 0;
constexpr uint32_t DUTY_CYCLE_MAX = 10000000;

class LEDCPWM {
   public:
    LEDCPWM(uint8_t pin = 2, ledc_timer_bit_t duty_resolution = LEDC_TIMER_13_BIT, uint32_t freq_hz = 5000, ledc_mode_t speed_mode = LEDC_LOW_SPEED_MODE,
            uint32_t duty = DUTY_CYCLE_MIN)
        : _pin(pin), _duty_resolution(duty_resolution), _freq_hz(freq_hz), _speed_mode(speed_mode), _duty(duty) {}

    // 初始化或修改LEDCPWM控制器的配置
    void pwmConfig(uint8_t pin, ledc_timer_bit_t duty_resolution, uint32_t freq_hz, ledc_mode_t speed_mode, uint32_t duty = DUTY_CYCLE_MIN);

    // 使能LEDCPWM控制器
    void pwmEnable();

    // 禁用LEDCPWM控制器
    void pwmDisable();

    // 调整PWM的占空比
    void updateDuty(uint32_t duty = DUTY_CYCLE_MIN);

    // 在给定时间内将当前占空比线性变换到目标占空比.
    void fadeWithTime(uint64_t duration_ms, uint32_t target_duty = DUTY_CYCLE_MAX, ledc_fade_mode_t fade_mode = LEDC_FADE_NO_WAIT);

    // 在给定步长和步数时将当前占空比线性变换到目标占空比.
    void fadeWithStep(uint32_t scale, uint32_t cycle_num, uint32_t target_duty = DUTY_CYCLE_MAX, ledc_fade_mode_t fade_mode = LEDC_FADE_NO_WAIT);

   private:
    uint8_t _pin;                       // PWM输出引脚
    ledc_timer_bit_t _duty_resolution;  // 占空比解析度
    uint32_t _freq_hz;                  // PWM频率
    ledc_mode_t _speed_mode;            // 速度模式
    uint32_t _duty;                     // 占空比

    ledc_timer_config_t ledc_timer;      // LEDC定时器配置结构体
    ledc_channel_config_t ledc_channel;  // LEDC通道配置结构体

    float TIMER_BIT_DUTY_CYCLE;
};