
#pragma once

#include "kalmanvert/kalmanvert.h"
#include "VarioImu/VarioImu.h"
#include "VarioGPS/VarioGPS.h"
#include "VarioBeeper/VarioBeeper.h"
#include "VarioSD/VarioSD.h"
#include "VarioBle/VarioBle.h"

class Variometer
{
private:
    const TickType_t delayT50 = pdMS_TO_TICKS(50);
    TaskHandle_t _taskVarioHandle = NULL;
    static void startTaskImpl(void *);
    void preTaskInit();
    void task();

    Kalmanvert *kalmanvert;
    VarioImu *varioImu;
    VarioGPS *varioGPS;
    VarioBeeper *varioBeeper;
    VarioSD *varioSD;
    VarioBle *varioBle;

public:
    Variometer(VarioBeeper *varioBeeper, VarioSD *varioSD);
    void init();
    void startTask();
    void debug();
    void initFromAgl();
    void disableAcquisition();
};
