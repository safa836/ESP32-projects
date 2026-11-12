//used ESP32 devkit v1
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
#define LED_PIN 18

String receivedData = "";

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    SerialBT.begin("ESP32_DevKit_V1");

    Serial.println("Bluetooth ready!");
}
void loop() {
    while (SerialBT.available()) {
        char c = SerialBT.read();

        if (c == '\n' || c == '\r') {
            if (receivedData == "ON") {
                digitalWrite(LED_PIN, HIGH);
                Serial.println("LED ON");
            }
            else if (receivedData == "OFF") {
                digitalWrite(LED_PIN, LOW);
                Serial.println("LED OFF");
            }

            receivedData = "";
        }
        else {
            receivedData += c;
        }
    }
}
