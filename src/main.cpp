#include <command_line_interface.h>

#include <system_boot.hpp>
#include <wifi_connector.hpp>

Command_Line_Interface CLI;
WiFiConnector WIFI;

void setup() {
    Serial.begin(115200);
    system_boot();
    delay(4000);
    // WIFI.setWifiCredentials("323", "88888888");
    // WIFI.connect();
    WIFI.autoConnect();
    std::cerr << WIFI.getSSID() << "\n";
}

void loop() { CLI.run(); }
