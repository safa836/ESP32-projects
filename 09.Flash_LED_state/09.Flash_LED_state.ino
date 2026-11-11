//used ESP32-U (ESP32-WROOM-32U)
#include <Preferences.h>

Preferences preferences;

#define LED_PIN 18
#define BUTTON_PIN 27

bool ledState = false;

int lastButtonState = HIGH;
int buttonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  delay(1500);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  preferences.begin("storage", false);

  ledState = preferences.getBool("led", false);

  digitalWrite(LED_PIN, ledState);

  Serial.println("ESP32 Started!");
  Serial.print("Previous LED state: ");
  Serial.println(ledState ? "ON" : "OFF");
}

void loop() {

  int reading = digitalRead(BUTTON_PIN);

  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      
      if (buttonState == LOW) {

        ledState = !ledState;

        digitalWrite(LED_PIN, ledState);
        
        preferences.putBool("led", ledState);

        Serial.print("LED is now: ");
        Serial.println(ledState ? "ON" : "OFF");
      }
    }
  }

  lastButtonState = reading;
}
