#include "VarioPower.h"
#include "HardwareConfig/HardwareConfig.h"
#include "Arduino.h"

VarioPower::VarioPower()
{
}

void VarioPower::init()
{
    pinMode(PIN_POWER, OUTPUT);
    digitalWrite(PIN_POWER, PIN_POWER_STATE); // turn on POWER (POWER_PIN_STATE is the voltage level HIGH/LOW)

    pinMode(PIN_VOLTAGE_DIVISOR, INPUT);
    analogReadResolution(12);
}

float VarioPower::getTension()
{
    long voltage = getVoltage();

    float uVoltage = (4.2 * float(voltage)) / REF_VOLTAGE; // 2280;

    return uVoltage;
}

long VarioPower::getVoltage()
{
    long tmpVoltage = 0;

    // moyenne sur 10 mesures
    for (int i = 0; i < 10; i++)
    {
        tmpVoltage += analogRead(PIN_VOLTAGE_DIVISOR);
    }
    tmpVoltage = tmpVoltage / 10;

    return (tmpVoltage);
}

int VarioPower::getCapacite()
{
    double tension = getTension();
    int pourcentage = 0;

    if (tension > TENSION_100)
    {
        pourcentage = 100;
    }
    else if (tension < TENSION_100 && tension > TENSION_90)
    {
        pourcentage = 90 + ((tension - TENSION_90) * (100 - 90) / (TENSION_100 - TENSION_90));
    }
    else if (tension < TENSION_90 && tension > TENSION_80)
    {
        pourcentage = 80 + ((tension - TENSION_80) * (90 - 80) / (TENSION_90 - TENSION_80));
    }
    else if (tension < TENSION_80 && tension > TENSION_70)
    {
        pourcentage = 70 + ((tension - TENSION_70) * (80 - 70) / (TENSION_80 - TENSION_70));
    }
    else if (tension < TENSION_70 && tension > TENSION_60)
    {
        pourcentage = 60 + ((tension - TENSION_60) * (70 - 60) / (TENSION_70 - TENSION_60));
    }
    else if (tension < TENSION_60 && tension > TENSION_50)
    {
        pourcentage = 50 + ((tension - TENSION_50) * (60 - 50) / (TENSION_60 - TENSION_50));
    }
    else if (tension < TENSION_50 && tension > TENSION_40)
    {
        pourcentage = 40 + ((tension - TENSION_40) * (50 - 40) / (TENSION_50 - TENSION_40));
    }
    else if (tension < TENSION_40 && tension > TENSION_30)
    {
        pourcentage = 30 + ((tension - TENSION_30) * (40 - 30) / (TENSION_40 - TENSION_30));
    }
    else if (tension < TENSION_30 && tension > TENSION_20)
    {
        pourcentage = 20 + ((tension - TENSION_20) * (30 - 20) / (TENSION_30 - TENSION_20));
    }
    else if (tension < TENSION_20 && tension > TENSION_10)
    {
        pourcentage = 10 + ((tension - TENSION_10) * (20 - 10) / (TENSION_20 - TENSION_10));
    }
    else if (tension < TENSION_10 && tension > TENSION_5)
    {
        pourcentage = 5 + ((tension - TENSION_5) * (10 - 5) / (TENSION_10 - TENSION_5));
    }
    else if (tension < TENSION_5 && tension > TENSION_0)
    {
        pourcentage = 0;
    }

    return pourcentage;
}