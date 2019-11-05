#include <Wire.h>

byte address=24, statusByte=0;

void setup(){
  Wire.begin();
  Serial.begin(115200);
}
 
void loop(){

  //Step 1: Send the activation command 0xAA0000 as a write request.
  Wire.beginTransmission(address);
  Wire.write(0xAA);
  Wire.write((byte)0x00);
  Wire.write((byte)0x00);
  Wire.endTransmission();

  //Step 2: Allow 5ms delay for the request to be processed.
  delay(10); //Absolute minimum is: 5ms Arduino, 6ms for nRF52832. 
             //Tried it with 4ms, and didn't work. Use 10 for safety.

  //Step3: Read all 4 bytes now
  Wire.requestFrom(address, (uint8_t)4);  
  statusByte = Wire.read();
  uint32_t rawData = Wire.read();
  rawData = rawData << 8;
  rawData = rawData | Wire.read();
  rawData = rawData << 8;
  rawData = rawData | Wire.read();

  Serial.print("Status Byte HEX: ");
  Serial.print(statusByte, HEX);
  if(statusByte != 0b01000000) //0x40
    Serial.print(", ERROR IN STATUS BYTE. ");
  else 
    Serial.print(", OK. ");

  Serial.print("Raw pressure data (24 bits): ");
  Serial.println(rawData, BIN);
  
  delay(1000);           // wait 1 second for next scan
}
