/**
 * @file io_esp32_s3.cpp
 * @date 10.08.2024
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

#include <driver/adc.h>
#include <esp32_s3_pin_defi.h>
#include <esp_adc_cal.h>
#include <io_esp32_s3.h>

#include <serial_warning.hpp>

// ESP32-S3-WROOM-1引脚功能定义
const std::unordered_map<uint8_t, std::unordered_set<uint8_t>> esp32_s3_wroom_1_gpios = {
    {0, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {1, {ADC1, TOUCH1, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {2, {ADC1, TOUCH2, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {3, {ADC1, TOUCH3, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {4, {ADC1, TOUCH4, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {5, {ADC1, TOUCH5, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {6, {ADC1, TOUCH6, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {7, {ADC1, TOUCH7, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {8, {ADC1, TOUCH8, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {9, {ADC1, TOUCH9, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {10, {ADC1, TOUCH10, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {11, {ADC2, TOUCH11, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {12, {ADC2, TOUCH12, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {13, {ADC2, TOUCH13, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {14, {ADC2, TOUCH14, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {15, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {16, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {17, {ADC2, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {18, {ADC2, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {19, {ADC2, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {20, {ADC2, UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {21, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {35, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI0, SPI1, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {36, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI0, SPI1, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {37, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI0, SPI1, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {38, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {39, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {40, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {41, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {42, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {43, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {44, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {45, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {46, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {47, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
    {48, {UART, I2C, PWM, I2S, LCD_CAMERA, RMT, SPI2, SPI3, PCNT_SIG, SDHOST, TWAI}},
};

// 定义了 ESP32-WROOM-32E GPIO管脚与ADC2通道的对应关系.
const std::unordered_map<uint8_t, adc2_channel_t> gpio_to_adc2_channel = {
    {4, ADC2_CHANNEL_0},  {0, ADC2_CHANNEL_1},  {2, ADC2_CHANNEL_2},  {15, ADC2_CHANNEL_3}, {13, ADC2_CHANNEL_4},
    {12, ADC2_CHANNEL_5}, {14, ADC2_CHANNEL_6}, {27, ADC2_CHANNEL_7}, {25, ADC2_CHANNEL_8}, {26, ADC2_CHANNEL_9}};

// 定义了 ESP32-WROOM-32E GPIO管脚与ADC1通道的对应关系.
const std::unordered_map<uint8_t, adc1_channel_t> gpio_to_adc1_channel = {
    {36, ADC1_CHANNEL_0}, {37, ADC1_CHANNEL_1}, {38, ADC1_CHANNEL_2}, {39, ADC1_CHANNEL_3},
    {32, ADC1_CHANNEL_4}, {33, ADC1_CHANNEL_5}, {34, ADC1_CHANNEL_6}, {35, ADC1_CHANNEL_7},
};

/**
 * @brief 获取模拟引脚读取的模拟数值
 * @return 模拟数值[0, 4095]
 */
int32_t ADC::gpioReadRawValue() { return raw_value; };

/**
 * @brief 获取模拟引脚读取的电压
 * @return 电压[0, 3300]mv
 */
uint32_t ADC::gpioReadVoltage() { return voltage; };

/**
 * @brief 设置模拟输出类型(可输出 模拟数值[0, 4095] 或 电压[0, 3300]mv)
 * @param mode: ADC_OUT_RAW_VALUE(模拟数值/默认) | ADC_OUT_VOLTAGE(电压)
 * @return 无
 */
void ADC::setAnalogOutputMode(bool mode) { OUTPUT_MODE = mode; }

/**
 * @brief 读取模拟数值和电压
 * @param pin: 拥有 ADC1 和 ADC2 单元的GPIO引脚
 * @return void
 */
