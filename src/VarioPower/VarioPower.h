#pragma once

#define TENSION_100 4.2
#define TENSION_90 4.1
#define TENSION_80 3.97
#define TENSION_70 3.92
#define TENSION_60 3.87
#define TENSION_50 3.83
#define TENSION_40 3.79
#define TENSION_30 3.75
#define TENSION_20 3.70
#define TENSION_10 3.60
#define TENSION_5 3.30
#define TENSION_0 3.0

#define DEFAULT_REF_VOLTAGE 2280
#define REF_VOLTAGE 2280

class VarioPower
{
private:
    long getVoltage();

public:
    VarioPower();
    void init();
    int getCapacite();
    float getTension();
};