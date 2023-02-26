/**
 * @file io.cpp
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

#include <driver/adc.h>
#include <esp32_pin_defi.h>
#include <esp_adc_cal.h>
#include <io.h>

// ESP32-WROOM-32E引脚定义(定义了MCU的GPIO编号, 以及是否支持输入/输出/模拟)
const std::unordered_map<uint8_t, std::unordered_set<uint8_t>> esp32_wroom_32e_gpios = {{0, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {1, {OUTPUT_OK, PWM_OK}},
                                                                                        {2, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {3, {INPUT_OK, PWM_OK}},
                                                                                        {4, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {5, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {12, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {13, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {14, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {15, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {16, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {17, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {18, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {19, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {21, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {22, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {23, {INPUT_OK, OUTPUT_OK, PWM_OK}},
                                                                                        {25, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {26, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {27, {INPUT_OK, OUTPUT_OK, ADC2_OK, PWM_OK}},
                                                                                        {32, {INPUT_OK, OUTPUT_OK, ADC1_OK, PWM_OK}},
                                                                                        {33, {INPUT_OK, OUTPUT_OK, ADC1_OK, PWM_OK}},
                                                                                        {34, {INPUT_OK, ADC1_OK, PWM_OK}},
                                                                                        {35, {INPUT_OK, ADC1_OK, PWM_OK}},
                                                                                        {36, {INPUT_OK, ADC1_OK, PWM_OK}},
                                                                                        {39, {INPUT_OK, ADC1_OK, PWM_OK}}};

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
    if ((esp32_wroom_32e_gpios.find(pin)->second).find(ADC2_OK) != (esp32_wroom_32e_gpios.find(pin)->second).end()) {
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
    } else if ((esp32_wroom_32e_gpios.find(pin)->second).find(ADC1_OK) != (esp32_wroom_32e_gpios.find(pin)->second).end()) {
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

/**
 * @brief 初始化指定的GPIO引脚
 * @param pin: 待初始化的GPIO引脚号
 * @param mode: 引脚模式, 可以是 推挽输出 | 开漏输出 | 浮空输入 | 上拉输入 | 下拉输入 | 模拟输入 | 开漏 | 上拉 | 下拉
 * @param defi: GPIO_OUT_PP | GPIO_OUT_OD | GPIO_IN_FLOATING | GPIO_IN_PU | GPIO_IN_PD | GPIO_IN_ANALOG | GPIO_OD | GPIO_PU | GPIO_PD
 * @return 无
 */
void GPIOs::gpioInit(uint8_t pin, uint8_t mode) {
    /*
    这个代码片段实现了对ESP32引脚模式的初始化，具体地：
    1. 查询该引脚是否是合法的ESP32引脚，如果不是则直接返回；
    2. 根据用户设置的引脚模式，查询该引脚是否支持相应的功能（输出、输入、模拟输入），若支持则设置该引脚的模式；
    3. 若用户设置的引脚模式为开漏、上拉或下拉，则直接设置引脚模式。
    */

    // 查询"pin"是否是合法的GPIO, 如果"pin"是实际上不存在的引脚则直接返回;
    auto gpio = esp32_wroom_32e_gpios.find(pin);
    if (gpio == esp32_wroom_32e_gpios.end()) return;

    // 若用户设置引脚模式为推挽或开漏输出则查询该引脚是否支持输出, 若支持则设置引脚模式;
    if (mode == GPIO_OUT_PP || mode == GPIO_OUT_OD) {
        if (gpio->second.find(OUTPUT_OK) != gpio->second.end()) Set_GPIO_Mode(pin, mode);
        return;
    }
    // 若用户设置引脚模式为浮空或上拉或下拉输入则查询该引脚是否支持输入, 若支持则设置引脚模式;
    if (mode == GPIO_IN_FLOATING || mode == GPIO_IN_PU || mode == GPIO_IN_PD) {
        if (gpio->second.find(INPUT_OK) != gpio->second.end()) Set_GPIO_Mode(pin, mode);
        return;
    }
    // 若用户设置引脚模式为模拟输入则查询该引脚是否支持模拟输入, 若支持则设置引脚模式;
    if (mode == GPIO_IN_ANALOG) {
        if (gpio->second.find(ADC2_OK) != gpio->second.end() || gpio->second.find(ADC1_OK) != gpio->second.end()) Set_GPIO_Mode(pin, mode);
        return;
    }
    // 若用户设置引脚模式为开漏或上拉或下拉, 则直接设置引脚模式；
    if (mode == GPIO_OD || mode == GPIO_PU || mode == GPIO_PD) Set_GPIO_Mode(pin, mode);
}