void ADC::gpioReadAnalogBit(uint8_t pin) {
    raw_value = 4095;  // ADC 数值(0-4095)
    voltage = 3300;    // ADC 电压(0-3300mv)

    // 根据读出的电压值选择合适的衰减系数, 目的是增加ADC的准确度；
    auto adc_auto_atten = [](uint32_t _voltage) -> adc_atten_t {
        if (_voltage <= 1000)
            return ADC_ATTEN_DB_0;  // 电压为1V时最准确;
        else if (_voltage <= 1340)
            return ADC_ATTEN_DB_2_5;  // 电压为1.34V时最准确;
        else if (_voltage <= 2000)
            return ADC_ATTEN_DB_6;  // 电压为2V时最准确;
        else
            return ADC_ATTEN_DB_11;  // 电压为3.3V时最准确;
    };

    // 创建一个ADC校准参数对象
    static esp_adc_cal_characteristics_t *adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));

    // 判断用户使用的GPIO管脚属于哪一个ADC单元(ESP32两个ADC单元：ADC_UNIT_1(SAR ADC1)和ADC_UNIT_2(SAR ADC2))
    if ((esp32_s3_wroom_1_gpios.find(pin)->second).find(ADC2) != (esp32_s3_wroom_1_gpios.find(pin)->second).end()) {
        /*若用户使用的引脚属于ADC2单元*/

        // 通过GPIO编号来查找对应的ADC2通道;
        adc2_channel_t channel = gpio_to_adc2_channel.find(pin)->second;

        // 要实现最准确的电压读取, 这里必须循环两次, 第一次读取粗略值后使用其校准衰减系数, 第二次再使用校准后的衰减系数读取精确电压;
        for (uint8_t i = 0; i < 2; ++i) {
            // 调用 adc_auto_atten 函数来自动校准衰减系数(选择使读出电压最准确的衰减系数)
            adc_atten_t atten = adc_auto_atten(voltage);

            // ADC2初始化(ADC2通道, 衰减系数)
            adc2_config_channel_atten(channel, atten);

            // ADC2 校准(ESP32两个ADC单元：ADC_UNIT_1(SAR ADC1)和ADC_UNIT_2(SAR ADC2), 衰减系数, ADC分辨率(测量位宽), 参考电压mv, ADC校准参数对象)
            esp_adc_cal_characterize(ADC_UNIT_2, atten, ADC_WIDTH_BIT_12, 3300, adc_chars);

            // ADC2 读取原始数值
            adc2_get_raw(channel, ADC_WIDTH_BIT_12, &raw_value);

            // 计算 ADC2 读取的电压值
            voltage = esp_adc_cal_raw_to_voltage(raw_value, adc_chars);
        }
    } else if ((esp32_s3_wroom_1_gpios.find(pin)->second).find(ADC1) != (esp32_s3_wroom_1_gpios.find(pin)->second).end()) {
        /*若用户使用的引脚属于 ADC1 单元*/

        // 通过 GPIO 编号来查找对应的 ADC1 通道;
        adc1_channel_t channel = gpio_to_adc1_channel.find(pin)->second;

        // 要实现最准确的电压读取, 这里必须循环两次, 第一次读取粗略值后使用其校准衰减系数, 第二次再使用校准后的衰减系数读取精确电压;
        for (uint8_t i = 0; i < 2; ++i) {
            // 调用 adc_auto_atten 函数来自动校准衰减系数(选择使读出电压最准确的衰减系数)
            adc_atten_t atten = adc_auto_atten(voltage);

            // 配置 ADC1 分辨率(测量位宽)
            adc1_config_width(ADC_WIDTH_BIT_12);

            // 配置 ADC1 的通道和衰减系数
            adc1_config_channel_atten(channel, atten);

            // ADC2 校准(ESP32两个ADC单元：ADC_UNIT_1(SAR ADC1)和ADC_UNIT_2(SAR ADC2), 衰减系数, ADC分辨率(测量位宽), 参考电压mv, ADC校准参数对象)
            esp_adc_cal_characterize(ADC_UNIT_1, atten, ADC_WIDTH_BIT_12, 3300, adc_chars);

            // ADC1 读取原始数值
            raw_value = adc1_get_raw(channel);

            // 计算 ADC1 读取的电压值
            voltage = esp_adc_cal_raw_to_voltage(raw_value, adc_chars);
        }
    }
}

/*------------------------------------ GPIO ------------------------------------*/

/**
 * @brief 初始化指定引脚的 GPIO 配置。
 *
 * 该函数用于初始化 ESP32-S3-WROOM-1 模块上的 GPIO 引脚配置。
 * 通过组合三态状态 (tri_state)、模式 (mode) 和输入输出类型 (io_type) 的任意标识符，可以实现GPIO 的灵活配置。
 *
 * @param[in] pin 要配置的 GPIO 引脚编号。
 * @param[in] configs 包含 GPIO 配置参数的向量。该向量中的标识符可以以任意顺序和数量出现，可以包括三态状态、模式和输入输出类型的标识符。
 *
 * @note 三态标识 (tri_state): GPIO_PU (上拉) | GPIO_PD (下拉) | GPIO_FLOAT (浮空)
 * @note 模式标识 (mode): GPIO_PP (推挽) | GPIO_OD (开漏) | GPIO_ANALOG (模拟) | GPIO_INTR (中断)
 * @note 输入输出类型标识 (io_type): GPIO_OUT (输出) | GPIO_IN (输入) | GPIO_IN_OUT (输入和输出)
 *
 * @note 只有在引脚合法且配置参数成功解析的情况下，函数才会设置引脚的配置。如果用户将模式设置为模拟输入，
 * 函数会验证该引脚是否支持 ADC。如果不支持，将发出警告并停止配置。
 *
 * @warning 如果传入的引脚编号不存在于 `esp32_s3_wroom_1_gpios` 列表中，函数将发出错误警告并直接返回。
 * @warning 如果引脚不支持用户设置的配置（例如 ADC 模式），函数将发出错误警告且不会应用该配置。
 *
 * @retval void 无返回值。
 *
 * @see parse_GPIO_config
 * @see set_GPIO_config
 */
void GPIOs::gpioInit(uint8_t pin, const std::unordered_set<uint8_t> &configs) {
    // 查询"pin"是否是合法的GPIO, 如果"pin"是实际上不存在的引脚则直接返回;
    auto gpio = esp32_s3_wroom_1_gpios.find(pin);
    if (gpio == esp32_s3_wroom_1_gpios.end()) {
        WARN(WarningLevel::ERROR, "this pin does not exist, pin:%d", pin);
        return;
    }

    // 解析并验证GPIO配置参数
    uint8_t tri_state, mode, io_type;
    if (!parse_GPIO_config(configs, tri_state, mode, io_type)) return;
    if (!validate_GPIO_config(tri_state, mode, io_type)) return;

    if (mode == GPIO_ANALOG) {
        // 用户设置引脚模式为模拟输入则查询该引脚是否支持模拟输入, 若支持则设置引脚模式;
        if (gpio->second.find(ADC2) != gpio->second.end() || gpio->second.find(ADC1) != gpio->second.end()) {
            set_GPIO_config(pin, tri_state, mode, io_type);
            return;
        }
        WARN(WarningLevel::ERROR, "this pin does not support ADC, pin:%d", pin);
    }

    set_GPIO_config(pin, tri_state, mode, io_type);
}

