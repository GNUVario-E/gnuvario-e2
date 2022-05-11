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

    if (isnan(firstAlti))
    {
        VARIO_DATA_DEBUG_PRINT("Fail firstAlti : ");
        VARIO_DATA_DEBUG_PRINTLN("reinit");
        ESP.restart();
    }

    VARIO_IMU_DEBUG_PRINT("first altitude: ");
    VARIO_IMU_DEBUG_PRINTLN(firstAlti);

    VARIO_IMU_DEBUG_PRINT("PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION: ");
    VARIO_IMU_DEBUG_PRINTLN(varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->getValueFloat());

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

        VARIO_IMU_DEBUG_PRINT("altitude: ");
        VARIO_IMU_DEBUG_PRINTLN(alti);

        VARIO_IMU_DEBUG_PRINT("accel: ");
        VARIO_IMU_DEBUG_PRINTLN(accel);

        VARIO_IMU_DEBUG_PRINT("temp: ");
        VARIO_IMU_DEBUG_PRINTLN(temp);

        return true;
    }
    else if (twScheduler.haveNewAccel())
    {
        VARIO_IMU_DEBUG_PRINTLN("haveNewAccel");
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
        VARIO_IMU_DEBUG_PRINTLN("Ni pression et/ou ni alti");
    }
    return false;
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

        VARIO_DATA_DEBUG_PRINT("bearing: ");
        VARIO_DATA_DEBUG_PRINTLN(bearing);
    }
    else
    {
        bearing = -1;
    }

    return bearing;
}