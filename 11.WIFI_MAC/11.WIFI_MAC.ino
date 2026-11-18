//used(ESP32-WROOM-32U)

#include <WiFi.h>
#include <esp_wifi.h>

uint8_t customMAC[] = {
  0x02, 0x11, 0x22, 0x33, 0x44, 0x55
};


void printMAC() {
  uint8_t mac[6];

  esp_err_t result = esp_wifi_get_mac(WIFI_IF_STA, mac);

  if (result == ESP_OK) {
    Serial.printf(
      "%02X:%02X:%02X:%02X:%02X:%02X\n",
      mac[0], mac[1], mac[2],
      mac[3], mac[4], mac[5]
    );
  } else {
    Serial.println("Failed to read MAC address!");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  
  WiFi.mode(WIFI_STA);
  delay(100);

  Serial.println("Original MAC Address:");
  printMAC();

  
  Serial.println("\nSetting custom MAC...");

  esp_err_t result = esp_wifi_set_mac(
    WIFI_IF_STA, customMAC
  );

  if (result == ESP_OK) {
    Serial.println("MAC changed successfully!");
  } else {
    Serial.printf("MAC change failed. Error: %d\n", result);
  }


  Serial.println("\nNew MAC Address:");
  printMAC();
}

void loop() {
}
