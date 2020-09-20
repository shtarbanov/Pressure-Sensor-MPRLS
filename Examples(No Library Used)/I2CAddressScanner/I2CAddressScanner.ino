#include <Wire.h>
#define REDLED 17
#define BLUELED 19
void setup(){
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\n I2C Device Detection starting...");
  pinMode(REDLED, OUTPUT);
  digitalWrite(REDLED,HIGH);
}
  
void loop(){
  byte error, address; 
  Serial.println("Scanning...");
  for(address = 1; address < 127; address++){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      Serial.print("I2C device found at address: ");
      Serial.print(address);
      Serial.print(", (0x");
      Serial.print(address,HEX);
      Serial.println(")");
    }   
  }
  Serial.println("\nDone");
  delay(5000); // wait 5 seconds for next scan.
}
