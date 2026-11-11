//used ESP32-U (ESP32-WROOM-32U)
#define LED_PIN 18

hw_timer_t *timer = NULL;

volatile bool timerFlag = false;

void ARDUINO_ISR_ATTR onTimer() {
    timerFlag = true;
}

void setup() {

    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);

    
    timer = timerBegin(1000000);

    timerAttachInterrupt(timer, &onTimer);

    
    timerAlarm(timer, 2000000, true, 0); //lED turns on each 2 seconds
}

void loop() {

    if (timerFlag) {

        timerFlag = false;

        digitalWrite(LED_PIN, !digitalRead(LED_PIN));

        Serial.println("Timer interrupt!");
    }
}
