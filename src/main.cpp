#include <CPPSTL.h>
#include <esp32_pin_defi.h>
#include <io.h>
#include <kernel_tasks.h>
#include <random.h>
#include <systime.h>

#include <code_interpreter.hpp>

Interpreter inte;

void setup() {
    Serial.begin(115200);

    // runKernelTasks();
}

std::string code = "print(\"Hello GScode!\"); num a=0; num b = 1; print(a+b);";

void loop() {
    inte.interpreter(code);
    delay(1000);
}