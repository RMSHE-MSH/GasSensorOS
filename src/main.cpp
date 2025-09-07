#include <command_line_interface.h>

#include <system_boot.hpp>
#include <wifi_connector.hpp>

Command_Line_Interface CLI;
WiFiConnector WIFI("RMSHE Find X7", "2837271954");

void setup() {
    Serial.begin(115200);
    system_boot();
    // delay(4000);
    WIFI.connect();
}

void loop() { CLI.run(); }
