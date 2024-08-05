/**
 * @file OLED.cpp
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

#include <OLED.h>

// I2C初始化
void OLED::I2C_Init() {
    SDA_OUT;
    SCL_OUT;
    I2C_SDA_H;
    I2C_SCL_H;
}

// I2C开始
void OLED::I2C_Start() {
    I2C_SCL_H;
    I2C_SDA_H;
    I2C_SDA_L;
    I2C_SCL_L;
}

// I2C结束
void OLED::I2C_Stop() {
    I2C_SCL_H;
    I2C_SDA_L;
    I2C_SDA_H;
}

// 检查应答信号
void OLED::I2C_Wait_Ack() {
    I2C_SDA_H;
    I2C_SCL_H;
    I2C_SCL_L;
}

// I2C写入一个字节
void OLED::Write_I2C_Byte(unsigned char dat) {
    unsigned char i;

    for (i = 0; i < 8; i++) {
        I2C_SCL_L;
        if (dat & 0x80) {
            I2C_SDA_H;
        } else {
            I2C_SDA_L;
        }
        dat <<= 1;
        I2C_SCL_H;
    }
    I2C_SCL_L;
}

// 发送一个字节
// 向SSD1306写入一个字节。
// mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED::OLED_WR_Byte(uint8_t dat, uint8_t mode) {
    // 如果开启预渲染则图形数据将先缓存在OLED_GDDRAM_CLONE中, 不会立刻显示到屏幕上;
    if (PreRendered == true && mode == OLED_DATA) {
        SET_OLED_GDDRAM_CLONE(dat);  // 更新OLED_GDDRAM_CLONE中的数据;
    } else if (PreRendered == false) {
        I2C_Start();
        Write_I2C_Byte(0x78);
        I2C_Wait_Ack();
        if (mode == OLED_DATA) {
            SET_OLED_GDDRAM_CLONE(dat);  // 更新OLED_GDDRAM_CLONE中的数据;
            Write_I2C_Byte(0x40);
        } else {
            Write_I2C_Byte(0x00);
        }
        I2C_Wait_Ack();
        Write_I2C_Byte(dat);
        I2C_Wait_Ack();
        I2C_Stop();
    }
}

// data: 为OLED某页中的某一列8位数据(1字节);
void OLED::SET_OLED_GDDRAM_CLONE(unsigned char BytesData) {
    OLED_GDDRAM_CLONE[OLED_Pos.y][OLED_Pos.x] = BytesData;  // [当前点在哪页][当前点的列位置]; 更新OLED_GDDRAM_CLONE中的数据;
}

// 反显函数
void OLED::OLED_ColorTurn(uint8_t i) {
    if (!i)
        OLED_WR_Byte(0xA6, OLED_CMD);  // 正常显示
    else
        OLED_WR_Byte(0xA7, OLED_CMD);  // 反色显示
}

// 屏幕旋转180度
void OLED::OLED_DisplayTurn(uint8_t i) {
    if (i == 0) {
        OLED_WR_Byte(0xC8, OLED_CMD);  // 正常显示
        OLED_WR_Byte(0xA1, OLED_CMD);
    } else {
        OLED_WR_Byte(0xC0, OLED_CMD);  // 反转显示
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

// 坐标设置
void OLED::OLED_Set_Pos(uint8_t x, uint8_t y) {
    OLED_Pos = {x, y};
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

// 开启OLED显示
void OLED::OLED_Display_On(void) {
    OLED_WR_Byte(0X8D, OLED_CMD);  // SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD);  // DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD);  // DISPLAY ON
}

// 关闭OLED显示
void OLED::OLED_Display_Off(void) {
    OLED_WR_Byte(0X8D, OLED_CMD);  // SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD);  // DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD);  // DISPLAY OFF
}

// 清屏函数;
void OLED::OLED_Clear(void) {
    for (uint8_t i = 0; i < 8; ++i) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);  // 设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);      // 设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);      // 设置显示位置—列高地址
        for (uint8_t n = 0; n < 128; ++n) OLED_WR_Byte(0, OLED_DATA);
    }  // 更新显示
}

// 在指定位置显示一个字符
// x:0~127
// y:0~7
// sizey:选择字体 6x8  8x16 16x32
void OLED::OLED_ShowChar(uint8_t x, uint8_t y, const uint8_t chr, uint8_t sizey) {
    uint8_t c = 0, sizex = sizey / 2, temp;
    uint16_t i = 0, size1;
    if (sizey == 8)
        size1 = 5;
    else
        size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * (sizey / 2);
    c = chr - ' ';  // 得到偏移后的值
    OLED_Set_Pos(x, y);
    for (i = 0; i < size1; i++) {
        if (i % sizex == 0 && sizey != 8) OLED_Set_Pos(x, y++);
        if (sizey == 8) {
            temp = pgm_read_byte(&GSOS_ASCII_0805[c][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 5*8 GSOS_ASCII字符集点阵字体;
        } else if (sizey == 16) {
            temp = pgm_read_byte(&asc2_1608[c][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 8x16字号
        } else if (sizey == 32) {
            temp = pgm_read_byte(&asc2_3216[c][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 16x32字号(Consolas)
        } else if (sizey == 64) {
            temp = pgm_read_byte(&asc2_6432[c][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 32x64字号(Consolas)
        } else if (sizey == 36) {
            temp = pgm_read_byte(&asc2_3618[c][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 18x36字号(Consolas)
        } else if (sizey == 48) {
            temp = pgm_read_byte(&asc2_4824[c][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 24x48字号(Consolas)
        } else if (sizey == 49) {
            temp = pgm_read_byte(&asc2_Digital_2448[c][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 24x48字号(DS-Digital)
        } else
            return;

        // 设置列坐标
        OLED_Pos = {static_cast<unsigned char>(static_cast<unsigned short>(x) + i), y};
    }
}

// m^n函数
uint32_t OLED::oled_pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--) result *= m;
    return result;
}

// 显示数字
// x,y :起点坐标
// num:要显示的数字
// len :数字的位数
// sizey:字体大小
void OLED::OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey) {
    uint8_t t, temp, m = 0;
    uint8_t enshow = 0;
    if (sizey == 8) m = 2;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (sizey / 2 + m) * t, y, ' ', sizey);
                continue;
            } else
                enshow = 1;
        }
        OLED_ShowChar(x + (sizey / 2 + m) * t, y, temp + '0', sizey);
    }
}

// 显示一个字符串
void OLED::OLED_ShowString(uint8_t x, uint8_t y, const char *chr, uint8_t sizey) {
    uint8_t j = 0;
    while (chr[j] != '\0') {
        OLED_ShowChar(x, y, chr[j++], sizey);
        if (sizey == 8)
            x += 6;
        else
            x += sizey / 2;
    }
}

// 显示汉字
void OLED::OLED_ShowChinese(uint8_t x, uint8_t y, const uint8_t no, uint8_t sizey) {
    uint16_t i, size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    uint8_t temp;
    for (i = 0; i < size1; i++) {
        if (i % sizey == 0) OLED_Set_Pos(x, y++);
        if (sizey == 32) {
            temp = pgm_read_byte(&FireWarning_32x32[no][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 32x32字号
        } else if (sizey == 64) {
            temp = pgm_read_byte(&FireWarning_32x64[no][i]);
            OLED_WR_Byte(temp, OLED_DATA);  // 32x64字号
        } else
            return;
    }
}

// 显示图片
// x,y显示坐标
// sizex,sizey,图片长宽
// BMP：要显示的图片
void OLED::OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, const uint8_t BMP[]) {
    uint16_t j = 0;
    uint8_t i, m, temp;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (i = 0; i < sizey; i++) {
        OLED_Set_Pos(x, i + y);
        for (m = 0; m < sizex; m++) {
            temp = pgm_read_byte(&BMP[j++]);
            OLED_WR_Byte(temp, OLED_DATA);
        }
    }
}

// OLED的初始化
void OLED::OLED_Init(void) {
    I2C_Init();  // 初始化I2C 管脚输出高电平

    OLED_WR_Byte(0xAE, OLED_CMD);  //--turn off oled panel
    OLED_WR_Byte(0x00, OLED_CMD);  //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD);  //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD);  //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD);  //--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD);  // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD);  //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8, OLED_CMD);  // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6, OLED_CMD);  //--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD);  //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD);  //--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD);  //-set display offset Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD);  //-not offset
    OLED_WR_Byte(0xd5, OLED_CMD);  //--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD);  //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD);  //--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD);  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD);  //--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD);  //--set vcomh
    OLED_WR_Byte(0x40, OLED_CMD);  // Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD);  //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD);  //
    OLED_WR_Byte(0x8D, OLED_CMD);  //--set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD);  //--set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD);  // Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD);  // Disable Inverse Display On (0xa6/a7)
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

/*---------------------------------------------------------------------------------------------------------*/