/**
 * @brief 替换指定引脚的 GPIO 配置
 *
 * 此函数用于根据提供的配置集合更新指定引脚的 GPIO 配置。它确保在应用新配置之前，
 * 该引脚已经初始化，并且配置参数合法有效。该函数允许灵活地更新引脚配置中的
 * 三态（tri_state）、模式（mode）和输入输出类型（io_type）。
 *
 * 代码功能概述：
 * - 引脚查找：首先在 GPIO 注册表中查找指定引脚，如果未初始化，则发出错误警告并返回。
 * - 配置解析：解析传入的配置集合，提取并解析三态、模式和输入输出类型参数。
 * - 默认配置处理：如果某一配置参数未设置（为 GPIO_NULL），则保留该引脚的当前配置。
 * - 配置验证：验证新的 GPIO 配置是否有效，如果无效，则发出警告并返回。
 * - 应用配置：将有效的新配置应用于指定引脚。
 *
 * @param pin 要配置的 GPIO 引脚编号
 * @param configs 包含新配置的集合，可能包含三态（tri_state）、模式（mode）和输入输出类型（io_type）
 *
 * @note 与 gpioInit 不同，本函数允许仅更新三联配置中的一个或多个参数，而不是全覆盖更新。
 */
void GPIOs::gpioReplaceConfig(uint8_t pin, const std::unordered_set<uint8_t> &configs) {
    // 在GPIO注册表中查找指定的pin
    auto it = GPIO_Registry.find(pin);
    // 未找到引脚(说明该引脚没有初始化), 则返回
    if (it == GPIO_Registry.end()) {
        WARN(WarningLevel::ERROR, "this pin is not initialized, pin:%d", pin);
        return;
    }

    // 解析GPIO配置参数
    uint8_t tri_state, mode, io_type;
    if (!parse_GPIO_config(configs, tri_state, mode, io_type)) return;

    // 如果某个配置参数为 GPIO_NULL，保留旧配置
    tri_state = (tri_state != GPIO_NULL) ? tri_state : it->second.tri_state;
    mode = (mode != GPIO_NULL) ? mode : it->second.mode;
    io_type = (io_type != GPIO_NULL) ? io_type : it->second.io_type;

    // 验证GPIO配置参数
    if (!validate_GPIO_config(tri_state, mode, io_type)) return;

    set_GPIO_config(pin, tri_state, mode, io_type);
}

/**
 * @brief 初始化一组 GPIO 引脚的配置。
 *
 * 该函数用于批量初始化一组 GPIO 引脚的配置。它通过遍历提供的引脚集合，对每个引脚调用 `gpioInit` 函数，并传入相同的配置参数进行配置。
 *
 * @param[in,out] pins 包含待初始化的 GPIO 引脚编号的 `std::unordered_set` 容器。集合中的每个引脚都将按给定的同一配置进行初始化。
 * @param[in] configs 包含 GPIO 配置参数的向量。该向量中的标识符可以以任意顺序和数量出现，可以包括三态状态、模式和输入输出类型的标识符。
 *
 * @note 三态标识 (tri_state): GPIO_PU (上拉) | GPIO_PD (下拉) | GPIO_FLOAT (浮空)
 * @note 模式标识 (mode): GPIO_PP (推挽) | GPIO_OD (开漏) | GPIO_ANALOG (模拟) | GPIO_INTR (中断)
 * @note 输入输出类型标识 (io_type): GPIO_OUT (输出) | GPIO_IN (输入) | GPIO_IN_OUT (输入和输出)
 *
 * @note 配置参数 `configs` 将应用于集合 `pins` 中的所有引脚。
 * @note 每个引脚的具体配置细节请参见 `gpioInit` 函数的说明。
 *
 * @warning 如果 `pins` 集合中包含不存在的引脚编号，`gpioInit` 函数将在初始化过程中发出警告，并跳过该引脚的配置。
 *
 * @retval void 无返回值。
 *
 * @see gpioInit
 */
void GPIOs::gpioInitGroup(const std::unordered_set<uint8_t> &pins, const std::unordered_set<uint8_t> &configs) {
    for (auto &i : pins) gpioInit(i, configs);
}

/**
 * @brief 读取一个GPIO引脚的电平值或模拟值
 * @param[in] pin GPIO引脚编号
 * @return 返回读取到的电平值（数字：HIGH/LOW）或模拟值（模拟数值[0, 4095]），如果GPIO引脚不存在，返回-1
 * @note 如果GPIO引脚未初始化，将自动初始化为浮空输入模式(GPIO_IN, GPIO_FLOAT)
 * @note 调用 setAnalogOutputMode(bool mode) 函数可以设置返回的模拟量类型，
 * mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值[0, 4095]，设为 ADC_OUT_VOLTAGE 时返回电压值[0, 3300]mV
 */
