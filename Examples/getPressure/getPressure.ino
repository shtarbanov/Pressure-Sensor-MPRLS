#include <Wire.h>
#include "MPRLS.h"

MPRLS mpr = MPRLS();

void setup() {
  Serial.begin(115200);
  if (mpr.begin()==false){
    Serial.println("Sensor not found.");
    while(1) delay(10);
  }
}

void loop() {
  float pressure = mpr.getPressurePSI();
  Serial.print("Pressure (ATM): "); Serial.println(pressure);
  delay(1000);
}