// 使用OLED_GDDRAM_CLONE刷新屏幕,可以将内容预渲染在OLED_GDDRAM_CLONE中然后再一次刷新(刷新区域, 默认全屏);
void OLED::OLED_GDDRAM_Refresh(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel) {
    //(Pixel >> 3): 计算"topPixel"与"bottomPixel"坐标对应的点位于哪一页;
    unsigned char topPage = static_cast<unsigned char>(topPixel >> 3);
    unsigned char bottomPage = static_cast<unsigned char>(bottomPixel >> 3);

    for (uint8_t i = topPage; i < bottomPage; ++i) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);  // 设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);      // 设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);      // 设置显示位置—列高地址
        for (uint8_t n = leftPixel; n < rightPixel; ++n) {
            OLED_WR_Byte(OLED_GDDRAM_CLONE[i][n], OLED_DATA);
        }
    }  // 更新显示
}

// 开始进行批量绘制(开始预渲染);
void OLED::BeginBatchDraw() { PreRendered = true; }

// 结束进行批量绘制(结束预渲染, 不会自动刷新预渲染缓存的内容);
void OLED::EndBatchDraw() { PreRendered = false; }

// 这个函数用于画点(点的坐标, enabl:true在屏幕上画一点, enabl:false清除屏幕上的一点);
void OLED::putpixel(uint8_t xPixel, uint8_t yPixel, bool enable) {
    if (xPixel > 127 || yPixel > 63) return;  // 非法参数(超出OLED显示区域);

    // 每页的一列中有8个像素，计算待显示的点在当前列中处于第几点, 然后以此对字节0x01进行移位(得到一个字节的图形数据);
    unsigned char Byte_data = static_cast<unsigned char>(0x01 << (yPixel % 8));

    //(yPixel >> 3): 计算"yPixel"坐标对应的点位于哪一页;
    unsigned char Page_Pos = static_cast<unsigned char>(yPixel >> 3);

    // 因为OLED是字节操作的无法真正的进行位操作, 所有我们读取"OLED_GDDRAM_CLONE"中的OLED上已经显示的数据,
    // 把同一位置将要显示的字节和已经显示进行位运算来达到只变更一位的目的;
    if (enable == true) {
        Byte_data |= OLED_GDDRAM_CLONE[Page_Pos][xPixel];  // 第Bit_xPixel(yPixel % 8)位，置1，其他位值不变;
    } else {
        Byte_data = OLED_GDDRAM_CLONE[Page_Pos][xPixel] & (~Byte_data);  // 第Bit_xPixel(yPixel % 8)位，置0，其他位值不变;
    }

    if (Byte_data == OLED_GDDRAM_CLONE[Page_Pos][xPixel]) return;  // 检查该点是否已经被画过(避免重复画点);

    OLED_Set_Pos(xPixel, Page_Pos);

    OLED_WR_Byte(static_cast<unsigned char>(0xb0 + Page_Pos), OLED_CMD);
    OLED_WR_Byte(static_cast<unsigned char>(((xPixel & 0xf0) >> 4) | 0x10), OLED_CMD);
    OLED_WR_Byte(static_cast<unsigned char>((xPixel & 0x0f) | 0x00), OLED_CMD);

    OLED_WR_Byte(Byte_data, OLED_DATA);
}

