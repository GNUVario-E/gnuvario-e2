

#include "Variometer.h"
#include "VarioTool/VarioTool.h"

#define COEF_ALTI_FILTERED 0.1
#define VARIO_TASK_PRIORITY 10

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
            fc.vario.velocity = velocity;

            calibratedAlti = kalmanvert->getCalibratedPosition();

            if (calibratedAlti < 0)
                calibratedAlti = 0;

            fc.vario.alti = round(calibratedAlti);

            // Serial.print("velocity:");
            // Serial.println(velocity);
        }
        // give time to other tasks
        vTaskDelay(delayT50);

        bearing = varioImu->getBearing();
        if (bearing != -1)
        {
            fc.vario.bearing = bearing;
            VarioTool::bearingToOrdinal2c(fc.vario.bearingTxt, bearing);
            VARIO_DATA_DEBUG_PRINT("Bearingtxt :");
            VARIO_DATA_DEBUG_PRINTLN(fc.vario.bearingTxt);
        }
        // give time to other tasks
        vTaskDelay(delayT50);
    }
}

Variometer::Variometer(VarioBeeper *_varioBeeper, VarioSD *_varioSD)
{
    varioBeeper = _varioBeeper;
    kalmanvert = new Kalmanvert();
    varioImu = new VarioImu(kalmanvert);
    varioGPS = new VarioGPS();
    varioSD = _varioSD;
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
    if (fc.gps.locTimestamp > (millis() - 1200) && fc.agl.aglTimestamp > (millis() - 1200))
    {
        int groundLevel = fc.agl.groundLvl;
        if (groundLevel != -1)
        {
            kalmanvert->calibratePosition(fc.agl.groundLvl);

            VARIO_AGL_DEBUG_PRINT("groundLvl:");
            VARIO_AGL_DEBUG_PRINTLN(groundLevel);

            varioBeeper->generateTone(523, 250);
            varioBeeper->generateTone(659, 250);
            varioBeeper->generateTone(784, 250);
            varioBeeper->generateTone(1046, 250);
        }
    }
}