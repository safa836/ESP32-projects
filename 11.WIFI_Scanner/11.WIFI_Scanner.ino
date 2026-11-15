//used esp32 devkit v1
#include <WiFi.h>

void setup() {
  Serial.begin(115200);


  WiFi.mode(WIFI_STA);

  WiFi.disconnect();

  delay(1000);

  Serial.println("\nESP32 Wi-Fi Scanner");

  int networks = WiFi.scanNetworks();

  if (networks == 0) {
    Serial.println("No networks found.");
  }
  else if (networks < 0) {
    Serial.println("Wi-Fi scan failed.");
  }
  else {
    Serial.print("Networks found: ");
    Serial.println(networks);

    Serial.println();

    for (int i = 0; i < networks; i++) {


      Serial.print("Network: ");
      Serial.println(WiFi.SSID(i));

      Serial.print("RSSI: ");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm");

      Serial.print("Channel: ");
      Serial.println(WiFi.channel(i));

      Serial.print("Encryption: ");

      switch (WiFi.encryptionType(i)) {

        case WIFI_AUTH_OPEN:
          Serial.println("Open");
          break;

        case WIFI_AUTH_WEP:
          Serial.println("WEP");
          break;

        case WIFI_AUTH_WPA_PSK:
          Serial.println("WPA");
          break;

        case WIFI_AUTH_WPA2_PSK:
          Serial.println("WPA2");
          break;

        case WIFI_AUTH_WPA_WPA2_PSK:
          Serial.println("WPA/WPA2");
          break;

        case WIFI_AUTH_WPA2_ENTERPRISE:
          Serial.println("WPA2 Enterprise");
          break;

        case WIFI_AUTH_WPA3_PSK:
          Serial.println("WPA3");
          break;

        case WIFI_AUTH_WPA2_WPA3_PSK:
          Serial.println("WPA2/WPA3");
          break;

        default:
          Serial.println("Other / Unknown");
          break;
      }

      Serial.println();
    }
  }

  WiFi.scanDelete();

  Serial.println("Scan complete.");
}

void loop() {
}
