#pragma once

#include "VarioIgcFile.h"
#include "VarioSD/VarioSD.h"
#include "FC.h"

class VarioIgc
{

private:
    const TickType_t delayT200 = pdMS_TO_TICKS(200);
    static void startTimerIgcImpl(TimerHandle_t timerHndlIgc);
    TimerHandle_t timerHndlIgc;
    void timerIgc();
    // TaskHandle_t _taskVarioIgcHandle = NULL;
    // static void startTaskImpl(void *);
    // void task();
    VarioIgcFile *igcFile;
    void initHeaders(const char *pilot, const char *glider, uint8_t day, uint8_t month, uint8_t year, int8_t timezone);
    bool isReadyToWrite = false;
    uint16_t lastIgcTrameTimestamp = 0;

public:
    VarioIgc();
    // void startTask();
    void startTimer();
    bool createNewIgcFile(const char *pilot, const char *glider, uint8_t day, uint8_t month, uint8_t year, int8_t timezone);
    void addBLine(uint8_t hour, uint8_t minute, uint8_t second, double locLat, double locLon, double altiVario, double altiGps);
};

extern FC fc;