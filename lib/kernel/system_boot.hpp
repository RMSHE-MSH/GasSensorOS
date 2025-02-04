
#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <systime.h>

// 时间更新任务
void update_time_task(void* pvParameters) {
    while (true) {
        update_time();
        vTaskDelay(pdMS_TO_TICKS(1000));  // 延迟1秒
    }
}

void system_boot() {
    // 初始化系统时间为 2025-01-01 00:00:00
    time_t initial_time = 1672531200;  // 2025-01-01 00:00:00 的 UNIX 时间戳
    time_init(initial_time);

    // 创建时间更新任务
    xTaskCreate(update_time_task, "UpdateTimeTask", 2048, NULL, 1, NULL);
}