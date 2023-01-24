#ifndef BNO_H
#define BNO_H

#include "SparkFun_BNO080_Arduino_Library.h"



class BNO
{
    public:
        void initBNO();
        double getAccel();        
};

#endif