int32_t GPIOs::gpioReadDataBit(uint8_t pin) {
    // 在GPIO注册表中查找指定的pin
    auto it = GPIO_Registry.find(pin);

    // 如果未找到引脚，尝试自动初始化为浮空输入模式
    if (it == GPIO_Registry.end()) {
        gpioInit(pin, {GPIO_IN, GPIO_FLOAT});

        it = GPIO_Registry.find(pin);              // 初始化后重新查找引脚
        if (it == GPIO_Registry.end()) return -1;  // 如果初始化后仍未找到，返回-1
    }

    int32_t DataBit = -1;  // 初始化返回值，默认为-1

    // 检查引脚模式
    if (it->second.mode == GPIO_ANALOG) {
        // 如果引脚为模拟输入模式，读取模拟值或电压
        gpioReadAnalogBit(pin);

        // 如果模拟输出 mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值[0, 4095], 设为 ADC_OUT_VOLTAGE时返回电压[0, 3300]mv;
        DataBit = (OUTPUT_MODE == ADC_OUT_RAW_VALUE) ? gpioReadRawValue() : gpioReadVoltage();
    } else {
        // 数字输入/输出模式
        if (it->second.io_type == GPIO_OUT || it->second.io_type == GPIO_IN_OUT) {
            // 如果为输出或双向模式，返回缓存的状态值
            DataBit = it->second.status;
        } else if (it->second.io_type == GPIO_IN) {
            // 如果为输入模式，读取实际电平值
            DataBit = gpio_get_level(static_cast<gpio_num_t>(pin));
        }
    }

    it->second.status = DataBit;  // 更新GPIO_Registry中该引脚的状态
    return DataBit;               // 返回读取的数据位
}

/**
 *@brief 读取一组GPIO状态
 *这个函数读取GPIO_Registry map中所有GPIO的状态，并返回一个map，DataGroup，用于存储所有GPIO的状态。
 *@return std::map<uint8_t, int32_t> DataGroup，用于存储所有GPIO的状态。
 * @note 调用 void setAnalogOutputMode(bool mode) 函数可设置返回的模拟量类型 mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值[0, 4095], 设为 ADC_OUT_VOLTAGE
 * 时返回电压[0, 3300]mv;
 */
std::map<uint8_t, int32_t> GPIOs::gpioReadDataGroup() {
    std::map<uint8_t, int32_t> DataGroup;  // 定义一个map，DataGroup，用于存储所有GPIO的状态

    // 遍历GPIO_Registry map
    // 对于每一个GPIO，调用gpioReadDataBit函数读取该GPIO的状态
    // 将该GPIO的编号和读取到的状态插入DataGroup map中
    for (auto &i : GPIO_Registry) DataGroup.insert(std::pair<uint8_t, int32_t>(i.first, gpioReadDataBit(i.first)));

    return DataGroup;  // 返回DataGroup map
}

/**
 * @brief 读取一个GPIO引脚的配置
 * @param[in] pin GPIO引脚编号
 * @return 返回读取到的配置，如果GPIO引脚不存在，返回-1
 */
GPIOs::GPIO_Type GPIOs::gpioReadConfigBit(uint8_t pin) {
    auto it = GPIO_Registry.find(pin);                  // 使用auto关键字，查询map对象GPIO_Registry中是否存在pin
    if (it == GPIO_Registry.end()) return GPIO_Type();  // 未找到配置则返回默认的空配置

    return it->second;  // 返回读取的引脚配置
}

/**
 *@brief 读取一组GPIO的引脚配置
 *这个函数读取GPIO_Registry map中所有GPIO的工作模式，并返回一个map。
 *@return std::map<uint8_t, int32_t> ConfigGroup，用于存储所有GPIO的工作模式。
 *@note 调用 void setAnalogOutputMode(bool mode) 函数可设置返回的模拟量类型 mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值, 设为 ADC_OUT_VOLTAGE
 * 时返回电压(mv);
 */
std::unordered_map<uint8_t, GPIOs::GPIO_Type> GPIOs::gpioReadConfigGroup() { return GPIO_Registry; }

/**
 * @brief 写入单个GPIO的电平值
 * @param pin - 要写入的GPIO引脚.
 * @param value - 要写入引脚的值，可以是0(LOW)或1(HIGH).
 * @note 如果要写入的GPIO引脚没有初始化, 则尝试自动初始化为推挽输出模式.
 * @return void
 */
void GPIOs::gpioWriteBit(uint8_t pin, uint8_t value) {
    // 在GPIO注册表中查找给定的pin是否存在
    auto it = GPIO_Registry.find(pin);

    // 如果未找到引脚，尝试自动初始化为推挽输出模式
    if (it == GPIO_Registry.end()) {
        gpioInit(pin, {GPIO_OUT, GPIO_PP});

        it = GPIO_Registry.find(pin);           // 初始化后重新查找引脚
        if (it == GPIO_Registry.end()) return;  // 如果初始化后仍未找到，返回
    }

    // 检查引脚的io_type是否为输出类型，如果不是则返回
    if (it->second.io_type != GPIO_OUT && it->second.io_type != GPIO_IN_OUT) return;

    // 写入引脚电平值
    gpio_set_level(static_cast<gpio_num_t>(pin), value);

    // 更新GPIO_Registry中该引脚的状态
    it->second.status = static_cast<int32_t>(value);
}

