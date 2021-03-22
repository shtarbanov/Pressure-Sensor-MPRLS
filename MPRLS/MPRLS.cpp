#include "Arduino.h"
#include "Wire.h"
#include "MPRLS.h"

#define MPRLS_ADDR						0x18
#define MPRLS_POWERED        	0b01000000 ///< Status SPI powered bit
#define MPRLS_BUSY           	0b00100000 ///< Status busy bit
#define MPRLS_INTEGRITY_FAIL 	0b00000100 ///< Status bit for integrity fail
#define MPRLS_SATURATION     	0b00000001 ///< Status bit for math saturation

//constructor
MPRLS::MPRLS(){
}
bool MPRLS::initSensor(){
  _i2c = &Wire; //_i2c is a pointer that points to Wire.
  _i2c->begin();
  delay(10); // startup timing
	_i2cInitialized=true;//Once this is set to true, there is nothing to set it back to false.
    //this is the only problem with this, apparent only if someone calls the i2c .end() funtion
    //and then tries to call the .getPressure() function without calling activateSensor() beforehand.
    //In that case, the system will hang, but a user would typically never need to ever call .end().
    //But once we start having more I2C devices used, users are more likely to invoke .end(), so this
    //should be fixed.

    //The following if statement should ideally go before the flag. However, the function
    //_getStatusByte() relies on the value of the flag being true. Thus we must have it after the flag.
    //There are better ways to do this
  if(_getStatusByte() == 0b01000000) return true;
  else{
    _errorCode = 222;
    _i2cInitialized=false;
    return false;
  }
}

void MPRLS::setPressureUnit(Unit pUnit){
  _pressureUnit=pUnit;
}

uint8_t MPRLS::_getStatusByte(){
  //Check if the the sensor has been initialized successfully
  if(_i2cInitialized==false){
    _errorCode = 225; //Initialization flag has not been set.
    return 0;
  }
  _i2c->requestFrom(MPRLS_ADDR, (uint8_t)1);
  return _i2c->read();
}

void MPRLS::requestPressure(){
  uint8_t statByte;
  uint32_t rawData = 0xFFFFFFFF; //only the lower 24 bits will store our data. Upper 8 bits will be 0.
  pressure_psi = 888.8;
  pressure_atm = 888.8;
  pressure_pa = 888.8;
  pressure_kpa = 888.8;
  pressure_mbar = 888.8;

  if(_i2cInitialized==false){	//Check if the user has forgotten to initialize the sensor
    _errorCode = 223; //Initialization flag has not been set.
    return;
  }

  //Request a new pressure reading to be performed.
  _i2c->beginTransmission(MPRLS_ADDR);
  _i2c->write(0xAA);   // command to read pressure
  _i2c->write(0x00);
  _i2c->write(0x00);
  _i2c->endTransmission();

  //At this point, the sensor has been asked to do a pressure reading. We can now send as many read requests as we
  //want and we will keep getting the same  pressure value. The sensor will keep spitting the same value until a
  //a new pressure reading request is made. Now, we don't want to send a request to read all 4 bytes, because the
  //sensor may still be busy, so we want to just read the status byte first. And then after we have confirmed that
  //it is not busy, then we will send a request to read 4 bytes. Thus, the minimum number of bytes we will actually
  //read will be 5. This takes advantage of the fact that the sensor stops sending data when it sees the stop signal,
  //and then it starts sending data from byte 1 again, when a new read request is made.
  //(An alternative approach would be to simply request 4 bytes from the start, and if the sensor is busy send another
  //request for 4 bytes. In the case when sensor is not busy, this saves us from reading status byte twice, but in the
  //case when the sensor is busy, we are reading the last 3 bytes when we know they are invalid. Thus this approach
  //would be more wasteful.)

  //Request to read the status byte only. Keep reading it until it is not busy.:
  while(_getStatusByte() & MPRLS_BUSY){
    delay(2); //computation will will not take more than 5ms.
  }

  //Request to read all 4 bytes, including status byte of course.
  _i2c->requestFrom(MPRLS_ADDR, (uint8_t)4);

  statByte = _i2c->read();
  if((statByte & MPRLS_SATURATION)==false && (statByte & MPRLS_INTEGRITY_FAIL)==false){
    rawData = _i2c->read();
    rawData <<= 8;
    rawData |= _i2c->read();
    rawData <<= 8;
    rawData |= _i2c->read();
  }
  else{
    return;
  }

  pressure_psi = rawData*(25/13421772.8)-3.125;
  pressure_atm = pressure_psi * 0.0680459639;
  pressure_mbar = pressure_psi * 68.9476;
  pressure_pa = pressure_psi * 6894.7600026396;
  pressure_kpa = pressure_psi * 6.89476;
}

float MPRLS::getPressure(Unit pUnit){
  requestPressure();
  if(pUnit==PSI) return pressure_psi;
  else if(pUnit==ATM) return pressure_atm;
  else if(pUnit==KPA) return pressure_kpa;
  else if(pUnit==PA) return pressure_kpa;
  else if(pUnit==MBAR) return pressure_mbar;
}

float MPRLS::getPressure(){
  return getPressure(_pressureUnit);
}
uint8_t MPRLS::readError(){
	return _errorCode;
}
