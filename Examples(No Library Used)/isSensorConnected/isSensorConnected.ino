#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  Wire.beginTransmission(0x18);
  delay(10);
  byte error = Wire.endTransmission();
  if(error == 0) Serial.println("Sensor Found");
  else Serial.println("Sensor not found");
  delay(1000);
}
