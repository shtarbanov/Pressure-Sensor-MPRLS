#include <Wire.h>
#include "MPRLS.h"

MPRLS mpr = MPRLS();

void setup() {
  Serial.begin(115200);
  if (mpr.begin()==false){
    Serial.println("Sensor not found.");
    while(1) delay(10);
  }
  mpr.setPressureUnit(HPA);
}

void loop() {
  //TODO: Can you read the pressure at the 400MHz bus speed?
  float pressure = mpr.getPressure(); //this returns in the unit we have specified in the setPressureUnit() function.
  Serial.print("\nPressure: "); Serial.println(pressure); 
  Serial.println();

  //We can also force a specific unit in the getPressure() function.
  float pPSI = mpr.getPressure(PSI);
  float pATM = mpr.getPressure(ATM);
  float pHPA = mpr.getPressure(HPA);
  Serial.print("Pressure (PSI): "); Serial.println(pPSI);
  Serial.print("Pressure (ATM): "); Serial.println(pATM);
  Serial.print("Pressure (HPA): "); Serial.println(pHPA);

  delay(1000);
}
