//used esp32-c3 supermini
//testing the frequncy before changing
 void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("CPU Frequency: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  Serial.print("Crystal Frequency: ");
  Serial.print(getXtalFrequencyMhz());
  Serial.println(" MHz");

  Serial.print("APB Frequency: ");
  Serial.print(getApbFrequency());
  Serial.println(" Hz");
  
//Changing the frequency
  
  setCpuFrequencyMhz(80);

  Serial.print("CPU Frequency: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");
}

void loop() {
}
