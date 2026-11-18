// used(ESP32-WROOM-32U)

#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "MIInTernet";
const char* password = "12345678";

IPAddress local_IP(172, 20, 96, 200);
IPAddress gateway(172, 20, 96, 1);
IPAddress subnet(255, 255, 255, 0);

IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // Configure static IP before connecting
  if (!WiFi.config(local_IP, gateway, subnet,
                   primaryDNS, secondaryDNS)) {
    Serial.println("Static IP configuration failed!");
    return;
  }

  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());

  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
}

void loop() {
}
