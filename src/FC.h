#pragma once

#include <Arduino.h>

struct power_data
{
    int capacite;
    float tension;
};

struct fcdata_t
{
    int16_t alt;
    float vario;
    bool isFixed;
    uint8_t volume;
    power_data power;
};