// 这个函数用于画直线;
void OLED::line(uint8_t x1Pixel, uint8_t y1Pixel, uint8_t x2Pixel, uint8_t y2Pixel, bool enable) {
    unsigned char dx = abs(x2Pixel - x1Pixel);
    unsigned char dy = abs(y2Pixel - y1Pixel);
    short sx = (x1Pixel < x2Pixel) ? 1 : -1;
    short sy = (y1Pixel < y2Pixel) ? 1 : -1;
    short err = (dx > dy ? dx : -dy) / 2;
    short e2;

    while (true) {
        putpixel(x1Pixel, y1Pixel, enable);
        if (x1Pixel == x2Pixel && y1Pixel == y2Pixel) break;
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x1Pixel += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1Pixel += sy;
        }
    }
}

// 这个函数用于画无填充的矩形;
void OLED::rectangle(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel) {
    line(leftPixel, topPixel, rightPixel, topPixel);
    line(leftPixel, topPixel, leftPixel, bottomPixel);
    line(rightPixel, topPixel, rightPixel, bottomPixel);
    line(leftPixel, bottomPixel, rightPixel, bottomPixel);
}

// 这个函数用于画有边框的填充矩形;
void OLED::fillrectangle(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel) {
    // Draw the four sides of the rectangle
    line(leftPixel, topPixel, rightPixel, topPixel);
    line(leftPixel, topPixel, leftPixel, bottomPixel);
    line(rightPixel, topPixel, rightPixel, bottomPixel);
    line(leftPixel, bottomPixel, rightPixel, bottomPixel);

    // Fill the rectangle
    for (unsigned char i = topPixel; i <= bottomPixel; i++) {
        line(leftPixel, i, rightPixel, i);
    }
}

