#pragma once
#include "libs.h"

class PowerSupply
{
private:
    string powerSupply;
    string batteryType;
    int batteryLifePercent;
    string powerSavingMode;
    unsigned long batteryLifeWithoutCharging;
    unsigned long fullBatteryLifeWithoutCharging;

public:
    void update();
    string getPowerSupply(_SYSTEM_POWER_STATUS status);
    string getBatteryType();
    friend ostream &operator<<(ostream &os, const PowerSupply &powerSupply);
};