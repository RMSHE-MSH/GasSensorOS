/**
 * @file kernel_tasks.cpp
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