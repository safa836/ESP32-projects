//used esp32 devkit v1

#include <Wire.h>

constexpr uint8_t MPU_ADDR        = 0x68; // Change to 0x69 if AD0 is HIGH
constexpr uint8_t REG_PWR_MGMT_1  = 0x6B;
constexpr uint8_t REG_GYRO_CONFIG = 0x1B;
constexpr uint8_t REG_GYRO_XOUT_H = 0x43;
constexpr uint8_t REG_WHO_AM_I    = 0x75;

// +/- 250 deg/s full scale -> 131.072 LSB per (deg/s)
constexpr float GYRO_SCALE = 131.072f;

// Calibration & filtering settings
constexpr int CAL_SAMPLES        = 200;
constexpr int MEAN_BUFFER_SIZE   = 16;
constexpr float LPF_ALPHA        = 0.25f;  // 0..1: lower = smoother, higher = faster response

// Timing settings
constexpr uint32_t SAMPLE_PERIOD_US = 10000; // 10 ms -> 100 Hz internal sampling
constexpr uint32_t PRINT_PERIOD_MS  = 250;   // 250 ms -> 4 lines/sec in Serial Monitor

// Zero-rate offsets (deg/s)
float offsetX = 0.0f;
float offsetY = 0.0f;
float offsetZ = 0.0f;

// Rolling mean buffers (deg/s)
float meanBufX[MEAN_BUFFER_SIZE] = {0};
float meanBufY[MEAN_BUFFER_SIZE] = {0};
float meanBufZ[MEAN_BUFFER_SIZE] = {0};
int bufIdx = 0;
int bufCount = 0;

// Filtered outputs (deg/s)
float finalX = 0.0f;
float finalY = 0.0f;
float finalZ = 0.0f;
bool filterReady = false;

uint32_t lastSampleMicros = 0;
uint32_t lastPrintMillis  = 0;

void writeByte(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}

uint8_t readByte(uint8_t reg) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, static_cast<uint8_t>(1), static_cast<uint8_t>(true));
  return Wire.available() ? Wire.read() : 0;
}

bool readGyroDps(float &gx, float &gy, float &gz) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(REG_GYRO_XOUT_H);
  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  if (Wire.requestFrom(MPU_ADDR, static_cast<uint8_t>(6), static_cast<uint8_t>(true)) != 6) {
    return false;
  }

  int16_t rawX = static_cast<int16_t>((Wire.read() << 8) | Wire.read());
  int16_t rawY = static_cast<int16_t>((Wire.read() << 8) | Wire.read());
  int16_t rawZ = static_cast<int16_t>((Wire.read() << 8) | Wire.read());

  gx = static_cast<float>(rawX) / GYRO_SCALE;
  gy = static_cast<float>(rawY) / GYRO_SCALE;
  gz = static_cast<float>(rawZ) / GYRO_SCALE;
  return true;
}

void calibrateOffsets() {
  float calX[CAL_SAMPLES];
  float calY[CAL_SAMPLES];
  float calZ[CAL_SAMPLES];

  Serial.println("Keep sensor completely still for calibration...");
  delay(1000);

  int count = 0;
  while (count < CAL_SAMPLES) {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    if (readGyroDps(x, y, z)) {
      calX[count] = x;
      calY[count] = y;
      calZ[count] = z;
      count++;
    }
    delay(5);
  }

  double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
  for (int i = 0; i < CAL_SAMPLES; i++) {
    sumX += calX[i];
    sumY += calY[i];
    sumZ += calZ[i];
  }

  offsetX = static_cast<float>(sumX / CAL_SAMPLES);
  offsetY = static_cast<float>(sumY / CAL_SAMPLES);
  offsetZ = static_cast<float>(sumZ / CAL_SAMPLES);

  Serial.printf("Offsets (deg/s) -> X: %.3f | Y: %.3f | Z: %.3f\n", offsetX, offsetY, offsetZ);
}

void updateRollingMean(float x, float y, float z, float &mx, float &my, float &mz) {
  meanBufX[bufIdx] = x;
  meanBufY[bufIdx] = y;
  meanBufZ[bufIdx] = z;

  bufIdx = (bufIdx + 1) % MEAN_BUFFER_SIZE;
  if (bufCount < MEAN_BUFFER_SIZE) {
    bufCount++;
  }

  float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
  for (int i = 0; i < bufCount; i++) {
    sumX += meanBufX[i];
    sumY += meanBufY[i];
    sumZ += meanBufZ[i];
  }

  mx = sumX / static_cast<float>(bufCount);
  my = sumY / static_cast<float>(bufCount);
  mz = sumZ / static_cast<float>(bufCount);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // ESP32 DevKit V1 I2C: SDA = GPIO 21, SCL = GPIO 22
  Wire.begin(21, 22);
  Wire.setClock(100000); // 100 kHz for stable breadboard/jumper operation

  Serial.printf("WHO_AM_I: 0x%02X\n", readByte(REG_WHO_AM_I));

  // Wake up sensor and select auto clock source
  writeByte(REG_PWR_MGMT_1, 0x01);
  delay(100);

  // Set gyro full-scale range to +/- 250 deg/s
  writeByte(REG_GYRO_CONFIG, 0x00);
  delay(50);

  calibrateOffsets();

  lastSampleMicros = micros();
  lastPrintMillis  = millis();
}

void loop() {
  uint32_t nowUs = micros();
  if (nowUs - lastSampleMicros >= SAMPLE_PERIOD_US) {
    lastSampleMicros += SAMPLE_PERIOD_US;

    float gx = 0.0f, gy = 0.0f, gz = 0.0f;
    if (readGyroDps(gx, gy, gz)) {
      // 1. Subtract calibrated zero-rate offset
      float corrX = gx - offsetX;
      float corrY = gy - offsetY;
      float corrZ = gz - offsetZ;

      // 2. Rolling mean over buffer
      float meanX = 0.0f, meanY = 0.0f, meanZ = 0.0f;
      updateRollingMean(corrX, corrY, corrZ, meanX, meanY, meanZ);

      // 3. Low-pass filter for final smooth X, Y, Z outputs
      if (!filterReady) {
        finalX = meanX;
        finalY = meanY;
        finalZ = meanZ;
        filterReady = true;
      } else {
        finalX = LPF_ALPHA * meanX + (1.0f - LPF_ALPHA) * finalX;
        finalY = LPF_ALPHA * meanY + (1.0f - LPF_ALPHA) * finalY;
        finalZ = LPF_ALPHA * meanZ + (1.0f - LPF_ALPHA) * finalZ;
      }
    }
  }

  uint32_t nowMs = millis();
  if (nowMs - lastPrintMillis >= PRINT_PERIOD_MS) {
    lastPrintMillis = nowMs;
    Serial.printf("Final Gyro (deg/s) -> X: %7.2f | Y: %7.2f | Z: %7.2f\n",
                  finalX, finalY, finalZ);
  }
}
