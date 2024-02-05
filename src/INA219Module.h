// INA219Module.h
#ifndef INA219Module_h
#define INA219Module_h
#define INA219_I2C_ADDRESS 0x40

#include <Adafruit_INA219.h>

class INA219Module {
public:
    INA219Module();
    bool begin();
    float getCurrent();
    float getBusVoltage();
    float getPower();
    float calculateAveragePower();
    float getTotalEnergy();
private:
    Adafruit_INA219 _ina219;
    unsigned long _lastEnergyCalculationTime = 0; // Initialisierung der Zeitvariable
    float _totalEnergy = 0.0; // Initialisierung der Gesamtenergie
};

#endif
