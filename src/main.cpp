#include <CPPSTL.h>
#include <esp32_pin_defi.h>
#include <io.h>
#include <kernel_tasks.h>
#include <random.h>
#include <systime.h>

#include <code_interpreter.hpp>

void setup() {
    Serial.begin(115200);
    runKernelTasks();
}

void loop() {}