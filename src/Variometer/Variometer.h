
#ifndef VARIOMETER_H
#define VARIOMETER_H

#include "kalmanvert/kalmanvert.h"
#include "VarioImu/VarioImu.h"
#include "VarioGps/VarioGps.h"
#include "VarioBeeper/VarioBeeper.h"

class Variometer
{
private:
    const TickType_t delayT50 = pdMS_TO_TICKS(50);
    TaskHandle_t _taskVarioHandle = NULL;
    static void startTaskImpl(void *);

    void task();

    Kalmanvert *kalmanvert;
    VarioImu *varioImu;
    VarioGps *varioGps;
    VarioBeeper *varioBeeper;
    //  //démarrage du vario
    //     varioImu = new VarioImu(this, kalmanvert);
public:
    Variometer(VarioBeeper *varioBeeper);
    void init();
    void startTask();
};
#endif // VARIOMETER_H