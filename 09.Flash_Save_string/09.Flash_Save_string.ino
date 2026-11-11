//used(ESP32-WROOM-32U)

#include <Preferences.h>

Preferences preferences;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("ESP32 Started");


  preferences.begin("storage", false);


  String myString = "Hello From ESP32!";


  preferences.putString("message", myString);

  Serial.println("String saved!");


  String retrievedString = preferences.getString("message", "Not found");

  Serial.print("Retrieved string: ");
  Serial.println(retrievedString);

    float Pi = 3.14159;

  preferences.putFloat("pi", Pi);

  float retrievedPi = preferences.getFloat("pi", 0.0);


  Serial.print("Retrieved Pi: ");
  Serial.println(retrievedPi, 5);


  preferences.end();
}

void loop() {
}