/**
 *@brief 向多个GPIO写入数据
 *@param value 一个unordered_map容器，用于存储GPIO的编号和要写入的数据
 *该函数遍历value容器中的每一个元素，
 *如果GPIO_Registry中存在该GPIO编号并且属于输出引脚，则设置GPIO状态。
 */
void GPIOs::gpioWriteGroup(std::unordered_map<uint8_t, uint8_t> value) {
    for (auto &i : value) gpioWriteBit(i.first, i.second);
}

/**
 * @brief 将指定输出引脚置为高电平
 * @param pin - 要写入的GPIO引脚.
 * @return void
 */
void GPIOs::gpioSetBit(uint8_t pin) { gpioWriteBit(pin, HIGH); }

/**
 * @brief 将指定输出引脚置为低电平
 * @param pin - 要写入的GPIO引脚.
 * @return void
 */
void GPIOs::gpioResetBit(uint8_t pin) { gpioWriteBit(pin, LOW); }

/**
 * @brief 将指定输出引脚电平反转
 * @param pin - 要反转的GPIO引脚.
 * @return void
 */
void GPIOs::gpioToggleBit(uint8_t pin) {
    // 在GPIO注册表中查找指定引脚的状态
    auto it = GPIO_Registry.find(pin);

    if (it != GPIO_Registry.end()) {
        // 如果引脚存在于注册表中，执行电平反转操作
        gpioWriteBit(pin, !it->second.status);
    } else {
        // 如果注册表中不存在引脚, 则将引脚初始化后设置为低电平;
        gpioWriteBit(pin, LOW);
    }
}

/**
 * @brief 反转一组输出GPIO的引脚电平
 * @return void
 */
void GPIOs::gpioToggleGroup() {
    for (auto &i : GPIO_Registry) gpioToggleBit(i.first);
}

/**
 * @brief 从一组GPIO中移除指定的引脚
 * @param pin - 要移除的GPIO引脚.
 * 如果元素不存在，则该函数不会执行任何操作.
 * @return void
 */
void GPIOs::gpioRemoveBit(uint8_t pin) { GPIO_Registry.erase(pin); }

/**
 * @brief 清空一组GPIO
 * 将Group中的所有GPIO移除.
 * @return void
 */
void GPIOs::gpioRemoveGroup() { GPIO_Registry.clear(); }

/**
 * @brief 解析GPIO配置参数。
 *
 * 该函数用于解析传入的GPIO配置参数列表，并设置相应的引脚三态标识、模式标识和输入输出类型标识。
 * 如果传入了多个相同类别的标识，则仅使用第一个有效的标识，并发出警告。
 *
 * @param configs 传入的GPIO配置参数列表，由多个uint8_t类型的标识组成。
 *                每个标识代表一种GPIO配置（如上拉电阻、推挽模式、输出类型等）。
 * @param[out] tri_state 解析后的引脚三态标识（浮空、上拉、下拉）。如果未设置有效标识，返回GPIO_NULL。
 * @param[out] mode 解析后的引脚模式标识（推挽、开漏、模拟、中断）。如果未设置有效标识，返回GPIO_NULL。
 * @param[out] io_type 解析后的输入输出类型标识（输入、输出、输入&输出）。如果未设置有效标识，返回GPIO_NULL。
 *
 * @warning 如果传入了多个同类别的标识（如多个三态标识或模式标识），函数将仅使用第一个有效标识，并发出相应的警告。
 *
 * @note 该函数会在所有标识都已设置时提前退出循环，以提高性能。
 * @return 解析成功返回:true; 解析出错返回:false;
 *
 * @example
 * @code
 * uint8_t tri_state, mode, io_type;
 * std::vector<uint8_t> configs = {GPIO_PU, GPIO_PP, GPIO_OUT};
 * parseGPIOConfig(configs, tri_state, mode, io_type);
 * // tri_state == GPIO_PU, mode == GPIO_PP, io_type == GPIO_OUT
 * @endcode
 */
