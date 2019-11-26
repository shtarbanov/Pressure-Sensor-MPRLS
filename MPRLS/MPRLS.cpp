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
  status = getStatus();
  if(status == 0b01000000) return true;
  else return false;
}

void MPRLS::setPressureUnit(unit pUnit){
  _pressureUnit=pUnit;
}

uint8_t MPRLS::getStatus(void){
  _i2c->requestFrom(_addr, (uint8_t)1);
  return _i2c->read();
}

uint32_t MPRLS::_getRawPressure(void){
  //Request a new pressure reading to be performed.
  _i2c->beginTransmission(_addr);
  _i2c->write(0xAA);   // command to read pressure
  _i2c->write((byte)0x0);
  _i2c->write((byte)0x0);
  _i2c->endTransmission();
    
  //At this point, the sensor has been asked to do a pressure reading. We can now send as many read requests as we
  //want and we will keep getting the same  pressure value. The sensor will keep spitting the same value until a 
  //a new pressure reading request is made. Now, we don't want to send a request to read all 4 bytes, because the 
  //sensor may still be buzy, so we want to just read the status byte first. And then after we have confirmed that 
  //it is not buzy, then we will send a request to read 4 bytes. Thus, the minimum number of bytes we will actually
  //read will be 5. This takes advantage of the fact that the sensor stops sending data when it sees the stop signal,
  //and then it starts sending data from byte 1 again, when a new read request is made. 
  //(An alternative approach would be to simply request 4 bytes from the start, and if the sensor is busy send another
  //request for 4 bytes. In the case when sensor is not buzy, this saves us from reading status byte twice, but in the
  //case when the sensor is buzy, we are reading the last 3 bytes when we know they are invalid. Thus this approach
  //would be more wasteful.)

  //Request to read the status byte only. Keep reading it until it is not busy.:
  while(status=getStatus() & MPRLS_BUSY){
    delay(2); //computation will will not take more than 5ms.
  }

  //Request to read all 4 bytes, including status byte of course.
  _i2c->requestFrom(_addr, (uint8_t)4);
  
  status = _i2c->read();
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

float MPRLS::getPressure(){
  if(_pressureUnit==PSI)      return _getPressurePSI();
  else if(_pressureUnit==HPA) return _getPressureHPA();
  else if(_pressureUnit==ATM) return _getPressureATM();
}

float MPRLS::getPressure(unit pUnit){
  if(pUnit==PSI)      return _getPressurePSI();
  else if(pUnit==HPA) return _getPressureHPA();
  else if(pUnit==ATM) return _getPressureATM();
}

float MPRLS::_getPressureHPA(void){
  return _getPressurePSI() * 68.947572932;
}

float MPRLS::_getPressurePSI(void){
  uint32_t raw_psi = _getRawPressure();
  if (raw_psi == 0xFFFFFFFF) return NAN;

  //See Derivation in the google Docs file: FlowIO/PressureMPRLS + API
  float psi = raw_psi*(25/13421772.8)-3.125;
  return psi;
}

float MPRLS::_getPressureATM(void){
  return _getPressurePSI() * 0.0680459639;
}
