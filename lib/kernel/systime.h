/**
 * @file systime.h
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
#include <Arduino.h>

// 通过中断更新时间的方式已弃用;
/*
// 当前代码实现了对系统日期的更新
void IRAM_ATTR update_time(void);

// 通过定时器中断使时间更新(这个程序将每秒更新一次系统时间);
void time_begin();
*/

// 当前代码实现了对系统日期的更新
void IRAM_ATTR update_time(void);

// 初始化或设置系统时间(接受"time_t"类型的数据);
void time_init(time_t time);

// 返回当前时间的"tm"结构体
tm tm_time(void);

// 返回当前时间的"time_t"类型(它一个整型数据类型，表示从 1970 年 1 月 1 日 00:00:00 至今的秒数)
time_t int_time(void);

// 返回当前时间的字符串类型;
String str_time(void);
