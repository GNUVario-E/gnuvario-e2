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
}

void VarioImu::postInit()
{
    while (!twScheduler.havePressure())
    {
        // attente pression
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    /* init kalman filter with 0.0 accel */
    double firstAlti = twScheduler.getAlti();

    // if (isnan(firstAlti))
    // {
    //     for (int i = 0; i < 4; i++)
    //     {
    //         vTaskDelay(pdMS_TO_TICKS(500));

    //         firstAlti = twScheduler.getAlti();

    //         if (!isnan(firstAlti))
    //             break;
    //     }
    // }

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
    if (twScheduler.havePressure() && twScheduler.haveAccel())
    {
        compteurAccel = 0;
        twScheduler.resetNewAccel();
        twScheduler.getTempAlti(temp, alti);
        accel = twScheduler.getAccel(NULL);

        Serial.print("altitude: ");
        Serial.println(alti);

        Serial.print("accel: ");
        Serial.println(accel);

        Serial.print("temp: ");
        Serial.println(temp);

        return true;
    }
    else if (twScheduler.haveNewAccel())
    {
        Serial.println("haveNewAccel");
        compteurAccel++;
        twScheduler.resetNewAccel();
        if (compteurAccel > 100)
        {
            compteurAccel = 0;
            twScheduler.resetNewAccel();
        }
        return false;
    }
    else
    {
        Serial.println("Ni pression et/ou ni alti");
    }
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
            fc.vario.alti = firstAlti;
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
    return alti;
}

double VarioImu::getTemp()
{
    return temp;
}

double VarioImu::getAccel()
{
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

int16_t VarioImu::getBearing(void)
{

    //     // vitesse > 5km et dernière mesure de vitesse de moins de 1.5s
    //     if ((variometerState > VARIOMETER_STATE_CALIBRATED) && (speedAvailable || ((millis() - timeSpeedMesure) < nbMsLastMesureAcceptable)) && (currentSpeed > 5))
    //     {
    //         if (nmeaParser.haveBearing())
    //         {

    //             bearing = nmeaParser.getBearing();

    //             gpsAvailable = true;
    //             timeCapMesure = millis();

    // #ifdef DATA_DEBUG
    //             SerialPort.print("Compas GPS : ");
    //             SerialPort.println(bearing);
    // #endif // DATA_DEBUG
    //         }
    //     }

    //     // desactive le baro GPS si pas de mesure durant 1,5sec - passe au baro magnetique
    //     if ((gpsAvailable) && ((millis() - timeCapMesure) < nbMsLastMesureAcceptable))
    //     {
    //         return bearing;
    //     }
    //     else
    //     {
    //         gpsAvailable = false;
    //     }

    if (twScheduler.haveAccel() && twScheduler.haveMag())
    {
        double vertVector[3];
        twScheduler.getAccel(vertVector);

        // accelerometer and magnetometer data
        float a, ax, ay, az;

        ax = vertVector[0];
        ay = vertVector[1];
        az = vertVector[2];

        // Normalize accelerometer and magnetometer data
        a = sqrtf(ax * ax + ay * ay + az * az);
        ax /= a;
        ay /= a;
        az /= a;
        twScheduler.resetNewAccel();

        double northVector[2];
        double northVectorNorm[2];
        twScheduler.getNorthVector(vertVector, northVector);

        double norm = sqrt(northVector[0] * northVector[0] + northVector[1] * northVector[1]);
        northVectorNorm[0] = northVector[0] / norm;
        northVectorNorm[1] = northVector[1] / norm;

        int16_t tmpcap = atan2(northVectorNorm[1], northVectorNorm[0]) * 180 / M_PI;
        Serial.print("tmpcap: ");
        Serial.println(tmpcap);
        if (tmpcap < 0)
        {
            tmpcap = tmpcap + 360;
        }
        bearing = tmpcap;

        Serial.print("bearing: ");
        Serial.println(bearing);
    }
    else
    {
        bearing = -1;
    }

    return bearing;
}