bool GPIOs::parse_GPIO_config(const std::unordered_set<uint8_t> &configs, uint8_t &tri_state, uint8_t &mode, uint8_t &io_type) {
    // 初始化或重置所有配置为无效状态（GPIO_NULL表示未设置）
    auto resetConfig = [&]() {
        tri_state = GPIO_NULL;  // 引脚三态（浮空、上拉、下拉）
        mode = GPIO_NULL;       // 引脚模式（推挽、开漏、模拟、中断）
        io_type = GPIO_NULL;    // 输入输出类型（输入、输出、输入&输出）
    };
    resetConfig();

    // 定义一个lambda函数，用于设置配置参数，并在必要时发出警告
    auto setConfig = [](uint8_t &field, uint8_t config, const char *warningMessage) {
        if (field != GPIO_NULL) {
            WARN(WarningLevel::WARNING, warningMessage);  // 如果对应的field已经设置过，发出警告
        } else {
            field = config;  // 否则设置field为传入的config值
        }
    };

    // 遍历配置列表，解析每个参数
    for (uint8_t config : configs) {
        // 检查是否为三态标识（浮空、上拉、下拉）
        if (config == GPIO_PU || config == GPIO_PD || config == GPIO_FLOAT) {
            setConfig(tri_state, config, "multiple tri-state identifiers.");  // 设置tri_state
        }
        // 检查是否为模式标识（推挽、开漏、模拟、中断）
        else if (config == GPIO_PP || config == GPIO_OD || config == GPIO_ANALOG || config == GPIO_INTR) {
            setConfig(mode, config, "multiple mode identifiers.");  // 设置mode
        }
        // 检查是否为IO类型标识（输入、输出、输入&输出）
        else if (config == GPIO_OUT || config == GPIO_IN || config == GPIO_IN_OUT) {
            setConfig(io_type, config, "multiple IO type identifiers.");  // 设置io_type
        }
        // 对于未匹配的模式，输出错误;
        else {
            WARN(WarningLevel::ERROR, "this IO config does not exist, config:%x", config);
            // 重置引脚配置为无效状态，防止错误的配置生效
            resetConfig();
            return false;
        }

        // 如果所有标识均已设置，提前退出循环以提高性能
        if (tri_state != GPIO_NULL && mode != GPIO_NULL && io_type != GPIO_NULL) break;
    }

    // 检查是否有未设置的标识，并发出相应警告
    if (tri_state == GPIO_NULL) WARN(WarningLevel::WARNING, "tri-state identifier not set.");
    if (mode == GPIO_NULL) WARN(WarningLevel::WARNING, "mode identifier not set.");
    if (io_type == GPIO_NULL) WARN(WarningLevel::WARNING, "iO type identifier not set.");

    return true;
}

/**
 * @brief 验证 GPIO 配置的有效性
 *
 * 该函数用于验证给定的 GPIO 配置组合（`tri_state`、`mode` 和 `io_type`）是否有效。
 * 如果该组合在无效配置集合 `invalid_GPIO_configs` 中，则函数会发出错误警告，并将所有配置重置为 `GPIO_NULL`。
 * 同时，如果在使用上拉或下拉电阻时，没有设置输入、输出或输入输出方式，则同样会重置配置并返回错误。
 *
 * @param tri_state 引脚的三态配置（浮空、上拉、下拉）
 * @param mode 引脚的模式配置（推挽、开漏、模拟、中断）
 * @param io_type 引脚的输入输出类型（输入、输出、输入&输出）
 * @return true 如果配置有效，返回 true
 * @return false 如果配置无效，返回 false 并重置配置
 */
bool GPIOs::validate_GPIO_config(uint8_t &tri_state, uint8_t &mode, uint8_t &io_type) {
    // 初始化或重置所有配置为无效状态（GPIO_NULL表示未设置）
    auto resetConfig = [&]() {
        tri_state = GPIO_NULL;  // 引脚三态（浮空、上拉、下拉）
        mode = GPIO_NULL;       // 引脚模式（推挽、开漏、模拟、中断）
        io_type = GPIO_NULL;    // 输入输出类型（输入、输出、输入&输出）
    };

    std::unordered_set<uint8_t> currentConfig = {tri_state, mode, io_type};  // 生成当前配置的集合
    currentConfig.erase(GPIO_NULL);                                          // 移除未设置的标识（GPIO_NULL）

    // 如果当前配置组合不为空且在禁止的组合列表中，发出错误警告并重置配置
    if (!currentConfig.empty() && invalid_GPIO_configs.find(currentConfig) != invalid_GPIO_configs.end()) {
        WARN(WarningLevel::ERROR, "this GPIO configuration combination is prohibited.");
        // 重置引脚配置为无效状态，防止不合理组合生效
        resetConfig();
        return false;
    }

    // 当使用上拉或下拉电阻时，必须确保引脚被配置为输入、输出或输入输出，否则这些配置将无效。
    if ((tri_state == GPIO_PU || tri_state == GPIO_PD) && io_type == GPIO_NULL) {
        WARN(WarningLevel::ERROR, "Pull-Up or Pull-Down resistors require IO type to be set.");
        // 重置引脚配置为无效状态，防止错误的配置生效
        resetConfig();
        return false;
    }

    // 如果没有发现无效的配置组合，则返回 true
    return true;
}

void GPIOs::set_GPIO_config(uint8_t &pin, uint8_t tri_state, uint8_t mode, uint8_t io_type) {
    // 创建并初始化GPIO配置结构体
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),          // 指定要配置的GPIO引脚
        .mode = GPIO_MODE_DISABLE,              // 默认禁用输入输出模式
        .pull_up_en = GPIO_PULLUP_DISABLE,      // 默认禁用上拉电阻
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // 默认禁用下拉电阻
        .intr_type = GPIO_INTR_DISABLE          // 默认禁用中断
    };

    // 设置上拉或下拉电阻，基于三态标识
    if (tri_state == GPIO_PU) {
        // 启用上拉电阻
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    } else if (tri_state == GPIO_PD) {
        // 启用下拉电阻
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    } else if (tri_state == GPIO_FLOAT) {
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;      // 禁用上拉电阻
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;  // 禁用下拉电阻
    }

    // 设置 GPIO 模式，基于 mode 和 io_type
    if (mode == GPIO_PP) {
        if (io_type == GPIO_OUT) {
            io_conf.mode = GPIO_MODE_OUTPUT;  // 推挽输出
        } else if (io_type == GPIO_IN_OUT) {
            io_conf.mode = GPIO_MODE_INPUT_OUTPUT;  // 输入和推挽输出
        }
    } else if (mode == GPIO_OD) {
        if (io_type == GPIO_OUT) {
            io_conf.mode = GPIO_MODE_OUTPUT_OD;  // 开漏输出
        } else if (io_type == GPIO_IN_OUT) {
            io_conf.mode = GPIO_MODE_INPUT_OUTPUT_OD;  // 输入和开漏输出
        }
    } else if (mode == GPIO_NULL && io_type == GPIO_IN) {
        io_conf.mode = GPIO_MODE_INPUT;  // 仅输入
    } else if (mode == GPIO_INTR) {
        io_conf.intr_type = GPIO_INTR_POSEDGE;  // 设置为上升沿触发中断
        io_conf.mode = GPIO_MODE_INPUT;         // 中断模式通常配置为输入
    }
    // 模拟模式（GPIO_ANALOG）通常不直接配置 GPIO 引脚，而是配置相关外设，因此在此未处理

    // 应用配置到指定的GPIO引脚
    gpio_config(&io_conf);

    // 储存 GPIO 引脚对应的工作模式（状态未知时存储为 -1）
    // GPIO_Registry[pin] = GPIO_Type{-1, mode};
    GPIO_Registry.insert(std::pair<uint8_t, GPIO_Type>(pin, GPIO_Type{-1, tri_state, mode, io_type}));
}

