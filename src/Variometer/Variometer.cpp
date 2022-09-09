

#include "Variometer.h"
#include "VarioTool/VarioTool.h"

#define COEF_ALTI_FILTERED 0.1
#define VARIO_TASK_PRIORITY 11

void Variometer::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("Task Vario started");
    xTaskCreate(this->startTaskImpl, "TaskVario", 2000, this, VARIO_TASK_PRIORITY, &_taskVarioHandle);
}

void Variometer::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<Variometer *>(parm)->task();
}

void Variometer::task()
{
    double alti;
    double altiFiltered = 0;
    double temperature;
    double accel;
    double velocity;
    int16_t bearing;
    double calibratedAlti;
    bool lastSentence;

    preTaskInit();
    // ici la loop du vario
    while (true)
    {
        if (varioImu->updateData())
        {
            alti = varioImu->getAlti();
            if (altiFiltered != 0)
            {
                altiFiltered = altiFiltered + COEF_ALTI_FILTERED * (alti - altiFiltered);
            }
            else
            {
                altiFiltered = alti; // first reading so set filtered to reading
            }

            accel = varioImu->getAccel();

            unsigned long myTime = millis();
            kalmanvert->update(altiFiltered, accel, myTime);

            velocity = kalmanvert->getVelocity();

            varioBeeper->setVelocity(velocity);
            fc.setVarioVelocity(velocity, millis());

            calibratedAlti = kalmanvert->getCalibratedPosition();

            if (calibratedAlti < 0)
                calibratedAlti = 0;

            fc.setVarioAlti(round(calibratedAlti), millis());

            // Serial.print("velocity:");
            // Serial.println(velocity);
        }
        // give time to other tasks
        vTaskDelay(delayT50);

        bearing = varioImu->getBearing();
        if (bearing != -1)
        {
            char bearingTxt[3];
            VarioTool::bearingToOrdinal2c(bearingTxt, bearing);
            fc.setVarioBearing(bearing, bearingTxt, millis());

            VARIO_DATA_DEBUG_PRINT("Bearingtxt :");
            VARIO_DATA_DEBUG_PRINTLN(bearingTxt);
        }
        // give time to other tasks
        vTaskDelay(delayT50);
    }
}

Variometer::Variometer(VarioBeeper *_varioBeeper, VarioSD *_varioSD)
{
    varioBeeper = _varioBeeper;
    varioSD = _varioSD;
    kalmanvert = new Kalmanvert();
    varioImu = new VarioImu(kalmanvert);
    varioGPS = new VarioGPS();
}

void Variometer::init()
{
    varioImu->init();
    varioBeeper->startTask();
    varioGPS->init();
    varioGPS->startTask();
}

void Variometer::preTaskInit()
{
    varioImu->postInit();
}

void Variometer::initFromAgl()
{
    if (fc.getGpsIsFixed() && fc.getGpsLocTimestamp() > (millis() - 1200) && fc.getAglAltTimestamp() > (millis() - 1200))
    {
        int groundLevel = fc.getAglGroundLvl();
        if (groundLevel != -1)
        {
            kalmanvert->calibratePosition(fc.getAglGroundLvl());

            VARIO_AGL_DEBUG_PRINT("groundLvl:");
            VARIO_AGL_DEBUG_PRINTLN(fc.getAglGroundLvl());

            varioBeeper->generateTone(523, 250);
            varioBeeper->generateTone(659, 250);
            varioBeeper->generateTone(784, 250);
            varioBeeper->generateTone(1046, 250);
        }
    }
}