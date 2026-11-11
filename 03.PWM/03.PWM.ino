//used esp32-c3 supermini
#define LED_PIN 4
#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

int dutyCycle = 0;

void setup() {
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);
}

void loop() {

  while (dutyCycle < 255) {
    ledcWrite(LED_PIN, dutyCycle++);
    delay(10);
  }

  while (dutyCycle > 0) {
    ledcWrite(LED_PIN, dutyCycle--);
    delay(10);
  }
}
