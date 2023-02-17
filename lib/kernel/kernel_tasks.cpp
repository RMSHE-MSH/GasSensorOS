#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <io.h>
#include <kernel_tasks.h>
#include <systime.h>

#define STACK_SIZE 2048
StackType_t stack[STACK_SIZE];

void sysTimeTask(void *pvParameter) {
    for (;;) {
        update_time();                          // 更新时间(每调用一次系统时间增加一秒);
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // 等待1s(每秒更新一次时间);
    }
    vTaskDelete(NULL);
}

void runKernelTasks() { xTaskCreate(sysTimeTask, "sysTimeTask", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL); }