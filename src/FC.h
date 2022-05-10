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

struct vario_data
{
    int16_t alti;
    float velocity;
    int bearing;
};

struct fcdata_t
{
    double_t speed;
    vario_data vario;
    bool isFixed;
    power_data power;
    sound_data sound;
};
