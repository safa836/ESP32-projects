//used esp32 devkit v1

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define POT_PIN 34

void setup() {
    Serial.begin(115200);

    analogReadResolution(12);
    analogSetPinAttenuation(POT_PIN, ADC_11db);

    if (!SerialBT.begin("ESP32_DevKit_V1")) {
        Serial.println("Bluetooth initialization failed!");
        return;
    }

    Serial.println("Bluetooth ready!");
}

void loop() {
    int adcValue = analogRead(POT_PIN);

    SerialBT.print("ADC Value: ");
    SerialBT.println(adcValue);

    delay(500);
}
