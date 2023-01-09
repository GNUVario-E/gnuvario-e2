#pragma once

#include "SimpleCircleFit.h"
#include "FC.h"

class VarioWind
{
private:
    SimpleCircleFit *circleFit;
    void timerWind();
    static void startTimers10sImpl(TimerHandle_t timerHndlWind);
    void calculateWind(IGCPoint *points, int numPoints, float &windSpeed, int &heading, float &meanSpeed);

public:
    VarioWind();
    ~VarioWind();
    void init();
    void addPoint(double lat, double lon, uint32_t timestamp);
};

extern FC fc;
