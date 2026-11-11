//used esp32-c3 supermini
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Hello, World!");
}

void loop() {
  Serial.println("ESP32-C3 is running!");
  delay(1000);
}
