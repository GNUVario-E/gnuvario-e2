

#include "Variometer.h"
#include "VarioTool/VarioTool.h"
#include "VarioBle/VarioBle.h"
#include "tasksBitsMask.h"

#define COEF_ALTI_FILTERED 0.1
#define VARIO_TASK_PRIORITY 11

void Variometer::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("Task Vario started");
    // xTaskCreate(this->startTaskImpl, "TaskVario", 4096, this, VARIO_TASK_PRIORITY, &_taskVarioHandle);
    xTaskCreatePinnedToCore(this->startTaskImpl, "TaskVario", 4096, this, VARIO_TASK_PRIORITY, &_taskVarioHandle, 1);
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
    double firstAlti;
    double temp;

    firstAlti = preTaskInitFirstAlti();
    varioHisto->init(firstAlti, millis());
    speedHisto->init(firstAlti, millis());
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

            // varioHisto->setAlti(calibratedAlti, millis());
            // // speedHisto->setAlti(calibratedAlti, millis());

            // if (varioHisto->haveNewClimbRate())
            // {
            //     Serial.println("haveNewClimbRate");
            //     Serial.println(varioHisto->getClimbRate(30));
            // }

            // if (speedHisto->haveNewClimbRate())
            // {
            //     Serial.println("haveNewClimbRate");
            //     Serial.println(speedHisto->getGlideRatio(100));
            // }

            fc.setVarioAlti(round(calibratedAlti), millis());

            if (params->P_BT_ENABLE->getValue())
            {
                if (VarioBle::_taskVarioBleHandle != NULL)
                {
                    xTaskNotify(VarioBle::_taskVarioBleHandle, BLE_LXWP0_SENTENCE_BIT, eSetBits);
                }
            }
            // Serial.print("velocity:");
            // Serial.println(velocity);
        }
        // // give time to other tasks
        // vTaskDelay(delayT50);

        bearing = varioImu->getBearing();
        if (bearing != -1)
        {
            char bearingTxt[3];
            VarioTool::bearingToOrdinal2c(bearingTxt, bearing);
            fc.setVarioBearing(bearing, bearingTxt, millis());

            VARIO_DATA_DEBUG_PRINT("Bearingtxt: ");
            VARIO_DATA_DEBUG_PRINTLN(bearingTxt);
        }

        temp = varioImu->getTemp();
        fc.setVarioTemperature(temp);

        // give time to other tasks
        vTaskDelay(delayT10);
    }
}

Variometer::Variometer(VarioBeeper *_varioBeeper, VarioSD *_varioSD)
{
    varioBeeper = _varioBeeper;
    varioSD = _varioSD;
    kalmanvert = new Kalmanvert();
    varioImu = new VarioImu(kalmanvert);
    varioGPS = new VarioGPS();
    if (params->P_BT_ENABLE->getValue())
    {
        varioBle = new VarioBle();
    }
    varioHisto = new VarioHisto<50, 40>();
    speedHisto = new SpeedHisto<500, 120, 2>();
}

void Variometer::init()
{
    varioImu->init();
    varioBeeper->startTask();
    varioGPS->init();
    varioGPS->startTask();
    if (params->P_BT_ENABLE->getValue())
    {
        varioBle->init();
        varioBle->startTask();
    }
    if (params->P_MUTE_VARIOBEGIN->getValue())
    {
        varioBeeper->mute();
    }
}

double Variometer::preTaskInitFirstAlti()
{
    return varioImu->postInitFirstAlti();
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

void Variometer::disableAcquisition()
{
    varioImu->disableAcquisition();
}

extern TaskHandle_t _taskVarioBleHandle;