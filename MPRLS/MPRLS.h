#include "Arduino.h"
#include "Wire.h"

#define MPRLS_POWERED       (0b01000000) ///< Status SPI powered bit
#define MPRLS_BUSY          (0b00100000) ///< Status busy bit
#define MPRLS_INTEGRITY_FAIL        (0b00000100) ///< Status bit for integrity fail
#define MPRLS_SATURATION       (0b00000001) ///< Status bit for math saturation

class MPRLS {
  private:
    uint8_t _addr = 0x18;
    TwoWire *_i2c;
    uint32_t getRawPressure(void);

  public:
    //Constructor
    MPRLS();

    //Methods
    bool begin(); //twoWire is a pointer that points to the address of Wire.
    uint8_t getStatus(void);
    float   getPressureHPA(void);
    float   getPressurePSI(void);
    float   getPressureATM(void);



};