/*------------------------------------ LEDCPWM ------------------------------------*/
struct LedcTimerHash {
    std::size_t operator()(const ledc_timer_t &timer) const { return std::hash<int>()(timer); }
};
struct LedcChannelHash {
    std::size_t operator()(const ledc_channel_t &channel) const { return std::hash<int>()(channel); }
};

// unordered_set 容器在创建时使用了 LedcTimerHash 和 LedcChannelHash 两个结构体，作为容器中元素的哈希函数，以维护容器中元素的哈希值;
std::unordered_set<ledc_timer_t, LedcTimerHash> timer = {LEDC_TIMER_0, LEDC_TIMER_1, LEDC_TIMER_2, LEDC_TIMER_3};
std::unordered_set<ledc_channel_t, LedcChannelHash> channel = {LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3,
                                                               LEDC_CHANNEL_4, LEDC_CHANNEL_5, LEDC_CHANNEL_6, LEDC_CHANNEL_7};

void LEDCPWM::pwmConfig(uint8_t pin, ledc_timer_bit_t duty_resolution, uint32_t freq_hz, ledc_mode_t speed_mode, uint32_t duty) {
    _pin = pin;
    _duty_resolution = duty_resolution;
    _freq_hz = freq_hz;
    _speed_mode = speed_mode;
    _duty = duty;
}

/**
 * @brief 使能LEDCPWM控制器
 * @note 将自动分配并链接定时器和通道, 启动LEDCPWM控制器
 * @return void
 */
void LEDCPWM::pwmEnable() {
    /*
    1.如果指定的PWM输出引脚pin不存在，则返回.
    2.如果 timer 集合为空，则返回.
    3.如果指定的PWM输出引脚pin不支持硬件PWM,则返回.
    */
    auto gpio = esp32_s3_wroom_1_gpios.find(_pin);
    if (gpio == esp32_s3_wroom_1_gpios.end() || timer.empty() || (gpio->second).find(PWM) == (gpio->second).end()) return;

    /*
    分别从两个无序集合里抽取定时器和通道并将其组合链接到一起,
    这两个集合中的元素被抽出表示定时器或通道正在被使用,
    当调用pwmDisable时, 才会把它们放回集合.
    */
    // 将一个定时器从unordered_set timer中抽出(unordered_set是无序的因此我们不知道抽出的具体是哪一个定时器)
    ledc_timer_t SelectedTimer = *timer.begin();
    timer.erase(SelectedTimer);

    // 将一个通道从unordered_set channel中抽出(unordered_set是无序的因此我们不知道抽出的具体是哪一个通道)
    ledc_channel_t SelectedChannel = *channel.begin();
    channel.erase(SelectedChannel);

    // 配置 LEDC 定时器
    ledc_timer.duty_resolution = _duty_resolution;  // 设置占空比解析度
    ledc_timer.freq_hz = _freq_hz;                  // 设置PWM频率
    ledc_timer.speed_mode = _speed_mode;            // 设置时钟速度模式
    ledc_timer.timer_num = SelectedTimer;           // 指定定时器
    ledc_timer_config(&ledc_timer);

    // 配置 LEDC 通道
    ledc_channel.channel = SelectedChannel;  // 指定通道
    ledc_channel.duty = _duty;               // 设置初始占空比
    ledc_channel.gpio_num = _pin;            // 设置PWM输出引脚
    ledc_channel.speed_mode = _speed_mode;   // 设置时钟速度模式
    ledc_channel.timer_sel = SelectedTimer;  // 指定定时器
    ledc_channel_config(&ledc_channel);

    /*
    更改占空比的函数如下:
    esp_err_t ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)

    ledc_set_duty函数中duty_cycle的最大值(即占空比100%时)取决于占空比解析度（duty_resolution）的设置;
    根据占空比解析度的设置，duty_cycle的最大值可以使用此公式计算: duty_cycle_max = (1 << duty_resolution) - 1;.

    这个位置将[DUTY_CYCLE_MIN = 0, DUTY_CYCLE_MAX = 10 000 000]区间映射到[0, duty_cycle_max = 100%占空比]区间, 公式如下:
    (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

    float TIMER_BIT_DUTY_CYCLE = ((1 << ledc_timer.duty_resolution) - 1) / static_cast<float>(DUTY_CYCLE_MAX);
    uint32_t duty_cycle = static_cast<uint32_t>(TIMER_BIT_DUTY_CYCLE * dutyCycle));
    */
    TIMER_BIT_DUTY_CYCLE = ((1 << ledc_timer.duty_resolution) - 1) / static_cast<float>(DUTY_CYCLE_MAX);
}

