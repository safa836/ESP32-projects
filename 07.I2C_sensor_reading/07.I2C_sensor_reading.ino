//used ESP32 devkit V1
#include <Wire.h>

#define SDA_PIN 18
#define SCL_PIN 19

// MPU I2C addresses
#define MPU_ADDR_1 0x68
#define MPU_ADDR_2 0x69

// MPU registers
#define WHO_AM_I     0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG  0x1B
#define ACCEL_XOUT_H 0x3B

uint8_t MPU_ADDR = 0;

// Raw sensor values
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t temperature;

//  I2C Scanner

void scanI2C() {

    for (byte address = 1; address < 127; address++) {

        Wire.beginTransmission(address);

        if (Wire.endTransmission() == 0) {

            Serial.print("Device found: 0x");
            Serial.println(address, HEX);

            if (address == MPU_ADDR_1 ||
                address == MPU_ADDR_2) {
                MPU_ADDR = address;
            }
        }
    }
}

// Write to MPU register

void writeRegister(byte reg, byte value) {

    Wire.beginTransmission(MPU_ADDR);

    Wire.write(reg);
    Wire.write(value);

    Wire.endTransmission();
}

// Read MPU registers 

bool readRegisters(byte reg, byte count, byte* data) {

    Wire.beginTransmission(MPU_ADDR);

    Wire.write(reg);

    if (Wire.endTransmission(false) != 0) {
        return false;
    }

    byte received = Wire.requestFrom(
        MPU_ADDR, count, true
    );

    if (received != count) {
        return false;
    }

    for (byte i = 0; i < count; i++) {
        data[i] = Wire.read();
    }

    return true;
}

// ---------- Combine two bytes ----------

int16_t combineBytes(byte highByte, byte lowByte) {

    return (int16_t)((highByte << 8) | lowByte);
}

// ---------- Initialize MPU ----------

void setupMPU() {

    // Wake up sensor
    writeRegister(PWR_MGMT_1, 0x00);

    delay(100);

    // Accelerometer: +/- 2g
    writeRegister(ACCEL_CONFIG, 0x00);

    // Gyroscope: +/- 250 degrees/sec
    writeRegister(GYRO_CONFIG, 0x00);

}

// Read sensor 

void readMPU() {

    byte data[14];

    // Read 14 consecutive bytes starting at 0x3B
    if (!readRegisters(ACCEL_XOUT_H, 14, data)) {

        Serial.println("Sensor reading failed!");
        return;
    }

    // Accelerometer
    ax = combineBytes(data[0], data[1]);
    ay = combineBytes(data[2], data[3]);
    az = combineBytes(data[4], data[5]);

    // Temperature
    temperature = combineBytes(data[6], data[7]);

    // Gyroscope
    gx = combineBytes(data[8], data[9]);
    gy = combineBytes(data[10], data[11]);
    gz = combineBytes(data[12], data[13]);

    // Converting to physical units

    float AccX = ax / 16384.0;
    float AccY = ay / 16384.0;
    float AccZ = az / 16384.0;

    float GyroX = gx / 131.0;
    float GyroY = gy / 131.0;
    float GyroZ = gz / 131.0;

    float TempC = temperature / 333.87 + 21.0;

    // Print accelerometer
    Serial.println("\nAccelerometer (g):");

    Serial.print("X: "); Serial.println(AccX);
    Serial.print("Y: "); Serial.println(AccY);
    Serial.print("Z: "); Serial.println(AccZ);

    // Print gyroscope
    Serial.println("Gyroscope (deg/s:");

    Serial.print("X: "); Serial.println(GyroX);
    Serial.print("Y: "); Serial.println(GyroY);
    Serial.print("Z: "); Serial.println(GyroZ);

    // Print temperature
    Serial.println(" Temperature: ");

    Serial.print(TempC);
    Serial.println(" C");

}

void setup() {

    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN, 100000);

    Serial.println("Scanning I2C...");

    scanI2C();

    if (MPU_ADDR == 0) {

        Serial.println("MPU not found!");
        while (1) {
            delay(1000);
        }
    }

    // Read WHO_AM_I
    byte identity;

    if (!readRegisters(WHO_AM_I, 1, &identity)) {
        Serial.println("Cannot read WHO_AM_I!");
        while (1) {
            delay(10000);
        }
    }

    Serial.print("WHO_AM_I: 0x");
    Serial.println(identity, HEX);

    setupMPU();

    Serial.println("MPU initialized!");
}

void loop() {

    readMPU();

    delay(500);
}
