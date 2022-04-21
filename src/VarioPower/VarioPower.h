#pragma once

#include <Arduino.h>

#define REF_VOLTAGE 2280
class VarioPower
{
private:
    long getVoltage();

public:
    VarioPower();
    void init();
    float getTension();
    uint8_t getCapacitePct();
};