// INA219Module.cpp
#include "INA219Module.h"

INA219Module::INA219Module() : _lastEnergyCalculationTime(0), _totalEnergy(0.0), _ina219(INA219_I2C_ADDRESS) {
}

bool INA219Module::begin() {
  return _ina219.begin();
}

float INA219Module::getCurrent() {
  return _ina219.getCurrent_mA() / 1000.0;
}

float INA219Module::getBusVoltage() {
  return _ina219.getBusVoltage_V();
}

float INA219Module::getPower() {
  return _ina219.getPower_mW() / 1000.0;
}

float INA219Module::calculateAveragePower() {
  return _ina219.getPower_mW() / 1000.0;
}

float INA219Module::getTotalEnergy() {
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - _lastEnergyCalculationTime) / 3600000.0;

    float currentPower = getPower();

    _totalEnergy += currentPower * elapsedTime;

    _lastEnergyCalculationTime  = currentTime;

    return _totalEnergy;
}

