//used esp32 devkit v1
#include <WiFi.h>

const char* ssid = "MIInTernet";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println("Starting RSSI measurements...");
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    int rssi = WiFi.RSSI();

    Serial.print("Signal Strength: ");
    Serial.print(rssi);
    Serial.println(" dBm");

  }
  else {
    Serial.println("Wi-Fi disconnected!");
  }

  delay(1000);
}
