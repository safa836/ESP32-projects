//used esp32 devkit v1

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!SerialBT.begin("ESP32_DevKit_V1")) {
        Serial.println("Bluetooth initialization failed!");
        return;
    }

    Serial.println("Bluetooth Classic is ready!");
}

void loop(){
  SerialBT.println("ESP32 here!");
  delay(1000);
}
