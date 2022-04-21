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

float VarioPower::getTension()
{
    long voltage = getVoltage();

    float uVoltage = (4.2 * float(voltage)) / REF_VOLTAGE; // 2280;

    return uVoltage;
}

uint8_t VarioPower::getCapacitePct()
{
    double tension = getTension();

    uint8_t pourcentage = min(round(123 - 123 / pow(1 + pow((tension / 3.7), 80), 0.165)), (double)100);

    return pourcentage;
}