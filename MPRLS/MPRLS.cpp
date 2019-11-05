#include "Arduino.h"
#include "Wire.h"
#include "MPRLS.h"

//constructor
MPRLS::MPRLS(){
}

bool MPRLS::begin(){
  _i2c = &Wire; //_i2c is a pointer that points to &Wire.
  _i2c->begin();
  delay(10); // startup timing
  uint8_t stat = getStatus();
  if(stat == 0b01000000) return true;
  else return false;
}

uint8_t MPRLS::getStatus(void){
  _i2c->requestFrom(_addr, (uint8_t)1);
  return _i2c->read();
}

uint32_t MPRLS::getRawPressure(void){
  _i2c->beginTransmission(_addr);
  _i2c->write(0xAA);   // command to read pressure
  _i2c->write((byte)0x0);
  _i2c->write((byte)0x0);
  _i2c->endTransmission();
    
  _i2c->requestFrom(_addr, (uint8_t)4);
  
  uint8_t status = _i2c->read();
  if (status & MPRLS_SATURATION) return 0xFFFFFFFF;
  if (status & MPRLS_INTEGRITY_FAIL)  return 0xFFFFFFFF;

  uint32_t rawData; //only the lower 24 bits will store our data. Upper 8 bits will be 0.
  rawData = _i2c->read();
  rawData <<= 8;
  rawData |= _i2c->read(); 
  rawData <<= 8;
  rawData |= _i2c->read();
  return rawData;
}

float MPRLS::getPressureHPA(void){
  return getPressurePSI() * 68.947572932;
}

float MPRLS::getPressurePSI(void){
  uint32_t raw_psi = getRawPressure();
  if (raw_psi == 0xFFFFFFFF) return NAN;

  //See Derivation in the google Docs file: FlowIO/PressureMPRLS + API
  float psi = raw_psi*(25/13421772.8)-3.125;
  return psi;
}

float MPRLS::getPressureATM(void){
  return getPressurePSI() * 0.0680459639;
}