// 这个函数用于清空矩形区域;
void OLED::clearrectangle(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel) {
    for (unsigned char y = topPixel; y <= bottomPixel; ++y) {
        for (unsigned char x = leftPixel; x <= rightPixel; ++x) {
            putpixel(x, y, false);
        }
    }
}

// 字符串分割, 将一个给定的字符串 "input" 根据另一个字符串 "separator" 进行分割，并将分割后的每一段字符串存储在一个 vector 容器中。
vector<String> OLED::strsplit(String input, String separator) {
    vector<String> vecStr;
    while (input.indexOf(separator) != -1) {
        unsigned int splitIndex = input.indexOf(separator);
        String segment = input.substring(0, splitIndex);
        vecStr.push_back(segment);
        input = input.substring(splitIndex + separator.length());
    }
    vecStr.push_back(input);
    return vecStr;
    /*
    该函数的作用是将一个给定的字符串 "input" 根据另一个字符串 "separator" 进行分割，并将分割后的每一段字符串存储在一个 vector 容器中。

    这个函数首先使用 while 循环来检查输入字符串中是否存在 "separator"，如果存在，则使用 "input.indexOf(separator)" 方法来找到 "separator"
    第一次出现的位置，然后使用 "input.substring(0, splitIndex)" 方法来截取从 0 到 "splitIndex" 位置的字符串，将截取的字符串存储在 vector 容器 "vecStr"
    中。接着，使用 "input = input.substring(splitIndex + 分割字符的长度);" 更新输入字符串，以便在下一次循环中继续查找 "separator"。

    当循环结束后，将剩余的字符串 "input" 添加到 vector 容器 "vecStr" 中，并返回该容器。
    */
}

