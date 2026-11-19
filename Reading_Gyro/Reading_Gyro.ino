//used esp32 devkit v1
#include <Wire.h>
#define SDA_PIN 21
#define SCL_PIN 22

void setup(){
  Serial.begin(115200);
  delay(500);

  Wire.begin(SDA_PIN,SCL_PIN);
  Wire.setClock(100000);

  Serial.println("\n scanning i2c bus (GPIO 21 =SDA ,GPIO 22=SCL...");
}

void loop(){
  int deviceFound = 0;
  for(uint8_t address = 1 ; address< 127; address++){
  Wire.beginTransmission(address);
  uint8_t error = Wire.endTransmission();

  if (error ==0){
    Serial.print("I2C device found at 0x");
    if (address <16 ){
      Serial.print("0");
    }
    Serial.println(address,HEX);
    deviceFound++;
  } else if(error == 4){
    Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
        }
    Serial.println(address,HEX);

    }
  }
  if (deviceFound == 0) {
    Serial.println("No I2C devices found. Check 3V3, GND, SDA (21), SCL (22), and pull-ups.");
  } else {
    Serial.println("Scan complete.\n");
    }

  delay(3000);
}  
