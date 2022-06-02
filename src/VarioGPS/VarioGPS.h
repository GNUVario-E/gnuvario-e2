#pragma once

#include <Arduino.h>
#include "HardwareConfig/HardwareConfig.h"
#include <TinyGPS++.h>

#define GPS_BAUD 9600

class VarioGPS
{
private:
    TinyGPSPlus gps;
    const TickType_t delayT100 = pdMS_TO_TICKS(100);
    TaskHandle_t _taskVarioGPSHandle = NULL;
    static void startTaskImpl(void *);
    void task();
    uint32_t startTaskTime;

public:
    void init();
    void startTask();
    void displayInfo();
};
