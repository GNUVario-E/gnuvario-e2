#pragma once

#include <Arduino.h>
#include "HardwareConfig/HardwareConfig.h"
#include <TinyGPS++.h>
#include <CircularBuffer.h>
#include "FC.h"
#include "VarioParameter/Parameters.h"

#define GPS_BAUD 9600
#define BUFFER_SIZE 100

class VarioGPS
{
private:
    Parameters *params = Parameters::getInstance();
    
    TinyGPSPlus gps;
    const TickType_t delayT500 = pdMS_TO_TICKS(500);
    const TickType_t delayT100 = pdMS_TO_TICKS(100);
    const TickType_t delayT10 = pdMS_TO_TICKS(10);
    const TickType_t delayT1 = pdMS_TO_TICKS(1);
    TaskHandle_t _taskVarioGPSHandle = NULL;
    static void startTaskImpl(void *);
    void task();
    uint32_t lastCharProcessedTime;
    CircularBuffer<char, BUFFER_SIZE> sentenceBuffer;
    void sendSentenceToFC();
    UBaseType_t minRemainingStackSize = 10000;

public:
    VarioGPS();
    void init();
    void startTask();
    void displayInfo();
};

extern FC fc;