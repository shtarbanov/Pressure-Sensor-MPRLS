#ifndef MS5837_h
#define MS5837_h

#include "Arduino.h"
#include <Wire.h> //There are multiple ports in this: Wire, Wire

enum Unit : uint8_t{
	PSI, ATM, MBAR, KPA, PA
};

class MS5837 {
public:
	float pressure_mbar=-1; //Pressure returned in mbar.
	float pressure_psi=-1;
	float pressure_pa=-1; //pascal
	float pressure_kpa=-1; //kiloPascal
	float pressure_atm=-1;
	float temp_c=-1; // Temperature returned in deg C

	MS5837(); //constructor
	bool initSensor();
	void requestPressureAndTemp();	//read from I2C takes up to 40 ms, so use sparingly if possible.
	void setPressureUnit(Unit pUnit); //default is PSI.
	float getPressure();
	float getPressure(Unit pUnit);
	uint8_t readError();
private:
	TwoWire * _i2c;//This stores the requested i2c port
	bool _i2cInitialized=false;
	Unit _pressureUnit=PSI;
	uint16_t C[8];
	uint32_t D1_pres, D2_temp;
	int32_t TEMP;
	int32_t P;

	void calculate(); //Performs calculations per the sensor data sheet for conversion and second order compensation.
	uint8_t crc4(uint16_t n_prom[]);

	uint8_t _errorCode = 0; //this will change value if an error occurs.
};
#endif
