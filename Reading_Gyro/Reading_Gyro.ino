//used esp32 devkit v1
#include <Wire.h>

constexpr uint8_t MPU_ADDR        = 0x68; // Change to 0x69 if AD0 is HIGH
constexpr uint8_t REG_PWR_MGMT_1  = 0x6B; // Power management register
constexpr uint8_t REG_GYRO_CONFIG = 0x1B; // Gyroscope full-scale configuration
constexpr uint8_t REG_GYRO_XOUT_H = 0x43; // First byte of raw gyro X, Y, Z (6 bytes total)
constexpr uint8_t REG_WHO_AM_I    = 0x75; // Device ID register

void writeByte(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}

uint8_t readByte(uint8_t reg) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false); // Repeated start to read register
  Wire.requestFrom(MPU_ADDR, static_cast<uint8_t>(1), static_cast<uint8_t>(true));
  return Wire.available() ? Wire.read() : 0;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // ESP32 DevKit V1 I2C: SDA = GPIO 21, SCL = GPIO 22
  Wire.begin(21, 22);
  Wire.setClock(100000); // 100 kHz is reliable on breadboards/jumpers

  uint8_t whoAmI = readByte(REG_WHO_AM_I);
  Serial.printf("WHO_AM_I: 0x%02X\n", whoAmI);

  // Wake up sensor (clear sleep bit, select auto clock source)
  writeByte(REG_PWR_MGMT_1, 0x01);
  delay(100);

  // Set gyro range to +/- 250 deg/s (FS_SEL = 0 -> 0x00)
  writeByte(REG_GYRO_CONFIG, 0x00);
  delay(50);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(REG_GYRO_XOUT_H);
  Wire.endTransmission(false);

  // Read 6 bytes: X high/low, Y high/low, Z high/low
  if (Wire.requestFrom(MPU_ADDR, static_cast<uint8_t>(6), static_cast<uint8_t>(true)) == 6) {
    int16_t rawGx = static_cast<int16_t>((Wire.read() << 8) | Wire.read());
    int16_t rawGy = static_cast<int16_t>((Wire.read() << 8) | Wire.read());
    int16_t rawGz = static_cast<int16_t>((Wire.read() << 8) | Wire.read());

    Serial.printf("Raw Gyro -> X: %6d | Y: %6d | Z: %6d\n", rawGx, rawGy, rawGz);
  } else {
    Serial.println("Read error");
  }

  delay(500); // ~10 Hz print rate for easy reading
}
