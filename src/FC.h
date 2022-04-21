#pragma once

#include <Arduino.h>

struct power_data
{
    int capacite;
    float tension;
};

struct sound_data
{
    bool isMute;
    float volume;
};

struct fcdata_t
{
    int16_t alt;
    float vario;
    bool isFixed;
    power_data power;
    sound_data sound;
};
