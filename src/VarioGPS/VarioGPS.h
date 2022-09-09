#pragma once

#include <Arduino.h>
#include "HardwareConfig/HardwareConfig.h"
#include <TinyGPS++.h>
#include "FC.h"

#define GPS_BAUD 9600

class VarioGPS
{
private:
    TinyGPSPlus gps;
    const TickType_t delayT500 = pdMS_TO_TICKS(500);
    TaskHandle_t _taskVarioGPSHandle = NULL;
    static void startTaskImpl(void *);
    void task();
    uint32_t startTaskTime;

public:
    VarioGPS();
    void init();
    void startTask();
    void displayInfo();
};

extern FC fc;