/**
 * @brief 禁用LEDCPWM控制器, 停止PWM输出.
 * @return void
 */
void LEDCPWM::pwmDisable() {
    ledc_stop(ledc_timer.speed_mode, ledc_channel.channel, 0);  // 停止PWM波形的输出
    timer.insert(ledc_timer.timer_num);                         // 释放定时器(将空闲的定时器放回timer, 以供其他PWM实例调用)
    channel.insert(ledc_channel.channel);                       // 释放通道(将空闲的通道放回channel, 以供其他PWM实例调用)
}

/**
 * @brief 调整PWM的占空比
 * @param dutyCycle 占空比, 取值范围为: [DUTY_CYCLE_MIN = 0, DUTY_CYCLE_MAX = 10 000 000].
 * @note dutyCycle = 0 时占空比为 0%, dutyCycle = 10 000 000 时占空比为 100%.
 * @note 为什么将DUTY_CYCLE_MAX定为10000000 :
 * 1.这个数值保证了硬件PWM所能达到的最高解析度, 在做区间映射时不会造成失真;
 * 2.这个数值避免了大量的浮点计算;
 * 3.这个数值使用起来相对方便;
 * @return void
 */
void LEDCPWM::updateDuty(uint32_t duty) {
    ledc_set_duty(ledc_timer.speed_mode, ledc_channel.channel, static_cast<uint32_t>(TIMER_BIT_DUTY_CYCLE * duty));
    ledc_update_duty(ledc_timer.speed_mode, ledc_channel.channel);
}

/**
 * @brief 在给定时间内将当前占空比线性变换到目标占空比.
 * @param duration_ms 持续时间 ms
 * @param target_duty 目标占空比
 * @param fade_mode 渐变模式, 设为"LEDC_FADE_NO_WAIT(默认)"时渐变函数将立即返回，不阻塞后续代码的执行;
 * 设为"LEDC_FADE_WAIT_DONE"时渐变函数将阻塞，直到渐变到目标占空比为止.
 * @note 占空比范围: [DUTY_CYCLE_MIN = 0, DUTY_CYCLE_MAX = 10 000 000]
 * @return void
 */
void LEDCPWM::fadeWithTime(uint64_t duration_ms, uint32_t target_duty, ledc_fade_mode_t fade_mode) {
    /*
    软件实现:
    // 持续时间不能为零, 起始占空比不能与中止占空比相同;
    if (duration_ms == 0 || beginDutyCycle == endDutyCycle) return;

    // 计算周期间隔
    int32_t interval = static_cast<int32_t>(endDutyCycle - beginDutyCycle) / static_cast<int32_t>(duration_ms);

    // 控制循环方向
    int8_t step = interval > 0 ? 1 : -1;

    // 循环间隔乘以 step 将变为正值
    interval = abs(interval) * step;

    // 更新占空比(检查循环变量 i 是否符合循环终止条件。如果 step 大于 0，则当 i 小于endDutyCycle 时继续循环；否则，当 i 大于 endDutyCycle 时继续循环。)
    for (int32_t i = beginDutyCycle; step > 0 ? i < endDutyCycle : i > endDutyCycle; i += interval) {
        updateDuty(i);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    */

    // 硬件实现:
    ledc_fade_func_install(0);  // 使能渐变
    ledc_set_fade_with_time(ledc_timer.speed_mode, ledc_channel.channel, static_cast<uint32_t>(TIMER_BIT_DUTY_CYCLE * target_duty), duration_ms);
    ledc_fade_start(ledc_timer.speed_mode, ledc_channel.channel, fade_mode);
}

/**
 * @brief 在给定时间内将当前占空比线性变换到目标占空比.
 * @param scale 占空比步长
 * @param cycle_num 总步数
 * @param target_duty 目标占空比
 * @param fade_mode 渐变模式, 设为"LEDC_FADE_NO_WAIT(默认)"时渐变函数将立即返回，不阻塞后续代码的执行;
 * 设为"LEDC_FADE_WAIT_DONE"时渐变函数将阻塞，直到渐变到目标占空比为止.
 * @note 占空比范围: [DUTY_CYCLE_MIN = 0, DUTY_CYCLE_MAX = 10 000 000]
 * @return void
 */
void LEDCPWM::fadeWithStep(uint32_t scale, uint32_t cycle_num, uint32_t target_duty, ledc_fade_mode_t fade_mode) {
    // 硬件实现:
    ledc_fade_func_install(0);  // 使能渐变
    ledc_set_fade_with_step(ledc_timer.speed_mode, ledc_channel.channel, static_cast<uint32_t>(TIMER_BIT_DUTY_CYCLE * target_duty), scale, cycle_num);
    ledc_fade_start(ledc_timer.speed_mode, ledc_channel.channel, fade_mode);
}