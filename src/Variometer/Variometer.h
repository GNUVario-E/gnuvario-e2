
#ifndef VARIOMETER_H
#define VARIOMETER_H

#include "kalmanvert/kalmanvert.h"
#include "VarioImu/VarioImu.h"
#include "VarioGps/VarioGps.h"
#include "VarioBeeper/VarioBeeper.h"

class Variometer
{
private:
    Kalmanvert *kalmanvert;
    VarioImu *varioImu;
    VarioGps *varioGps;
    VarioBeeper *varioBeeper;
    //  //démarrage du vario
    //     varioImu = new VarioImu(this, kalmanvert);
public:
    Variometer(VarioBeeper *varioBeeper);
    void init();
};
#endif //VARIOMETER_H