// 渲染文本箱;
void OLED::drawPrintBox() {
    BeginBatchDraw();  // 开始预渲染;

    // 计算当前滚动条位置;
    sliderPos[0] = static_cast<unsigned char>(8 * Hight_MaxNumChar * (static_cast<float>(First_Line) / static_cast<float>(PrintBox.size())));

    line(PrintRECT.right - 5, PrintRECT.top, PrintRECT.right - 5, PrintRECT.bottom);                  // 绘制滚动条栏;
    clearrectangle(PrintRECT.right - 3, sliderPos[1], PrintRECT.right, sliderPos[1] + sliderHeight);  // 清空上一个滚动条区域;
    rectangle(PrintRECT.right - 3, sliderPos[0], PrintRECT.right - 1, sliderPos[0] + sliderHeight);   // 绘制滚动条;

    sliderPos[1] = sliderPos[0];  // 更新滚动条位置;

    // 绘制字符;
    unsigned char y = PrintRECT.top;  // 将输入指针移到文本框最顶端;

    // 显示PrintBox中First_Line到Hight_MaxNumChar的内容;
    for (unsigned int i = First_Line; i < First_Line + [](unsigned int PrintBox_size, unsigned int Hight_MaxNumChar) -> unsigned int {
             // 这里要做一个比较, 如果PrintBox_size小于Hight_MaxNumChar则显示从首行到尾行的内容(否则会因为强制显示不存在的内容而发生错误!);
             if (PrintBox_size < Hight_MaxNumChar)
                 return PrintBox_size;
             else
                 return Hight_MaxNumChar;
         }(PrintBox.size(), Hight_MaxNumChar);
         ++i) {
        unsigned char x = PrintRECT.left;
        for (auto &j : PrintBox[i]) {
            OLED_ShowChar(x, y, j, 8);
            x += 5;
        }
        ++y;
    }

    EndBatchDraw();                                                                         // 结束预渲染;
    OLED_GDDRAM_Refresh(PrintRECT.left, PrintRECT.top, PrintRECT.right, PrintRECT.bottom);  // 刷新预渲染内容到屏幕上;
    /*
    unsigned char x = PrintRECT.left;
    unsigned char y = PrintRECT.top;

    // 计算一行显示最多字符时的像素长度("-5"这个位置用于显示滚动条);
    unsigned char PrintWidth = static_cast<unsigned char>(0.2 * (PrintRECT.right - PrintRECT.left)) * 5 - 5;
    unsigned char PrintHight = static_cast<unsigned char>(0.125 * (PrintRECT.bottom - PrintRECT.top));  // 计算一列显示最多字符时的像素长度;


    unsigned int Newline_Pos = text.indexOf("\n", 0);

    for (auto &i : text) {
        // 获取迭代器下标;
        unsigned int Text_Pos = static_cast<unsigned int>(std::distance(text.begin(), &i));

        // 如果右方超出显示区域或遇到换行符则换行;
        if (x >= PrintWidth || Text_Pos == Newline_Pos) {
            // 查找下一个换行符在字符串中的位置(从上一个换行符后开始查找);
            Newline_Pos = text.indexOf("\n", Newline_Pos + 1);

            x = PrintRECT.left;
            ++y;
            // 如果显示行数超出显示区域则从开位置头显示剩余的字符;
            if (y >= PrintHight) {
                x = PrintRECT.left;
                y = PrintRECT.top;
            }
        } else {
            OLED_ShowChar(x, y, i, 8);
            x += 5;
        }
    }
    */
}

// 设置文本框(文本框左部 x 坐标, 文本框顶部 y 坐标, 文本框右部 x 坐标,文本框底部 y 坐标);
void OLED::setTextBox(uint8_t leftPixel, uint8_t topPixel, uint8_t rightPixel, uint8_t bottomPixel) {
    PrintRECT = {leftPixel, topPixel, rightPixel, bottomPixel};  // 设置文本框位置和大小;
    First_Line = 0;                                              // 首行位置设为0;

    // 若PrintBox不为空, 则清空释放其内存;
    if (PrintBox.empty() == false) {
        PrintBox.clear();
        PrintBox.shrink_to_fit();
    }
}

