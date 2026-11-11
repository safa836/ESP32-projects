//used ESP32-U (ESP32-WROOM-32U)

#include <Wire.h>

#define SDA_PIN 18
#define SCL_PIN 19

void scanI2C() {

    int devices = 0;

    for (byte address = 1; address < 127; address++) {

        Wire.beginTransmission(address);

        byte error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("Device found: 0x");
            Serial.println(address, HEX);
            devices++;
        }
    }

    Serial.print("Total devices: ");
    Serial.println(devices);
}

void setup() {
    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN, 100000);
}

void loop() {

    Serial.println("\nScanning at 100 kHz");
    scanI2C();

    delay(2000);

    Wire.setClock(400000);

    Serial.println("\nScanning at 400 kHz");
    scanI2C();

    delay(2000);

    Wire.setClock(100000);
}
