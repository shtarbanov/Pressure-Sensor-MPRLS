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
  //TODO: Can you read the pressure at the 400MHz bus speed?  
  float pPSI = mpr.getPressurePSI();
  //delay(5); This is no longer necessary, b/c I fixed the library to check if status byte is busy!
  float pATM = mpr.getPressureATM();
  float pHPA = mpr.getPressureHPA();
  Serial.print("Pressure (PSI): "); Serial.println(pPSI);
  Serial.print("Pressure (ATM): "); Serial.println(pATM);
  Serial.print("Pressure (HPA): "); Serial.println(pHPA);
  delay(1000);
}