// 打印字符串(字符串, 自动滚动[设为true时会随着打印内容而自动向下滚动, 默认启用]);
void OLED::print(String text, bool autoScroll) {
    // 清空文本框区域(但是不要立刻显示出来);
    BeginBatchDraw();
    clearrectangle(PrintRECT.left, PrintRECT.top, PrintRECT.right, PrintRECT.bottom);
    EndBatchDraw();

    /*-------------------------------------------------------------------------------------------------------------------------------
这里实现的是首先按照换行符换行, 换行符换行完成后检查是否有某行超出显示范围, 如果有则再对其换行.
这段代码主要的作用是将文本字符串根据换行符和一行最大字符数分割成若干行,

1.首先, 通过计算 PrintRECT 的宽度和高度，计算出最大字符数 Width_MaxNumChar 和最大行数 Hight_MaxNumChar。

2.然后，通过strsplit函数将文本按照'\n'分割成一个数组。

3.接下来，对于每一个分割后的字符串，使用 for 循环将其分割成若干段，每一段不会超过Width_MaxNumChar个字符。

4.对于每一段，使用匿名函数计算出需要填充的空格数，并将这一段文本加上空格作为一个字符串插入到PrintBox中。

5.整个过程循环进行直到所有的文本都被分割完成。

'\n' 在代码中的作用是用来将文本分割成若干行, 这样就可以按照行来进行分割。
--------------------------------------------------------------------------------------------------------------------------------*/

    // 计算一行最多能显示多少字符("-1"是为了腾出滚动条的显示空间);
    Width_MaxNumChar = static_cast<unsigned char>(0.2 * (PrintRECT.right - PrintRECT.left)) - 1;
    // 计算一列最多能显示多少字符;
    Hight_MaxNumChar = static_cast<unsigned char>(0.125 * (PrintRECT.bottom - PrintRECT.top));

    // 通过换行符"\n"分割字符串成若干行;
    for (auto &i : strsplit(text, "\n")) {
        // 每行的字符串按照"Width_MaxNumChar"的字符数分割成若干小字符串行;
        for (unsigned int j = 0; j < i.length(); j += Width_MaxNumChar) {
            PrintBox.push_back(i.substring(j, min(j + Width_MaxNumChar, i.length())) + [](unsigned int length, unsigned char Width_MaxNumChar) -> String {
                // 每行的末尾可能会没有内容, 这样在文本滚动时造成显示错误, 我们对其添加空格来进行填充;
                String SpaceChar = "";
                for (unsigned int k = length; k < Width_MaxNumChar; ++k) SpaceChar += " ";
                return SpaceChar;
            }(i.length(), Width_MaxNumChar));
        }
    }

    // 计算滚动条高度;
    sliderHeight = static_cast<unsigned char>(8 * Hight_MaxNumChar * (static_cast<float>(Hight_MaxNumChar) / static_cast<float>(PrintBox.size())));

    // 如果启用自动滚动并且文本行数超过屏幕能够显示的行数则滚动到文本最底部;
    if (autoScroll == true && PrintBox.size() > Hight_MaxNumChar) First_Line = PrintBox.size() - Hight_MaxNumChar;

    drawPrintBox();  // 渲染文本箱;
}

// 清空文本框并释放内存;
void OLED::clearTextBox() {
    PrintBox.clear();
    PrintBox.shrink_to_fit();
    First_Line = 0;  // 首行位置设为0;
};

// 获取文本框的全部文本;
vector<String> OLED::getPrintBox() { return PrintBox; }

// 用一个新的PrintBox结构的数据替换掉现有的PrintBox(要求保证数据结构正确);
void OLED::replacePrintBox(vector<String> newPrintBox) { PrintBox = newPrintBox; }

// 移动滚动条(true:向下滚动, falas:向上滚动);
void OLED::moveScrollBar(bool direction) {
    if (direction == true && (First_Line + Hight_MaxNumChar) < PrintBox.size()) {
        ++First_Line;
    } else if (direction == false && First_Line > 0) {
        --First_Line;
    }
    drawPrintBox();
    delay(1);
}