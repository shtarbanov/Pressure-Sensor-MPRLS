#include "Arduino.h"
#include "Wire.h"

enum Unit : uint8_t{
	PSI, ATM, MBAR, KPA, PA
};

class MPRLS {
  private:
    uint8_t _addr = 0x18;
    TwoWire *_i2c;
    bool _i2cInitialized=false;
    Unit _pressureUnit=PSI;
    uint8_t _getStatusByte();
    uint8_t _errorCode = 0; //this will change value if an error occurs.

  public:
    //Constructor
    MPRLS();

    //Methods
    bool    activateSensor(); //twoWire is a pointer that points to the address of Wire.
    void requestPressure();
  	void setPressureUnit(Unit pUnit); //default is PSI.
  	float getPressure();
  	float getPressure(Unit pUnit);

    //Literals
  	float pressure_mbar=-1; //Pressure returned in mbar.
  	float pressure_psi=-1;
  	float pressure_atm=-1;
  	float pressure_pa=-1; //pascal
  	float pressure_kpa=-1; //kiloPascal
};
