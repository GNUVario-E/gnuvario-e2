
#pragma once

#include "kalmanvert/kalmanvert.h"
#include "VarioImu/VarioImu.h"
#include "VarioGPS/VarioGPS.h"
#include "VarioBeeper/VarioBeeper.h"
#include "VarioSD/VarioSD.h"
#include "VarioBle/VarioBle.h"
#include "VarioHisto/VarioHisto.h"
#include "VarioParameter/Parameters.h"
#include "Variometer/ImuBno08x/bno.h"

#define SDA_PIN 27
#define SCL_PIN 32
#define POWER_PIN 12
#define POWER_PIN_STATE HIGH 

class Variometer
{
private:
    Parameters *params = Parameters::getInstance();
    const TickType_t delayT50 = pdMS_TO_TICKS(50);
    const TickType_t delayT10 = pdMS_TO_TICKS(10);
    TaskHandle_t _taskVarioHandle = NULL;
    static void startTaskImpl(void *);
    double preTaskInitFirstAlti();
    void task();

    BNO *bno;
    Kalmanvert *kalmanvert;
    VarioImu *varioImu;
    VarioGPS *varioGPS;
    VarioBeeper *varioBeeper;
    VarioSD *varioSD;
    VarioBle *varioBle;
    VarioHisto<50, 40> *varioHisto;
    SpeedHisto<500, 120, 2> *speedHisto;

public:
    Variometer(VarioBeeper *varioBeeper, VarioSD *varioSD);
    void init();
    void startTask();
    void debug();
    void initFromAgl();
    void disableAcquisition();
};
