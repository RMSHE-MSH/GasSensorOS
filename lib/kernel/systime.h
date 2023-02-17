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
