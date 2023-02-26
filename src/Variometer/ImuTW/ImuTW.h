#ifndef BNO_H
#define BNO_H

#include "SparkFun_BNO080_Arduino_Library.h"

#define SDA_PIN 27
#define SCL_PIN 32
#define POWER_PIN 12
#define POWER_PIN_STATE HIGH 

class IMUTW
{
    public:
        void initBNO();
        double getAccel();    
        void getpressure();
        double alti;
        double accel;
        double getAlti();
    private:
        double computeAlti(double pressure, double seaLevelPressure = 101325);
};

#endif