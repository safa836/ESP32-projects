//used ESP32-U (ESP32-WROOM-32U)
#include <LiquidCrystal.h>

LiquidCrystal lcd(23, 22, 21, 19, 18, 17);

#define SIGNAL_OUT 25
#define SIGNAL_IN 27

hw_timer_t *measureTimer = NULL;

uint32_t timerFrequency = 0;

volatile uint64_t measuredPeriod = 0;
volatile bool newMeasurement = false;

uint64_t previousEdge = 0;
bool firstEdge = true;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


// External interrupt
void ARDUINO_ISR_ATTR onRisingEdge() {

    uint64_t currentEdge = timerRead(measureTimer);

    if (!firstEdge) {

        uint64_t period = currentEdge - previousEdge;

        if (period > 0) {

            portENTER_CRITICAL_ISR(&timerMux);

            measuredPeriod = period;
            newMeasurement = true;

            portEXIT_CRITICAL_ISR(&timerMux);
        }
    }

    previousEdge = currentEdge;
    firstEdge = false;
}


void setup() {

    Serial.begin(115200);

    lcd.begin(16, 2);

    lcd.setCursor(0, 0);
    lcd.print("Frequency:");

    // Hardware timer: 1 MHz
    measureTimer = timerBegin(1000000);

    if (measureTimer == NULL) {
        Serial.println("Timer initialization failed");
        return;
    }

    timerFrequency = timerGetFrequency(measureTimer);

    Serial.print("Timer frequency: ");
    Serial.println(timerFrequency);

    // Generate 100 Hz square wave
    if (!ledcAttach(SIGNAL_OUT, 100, 8)) {
        Serial.println("LEDC initialization failed");
        return;
    }

    ledcWrite(SIGNAL_OUT, 128);

    pinMode(SIGNAL_IN, INPUT);

    attachInterrupt(
        digitalPinToInterrupt(SIGNAL_IN),
        onRisingEdge,
        RISING
    );
}


void loop() {

    uint64_t period = 0;
    bool ready = false;

    portENTER_CRITICAL(&timerMux);

    if (newMeasurement) {
        period = measuredPeriod;
        newMeasurement = false;
        ready = true;
    }

    portEXIT_CRITICAL(&timerMux);

    if (ready && period > 0) {

        float frequency = (float)timerFrequency / period;

        Serial.print("Frequency: ");
        Serial.print(frequency);
        Serial.println(" Hz");

        lcd.setCursor(0, 1);
        lcd.print("                ");

        lcd.setCursor(0, 1);
        lcd.print(frequency, 1);
        lcd.print(" Hz");
    }
}
