/**
 * @file OLED.h
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
 *
 * @note 请注意: 本文件的部分代码并不是我写的, 但是我找不到这些代码的原作者, 如果您是原作者的话请联系我. Please note: I did not write some of the code in
 * this document, but I cannot find the original author of the code, so please contact me if you are the original author.
 */

#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <oledfont.h>

#include <cstdint>
#include <vector>

// I2C管脚的定义;
#define SDA 21
#define SCL 22

using namespace std;

// 输出电平的宏定义
#define I2C_SCL_L digitalWrite(SCL, LOW)  // SCL
#define I2C_SCL_H digitalWrite(SCL, HIGH)

#define I2C_SDA_L digitalWrite(SDA, LOW)  // SDA
#define I2C_SDA_H digitalWrite(SDA, HIGH)

// 管脚模式的宏定义
#define SCL_IN pinMode(SCL, INPUT)
#define SCL_OUT pinMode(SCL, OUTPUT)  // SCL
#define SDA_IN pinMode(SDA, INPUT)
#define SDA_OUT pinMode(SDA, OUTPUT)  // SDA

#define OLED_CMD 0   // 写命令
#define OLED_DATA 1  // 写数据

// 坐标点类型定义;
typedef struct POINT {
    unsigned char x;
    unsigned char y;
} POINT;

// 矩形区域类型定义;
typedef struct RECT {
    unsigned char left;
    unsigned char top;
    unsigned char right;
    unsigned char bottom;
} RECT;

class OLED {
   private:
    POINT OLED_Pos = {0, 0};  // OLED像素指针位置;

    unsigned int First_Line = 0;                                  // 文本打印区域的首行在PrintBox中的位置;
    RECT PrintRECT = {0, 0, 128, 64};                             // 文本打印区域;
    unsigned char Width_MaxNumChar, Hight_MaxNumChar;             // 一行最多能显示多少字符, 一列最多能显示多少字符;
    unsigned char sliderHeight;                                   // 滚动条高度;
    unsigned char sliderPos[2] = {PrintRECT.top, PrintRECT.top};  // 滚动位置{当前位置, 上一个位置};
    vector<String> PrintBox;                                      // 储存打印字符串的容器;

    // "OLED_GDDRAM_CLONE"是"SSD1306 GDDRAM"的克隆数据(这里的数据总与OLED上显示的内容保持一致);
    // 由于此OLED屏幕不能按位更新, 因此待显示字节通过与这里的数据进行位运算实现显示屏位更新;
    unsigned char OLED_GDDRAM_CLONE[8][128] = {
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page0;
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page1;
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page2;
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page3;
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page4;
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page5;
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page6;
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },  // Page7;
    };

    // 预渲染开关(true:启动预渲染待显示内容将会先缓存到OLED_GDDRAM_CLONE中,当设为false时将会把OLED_GDDRAM_CLONE的内容显示到屏幕上);
    bool PreRendered = false;

    // 按字节更新OLED_GDDRAM_CLONE中的信息;
    void SET_OLED_GDDRAM_CLONE(unsigned char BytesData);

    // 渲染文本箱;
    void drawPrintBox();

   public:
    // 字符串分割, 将一个给定的字符串 "input" 根据另一个字符串 "separator" 进行分割，并将分割后的每一段字符串存储在一个 vector 容器中。
    vector<String> strsplit(String input, String separator);

    // 将OLED_GDDRAM_CLONE中的内容刷新到屏幕上(可以指定刷新区域, 默认全屏刷新);
    void OLED_GDDRAM_Refresh(uint8_t leftPixel = 0, uint8_t topPixel = 0, uint8_t rightPixel = 128, uint8_t bottomPixel = 64);

    // 开始进行批量绘制(开始预渲染);
    void BeginBatchDraw();

    // 结束进行批量绘制(结束预渲染, 不会自动刷新预渲染缓存的内容);
    void EndBatchDraw();

    // 这个函数用于画点(点的坐标, enabl:true在屏幕上画一点, enabl:false清除屏幕上的一点);
    void putpixel(uint8_t xPixel, uint8_t yPixel, bool enabl = true);

    // 这个函数用于画直线;
    void line(uint8_t x1Pixel, uint8_t y1Pixel, uint8_t x2Pixel, uint8_t y2Pixel, bool enable = true);

    // 这个函数用于画无填充的矩形;
    void rectangle(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel);

    // 这个函数用于画有边框的填充矩形;
    void fillrectangle(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel);

    // 这个函数用于清空矩形区域;
    void clearrectangle(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel);

    // 设置文本框(文本框左部 x 坐标, 文本框顶部 y 坐标, 文本框右部 x 坐标,文本框底部 y 坐标);
    void setTextBox(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel);

    // 打印字符串(字符串, 自动滚动[设为true时会随着打印内容而自动向下滚动, 默认启用]);
    void print(String text, bool autoScroll = true);

    // 清空文本框并释放内存;
    void clearTextBox();

    // 获取文本框的全部文本;
    vector<String> getPrintBox();

    // 用一个新的PrintBox结构的数据替换掉现有的PrintBox(要求保证数据结构正确);
    void replacePrintBox(vector<String> newPrintBox);

    // 移动滚动条一行(true:向下滚动, falas:向上滚动);
    void moveScrollBar(bool direction = true);

   private:
    // I2C初始化
    void I2C_Init();

    // I2C开始
    void I2C_Start();

    // I2C结束
    void I2C_Stop();

    // 检查应答信号
    void I2C_Wait_Ack();

    // I2C写入一个字节
    void Write_I2C_Byte(unsigned char dat);

    // 发送一个字节
    // 向SSD1306写入一个字节。
    // mode:数据/命令标志 0,表示命令;1,表示数据;
    void OLED_WR_Byte(uint8_t dat, uint8_t mode);

    // 坐标设置
    void OLED_Set_Pos(uint8_t x, uint8_t y);

    // m^n函数
    uint32_t oled_pow(uint8_t m, uint8_t n);

   public:
    // 反显函数
    void OLED_ColorTurn(uint8_t i);

    // 屏幕旋转180度
    void OLED_DisplayTurn(uint8_t i);

    // 开启OLED显示
    void OLED_Display_On(void);

    // 关闭OLED显示
    void OLED_Display_Off(void);

    // 清屏函数;
    void OLED_Clear(void);

    // 在指定位置显示一个字符
    // x:0~127
    // y:0~7
    // sizey:选择字体 6x8  8x16
    void OLED_ShowChar(uint8_t x, uint8_t y, const uint8_t chr, uint8_t sizey);

    // 显示数字
    // x,y :起点坐标
    // num:要显示的数字
    // len :数字的位数
    // sizey:字体大小
    void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey);

    // 显示一个字符串
    void OLED_ShowString(uint8_t x, uint8_t y, const char *chr, uint8_t sizey);

    // 显示汉字
    void OLED_ShowChinese(uint8_t x, uint8_t y, const uint8_t no, uint8_t sizey);

    // 显示图片
    // x,y显示坐标
    // sizex,sizey,图片长宽
    // BMP：要显示的图片
    void OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, const uint8_t BMP[]);

    // OLED的初始化
    void OLED_Init(void);
};