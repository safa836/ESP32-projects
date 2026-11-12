//used esp32 devkit v1

#include <WiFi.h>

const char* ssid = "MIInTernet";
const char* password = "notyouanymore";

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startTime < 15000) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected!");

        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        Serial.print("MAC Address: ");
        Serial.println(WiFi.macAddress());

        Serial.print("Signal Strength: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    }
    else {
        Serial.println("\nConnection failed!");
    }
}

void loop() {
}
