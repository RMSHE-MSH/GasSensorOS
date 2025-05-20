#include <command_line_interface.h>
#include <ring_buffer.h>

#include <system_boot.hpp>

Command_Line_Interface CLI;

void setup() {
    Serial.begin(115200);
    system_boot();
    // delay(4000);
}

void loop() { CLI.run(); }
