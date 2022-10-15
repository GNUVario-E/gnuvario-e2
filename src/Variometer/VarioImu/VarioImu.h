#pragma once

class Kalmanvert;

#include "FC.h"
class VarioImu
{
private:
    Kalmanvert *kalmanvert;
    double alti;
    double temp;
    double accel;
    int16_t bearing = 0;
    int8_t compteurAccel = 0;

public:
    VarioImu(Kalmanvert *kalmanvert);
    void init();
    void postInit();
    void initKalman(double firstAlti);
    bool havePressure(void);
    bool updateData();
    void updateKalman(double mp, double ma, unsigned long timestamp);
    double getAlti();
    double getTemp();
    double getAccel();
    int16_t getBearing(void);
    void disableAcquisition();
};

extern FC fc;
