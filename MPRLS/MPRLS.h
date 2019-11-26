#include "Arduino.h"
#include "Wire.h"

#define MPRLS_POWERED       (0b01000000) ///< Status SPI powered bit
#define MPRLS_BUSY          (0b00100000) ///< Status busy bit
#define MPRLS_INTEGRITY_FAIL        (0b00000100) ///< Status bit for integrity fail
#define MPRLS_SATURATION       (0b00000001) ///< Status bit for math saturation

enum unit : uint8_t{
    PSI, HPA, ATM
};

class MPRLS {
  private:
    uint8_t _addr = 0x18;
    TwoWire *_i2c;
    uint32_t _getRawPressure(void);
    float   _getPressureHPA(void);
    float   _getPressurePSI(void);
    float   _getPressureATM(void);
    uint8_t status;
    unit _pressureUnit=PSI;

  public:
    //Constructor
    MPRLS();

    //Methods
    bool    begin(); //twoWire is a pointer that points to the address of Wire.
    void    setPressureUnit(unit pUnit);
    uint8_t getStatus(void);
    float   getPressure();
    float   getPressure(unit pUnit);
};
