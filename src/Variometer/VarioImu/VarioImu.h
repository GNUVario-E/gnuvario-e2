#pragma once

class Kalmanvert;

#include "FC.h"
#include "VarioParameter/Parameters.h"

class VarioImu
{
private:
    Parameters *params = Parameters::getInstance();

    Kalmanvert *kalmanvert;
    double alti;
    double temp;
    double accel;
    int16_t bearing = 0;
    int8_t compteurAccel = 0;

public:
    VarioImu(Kalmanvert *kalmanvert);
    void init();
    double postInitFirstAlti();
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
