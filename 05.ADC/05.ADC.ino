//used ESP32-U (ESP32-WROOM-32U)
#define ADC_PIN 34
#define LED_PIN 25

void setup(){
  Serial.begin(115200);

  pinMode(LED_PIN,OUTPUT);

  analogReadResolution(12);
  analogWriteResolution(LED_PIN ,8);
}

void loop(){

  int adcValue = analogRead(ADC_PIN);
  int pwmValue = map(adcValue,0,4095, 0,255);
  analogWrite(LED_PIN, pwmValue);
  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" PWM: ");
  Serial.println(pwmValue);

  delay(50);
}