/**
 *@brief 初始化一组GPIO
 *@param Group 一个unordered_map容器，用于存储GPIO的编号和要设置的工作模式{{pin, mode}, {pin, mode}, ...}
 * @param pin: 待初始化的GPIO引脚号
 * @param mode: 引脚模式, 可以是 推挽输出 | 开漏输出 | 浮空输入 | 上拉输入 | 下拉输入 | 模拟输入 | 开漏 | 上拉 | 下拉
 * @param defi: GPIO_OUT_PP | GPIO_OUT_OD | GPIO_IN_FLOATING | GPIO_IN_PU | GPIO_IN_PD | GPIO_IN_ANALOG | GPIO_OD | GPIO_PU | GPIO_PD
 */
void GPIOs::gpioInitGroup(std::unordered_map<uint8_t, uint8_t> Group) {
    for (auto &i : Group) gpioInit(i.first, i.second);
}

/**
 * @brief 读取一个GPIO引脚的数据位值
 * @param[in] pin GPIO引脚编号
 * @return 返回读取到的数据位值(数字值: HIGH/LOW; 模拟值: 模拟数值[0, 4095])，如果GPIO引脚不存在，返回-1
 * @note 调用 void setAnalogOutputMode(bool mode) 函数可设置返回的模拟量类型 mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值[0, 4095], 设为 ADC_OUT_VOLTAGE
 * 时返回电压[0, 3300]mv;
 */
int32_t GPIOs::gpioReadDataBit(uint8_t pin) {
    /*
    这段代码的功能是读取一个GPIO引脚的数值，如果该引脚对应的模式为GPIO_IN_ANALOG，则使用analogRead函数读取该引脚的数值；否则使用digitalRead函数读取该引脚的数值。
    读取到的数值存入DataBit变量中，并将GPIO_Status中该引脚对应的状态更新为读取到的数值。最后返回读取到的数值。如果没有找到该引脚，则返回-1。
    */

    auto it = GPIO_Status.find(pin);         // 使用auto关键字，查询map对象GPIO_Status中是否存在pin
    if (it == GPIO_Status.end()) return -1;  // 如果没有找到，返回-1

    int32_t DataBit;

    // 如果该引脚的模式是模拟输入
    if ((it->second).mode == GPIO_IN_ANALOG) {
        gpioReadAnalogBit(pin);  // 读取该引脚的模拟数值和电压

        // 如果模拟输出 mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值[0, 4095], 设为 ADC_OUT_VOLTAGE时返回电压[0, 3300]mv;
        (OUTPUT_MODE = ADC_OUT_RAW_VALUE) ? (DataBit = gpioReadRawValue()) : (DataBit = gpioReadVoltage());

        // DataBit = static_cast<int32_t>(analogRead(pin));  // 将该引脚的读取值转化为整型并赋值给DataBit
    } else {
        DataBit = digitalRead(pin);  // 否则读取该引脚的数字读取值，并赋值给DataBit
    }

    (it->second).status = DataBit;  // 更新GPIO_Status中该引脚的状态
    return DataBit;                 // 返回读取的数据位
}

/**
 *@brief 读取一组GPIO状态
 *这个函数读取GPIO_Status map中所有GPIO的状态，并返回一个map，DataGroup，用于存储所有GPIO的状态。
 *@return std::map<uint8_t, int32_t> DataGroup，用于存储所有GPIO的状态。
 * @note 调用 void setAnalogOutputMode(bool mode) 函数可设置返回的模拟量类型 mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值[0, 4095], 设为 ADC_OUT_VOLTAGE
 * 时返回电压[0, 3300]mv;
 */
std::map<uint8_t, int32_t> GPIOs::gpioReadDataGroup() {
    std::map<uint8_t, int32_t> DataGroup;  // 定义一个map，DataGroup，用于存储所有GPIO的状态

    // 遍历GPIO_Status map
    // 对于每一个GPIO，调用gpioReadDataBit函数读取该GPIO的状态
    // 将该GPIO的编号和读取到的状态插入DataGroup map中
    for (auto &i : GPIO_Status) DataGroup.insert(std::pair<uint8_t, int32_t>(i.first, gpioReadDataBit(i.first)));

    return DataGroup;  // 返回DataGroup map
}

