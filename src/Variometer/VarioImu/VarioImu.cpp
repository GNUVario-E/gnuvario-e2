#include "VarioImu.h"
#include "VarioDebug/VarioDebug.h"
#include "Variometer/TwoWireScheduler/TwoWireScheduler.h"
#include "Variometer/LightInvensense/LightInvensense.h"
#include "Variometer/IntTW/IntTW.h"
#include "Variometer/ms5611TW/ms5611TW.h"
#include "Variometer/vertaccel/vertaccel.h"
#include "VarioManager.h"
#include "Variometer/kalmanvert/kalmanvert.h"
#include "VarioManager.h"

#define R2D 57.2958

// Ms5611 TWScheduler::ms5611;
// Vertaccel TWScheduler::vertaccel;
VarioImu::VarioImu(Kalmanvert *_kalmanvert)
{
    kalmanvert = _kalmanvert;
}

void VarioImu::init()
{
    intTW.begin();
    twScheduler.init();

    while (!twScheduler.havePressure())
    {
        // attente pression
    }

    /* init kalman filter with 0.0 accel */
    double firstAlti = twScheduler.getAlti();

    Serial.print("first altitude: ");
    Serial.println(firstAlti);

    Serial.print("PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION: ");
    Serial.println(varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->getValueFloat());

    kalmanvert->init(firstAlti,
                     0.0,
                     POSITION_MEASURE_STANDARD_DEVIATION,
                     varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->getValueFloat(),
                     millis());
}

bool VarioImu::havePressure(void)
{
    return (twScheduler.havePressure());
}

bool VarioImu::updateData(void)
{
    if (twScheduler.havePressure())
    {
        Serial.println("havePressure");
    }
    if (twScheduler.haveAccel())
    {
        Serial.println("haveAccel");
    }
    if (twScheduler.havePressure() && twScheduler.haveAccel())
    {
        double temp;
        double alti;
        double accel;
        twScheduler.resetNewAccel();
        twScheduler.getTempAlti(temp, alti);
        accel = twScheduler.getAccel(NULL);
        Serial.print("altitude: ");
        Serial.println(alti);

        Serial.print("accel: ");
        Serial.println(accel);

        Serial.print("temp: ");
        Serial.println(temp);

        // kalmanvert->update(twScheduler.getAlti(),
        //                    twScheduler.getAccel(NULL),
        //                    millis());

        return true;
    }
    else
    {
        Serial.println("Ni pression et/ou ni alti");
    }
    // else if (twScheduler.haveNewAccel())
    // {
    //     compteurAccel++;
    //     twScheduler.resetNewAccel();
    //     if (compteurAccel > 100)
    //     {
    //         compteurAccel = 0;
    //         twScheduler.resetNewAccel();
    //     }
    //     return false;
    // }
    return false;
}

double VarioImu::firstAlti()
{

    /******************/
    /* get first data */
    /******************/

    VARIO_DATA_DEBUG_PRINTLN("Attente premiere mesure alti");

    while (!twScheduler.havePressure())
    {
    }

    VARIO_DATA_DEBUG_PRINTLN("première mesure");

    double firstAlti = twScheduler.getAlti();

    if (isnan(firstAlti))
    {
        for (int i = 0; i < 4; i++)
        {
            delay(1000);

            firstAlti = twScheduler.getAlti();

            if (!isnan(firstAlti))
                break;
        }
    }

    if (isnan(firstAlti))
    {
        VARIO_DATA_DEBUG_PRINT("Fail firstAlti : ");
        VARIO_DATA_DEBUG_PRINTLN("reinit");
        ESP.restart();
    }

    return firstAlti;
}

double VarioImu::getAlti()
{
    alti = twScheduler.getAlti();
    return alti;
}

double VarioImu::getTemp()
{
    return temp;
}

double VarioImu::getAccel()
{
    accel = twScheduler.getAccel(NULL);
    return accel;
}

void VarioImu::initKalman(double firstAlti)
{

    kalmanvert->init(firstAlti,
                     0.0,
                     POSITION_MEASURE_STANDARD_DEVIATION,
                     varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->getValueFloat(),
                     millis());

    VARIO_DATA_DEBUG_PRINTLN("kalman init");
}

void VarioImu::updateKalman(double mp, double ma, unsigned long timestamp)
{
    kalmanvert->update(mp, ma, timestamp);
}