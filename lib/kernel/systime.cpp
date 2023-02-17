#include <systime.h>

struct tm sys_time;

// 通过中断更新时间的方式已弃用;
/*
hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR update_time(void) {
    portENTER_CRITICAL_ISR(&timerMux);  // 保护中断代码避免被其他代码打断(代码在这里执行)

    sys_time.tm_sec++;
    if (sys_time.tm_sec >= 60) {
        sys_time.tm_sec = 0;
        sys_time.tm_min++;
    }
    if (sys_time.tm_min >= 60) {
        sys_time.tm_min = 0;
        sys_time.tm_hour++;
    }
    if (sys_time.tm_hour >= 24) {
        sys_time.tm_hour = 0;
        sys_time.tm_mday++;
    }

    unsigned char daysInMonth = 31;
    if (sys_time.tm_mon == 2) {
        // 当month等于2时, 使用三目运算符，以计算当前年份是否为闰年。如果是闰年，则daysInMonth的值将设置为29；否则为28。
        daysInMonth = (sys_time.tm_year % 400 == 0 || (sys_time.tm_year % 4 == 0 && sys_time.tm_year % 100 != 0)) ? 29 : 28;
    } else if (sys_time.tm_mon == 4 || sys_time.tm_mon == 6 || sys_time.tm_mon == 9 || sys_time.tm_mon == 11) {
        daysInMonth = 30;  // 对于4,6,9,11月只有30天,将daysInMonth的值设置为30;
    }

    // 如果天数大于当前月份的天数，代码将月份增加1，并在必要时将年份增加1;
    if (sys_time.tm_mday > daysInMonth) {
        sys_time.tm_mday = 1;
        sys_time.tm_mon++;
        if (sys_time.tm_mon > 12) {
            sys_time.tm_mon = 1;
            sys_time.tm_year++;
        }
    }

    portEXIT_CRITICAL_ISR(&timerMux);  // 结束对中断代码的保护
}

void time_begin() {
    //1. 初始化TIMER：使用timerBegin函数初始化TIMER，其中0代表TIMER0，80代表80分频，即1 us 的时钟周期，true代表自动重新加载。
    //2. 配置时钟中断：使用timerAttachInterrupt函数配置时钟中断，onTimer函数是时钟中断的回调函数，true代表进入中断时使用串行模式。
    //3. 设置时钟中断时间：使用timerAlarmWrite函数设置时钟中断时间，1000000代表每隔1s中断一次，true代表自动重新加载。
    //4. 启动时钟中断：使用timerAlarmEnable函数启动时钟中断。

    timer = timerBegin(0, 80, true);                  // 初始化定时器;
    timerAttachInterrupt(timer, &update_time, true);  // 设置时钟中断门;
    timerAlarmWrite(timer, 1000000, true);            // 设置中断时间(1s中断);
    timerAlarmEnable(timer);                          // 开启时钟中断;
}
*/

void IRAM_ATTR update_time(void) {
    sys_time.tm_sec++;
    if (sys_time.tm_sec >= 60) {
        sys_time.tm_sec = 0;
        sys_time.tm_min++;
    }
    if (sys_time.tm_min >= 60) {
        sys_time.tm_min = 0;
        sys_time.tm_hour++;
    }
    if (sys_time.tm_hour >= 24) {
        sys_time.tm_hour = 0;
        sys_time.tm_mday++;
    }

    unsigned char daysInMonth = 31;
    if (sys_time.tm_mon == 2) {
        // 当month等于2时, 使用三目运算符，以计算当前年份是否为闰年。如果是闰年，则daysInMonth的值将设置为29；否则为28。
        daysInMonth = (sys_time.tm_year % 400 == 0 || (sys_time.tm_year % 4 == 0 && sys_time.tm_year % 100 != 0)) ? 29 : 28;
    } else if (sys_time.tm_mon == 4 || sys_time.tm_mon == 6 || sys_time.tm_mon == 9 || sys_time.tm_mon == 11) {
        daysInMonth = 30;  // 对于4,6,9,11月只有30天,将daysInMonth的值设置为30;
    }

    // 如果天数大于当前月份的天数，代码将月份增加1，并在必要时将年份增加1;
    if (sys_time.tm_mday > daysInMonth) {
        sys_time.tm_mday = 1;
        sys_time.tm_mon++;
        if (sys_time.tm_mon > 12) {
            sys_time.tm_mon = 1;
            sys_time.tm_year++;
        }
    }
}

void time_init(time_t time) { sys_time = *localtime(&time); }

time_t int_time(void) { return mktime(&sys_time); }

String str_time() { return asctime(&sys_time); }

tm tm_time() { return sys_time; }