/**
 * @brief 读取一个GPIO引脚的工作模式
 * @param[in] pin GPIO引脚编号
 * @return 返回读取到的工作模式，如果GPIO引脚不存在，返回-1
 */
uint8_t GPIOs::gpioReadModeBit(uint8_t pin) {
    auto it = GPIO_Status.find(pin);         // 使用auto关键字，查询map对象GPIO_Status中是否存在pin
    if (it == GPIO_Status.end()) return -1;  // 如果没有找到，返回-1

    return (it->second).mode;  // 返回读取的引脚工作模式
}

/**
 *@brief 读取一组GPIO的工作模式
 *这个函数读取GPIO_Status map中所有GPIO的工作模式，并返回一个map。
 *@return std::map<uint8_t, int32_t> ModeGroup，用于存储所有GPIO的工作模式。
 *@note 调用 void setAnalogOutputMode(bool mode) 函数可设置返回的模拟量类型 mode 设为 ADC_OUT_RAW_VALUE(默认) 时返回模拟值, 设为 ADC_OUT_VOLTAGE
 * 时返回电压(mv);
 */
std::map<uint8_t, uint8_t> GPIOs::gpioReadModeGroup() {
    std::map<uint8_t, uint8_t> ModeGroup;  // 定义一个map，ModeGroup，用于存储所有GPIO的工作模式

    // 遍历GPIO_Status map
    // 对于每一个GPIO，调用gpioReadModeBit函数读取该GPIO的工作模式
    // 将该GPIO的编号和读取到的状态插入ModeGroup map中
    for (auto &i : GPIO_Status) ModeGroup.insert(std::pair<uint8_t, uint8_t>(i.first, gpioReadModeBit(i.first)));

    return ModeGroup;  // 返回ModeGroup map
}

/**
 * @brief 写入单个GPIO的数值
 * @param pin - 要写入的GPIO引脚.
 * @param value - 要写入引脚的值，可以是0(LOW)或1(HIGH).
 * @return void
 */
void GPIOs::gpioWriteBit(uint8_t pin, uint8_t value) {
    // 在GPIO_Status中查找给定的pin
    auto it = GPIO_Status.find(pin);

    // 如果找不到pin或者该pin的mode不是GPIO_OUT_PP或者不是GPIO_OUT_OD，直接返回
    if (it == GPIO_Status.end() || ((it->second).mode != GPIO_OUT_PP && (it->second).mode != GPIO_OUT_OD)) return;

    digitalWrite(pin, value);                           // 使用digitalWrite函数写入数据位
    (it->second).status = static_cast<int32_t>(value);  // 更新GPIO_Status中该pin的状态为value
}

/**
 *@brief 向多个GPIO写入数据
 *@param value 一个unordered_map容器，用于存储GPIO的编号和要写入的数据
 *该函数遍历value容器中的每一个元素，
 *如果GPIO_Status中存在该GPIO编号并且属于输出引脚，则设置GPIO状态。
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
    gpioWriteBit(pin, !digitalRead(pin));
    /*
    gpioWriteBit(pin, [this](uint8_t pin) -> int32_t {
        if (gpioReadDataBit(pin) == HIGH)
            return LOW;
        else
            return HIGH;
    }(pin));
    */
}

/**
 * @brief 反转一组输出GPIO的引脚电平
 * @return void
 */
void GPIOs::gpioToggleGroup() {
    for (auto &i : GPIO_Status) gpioToggleBit(i.first);
}

/**
 * @brief 从一组GPIO中移除指定的引脚
 * @param pin - 要移除的GPIO引脚.
 * 如果元素不存在，则该函数不会执行任何操作.
 * @return void
 */
void GPIOs::gpioRemoveBit(uint8_t pin) { GPIO_Status.erase(pin); }

/**
 * @brief 清空一组GPIO
 * 将Group中的所有GPIO移除.
 * @return void
 */
void GPIOs::gpioRemoveGroup() { GPIO_Status.clear(); }

void GPIOs::Set_GPIO_Mode(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);                                                           // 设置指定GPIO的工作模式
    GPIO_Status.insert(std::pair<uint8_t, GPIO_Type>(pin, GPIO_Type{-1, mode}));  // 储存GPIO引脚对应的工作模式(状态未知)
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
    auto gpio = esp32_wroom_32e_gpios.find(_pin);
    if (gpio == esp32_wroom_32e_gpios.end() || timer.empty() || (gpio->second).find(PWM_OK) == (gpio->second).end()) return;

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