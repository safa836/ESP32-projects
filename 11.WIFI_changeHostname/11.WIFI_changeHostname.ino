#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "MIInTernet";
const char* password = "12345678";

// Hostname
const char* hostname = "ESP32-Robot";

// Static IP configuration
IPAddress local_IP(172, 20, 96, 200);
IPAddress gateway(172, 20, 96, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // Set hostname before connecting
  WiFi.setHostname(hostname);

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet,
                   primaryDNS, secondaryDNS)) {
    Serial.println("IP configuration failed!");
    return;
  }

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}
