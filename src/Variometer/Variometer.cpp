

#include "Variometer.h"

#define COEF_ALTI_FILTERED 0.1

Variometer::Variometer(VarioBeeper *_varioBeeper)
{
    kalmanvert = new Kalmanvert();
    varioImu = new VarioImu(kalmanvert);
    varioBeeper = _varioBeeper;
}
void Variometer::init()
{
    varioImu->init();

    // ici la loop du vario
    double alti;
    double altiFiltered = 0;
    double temperature;
    double accel;
    double velocity;
    double calibratedAlti;
    bool lastSentence;
    varioBeeper->startTask();

    while (true)
    {
        if (varioImu->updateData())
        {
            velocity = kalmanvert->getVelocity();
            varioBeeper->setVelocity(velocity);
            Serial.print("velocity:");
            Serial.println(velocity);
        }
        vTaskDelay(pdMS_TO_TICKS(100));

        //           if( twScheduler.havePressure() && twScheduler.haveAccel() ) {
        //     kalmanvert.update( twScheduler.getAlti(),
        //                        twScheduler.getAccel(NULL),
        //                        millis() );

        //     beeper.setVelocity( kalmanvert.getVelocity() );

        //   }

        //         if (varioImu->havePressure() && varioImu->haveAccel())
        //         {
        //             kalmanvert->update(varioImu->getAlti(),
        //                                varioImu->getAccel(NULL),
        //                                millis());
        //         }

        //         if (varioImu->havePressure() && varioImu->haveAccel())
        //         {
        //             kalmanvert->update(varioImu->getAlti(),
        //                                varioImu->getAccel(),
        //                                millis());

        //             if (varioImu->updateData())
        //             {
        //                 alti = varioImu->getAlti();

        //                 if (altiFiltered != 0)
        //                 {
        //                     altiFiltered = altiFiltered + COEF_ALTI_FILTERED * (alti - altiFiltered);
        //                 }
        //                 else
        //                 {
        //                     altiFiltered = alti; // first reading so set filtered to reading
        //                 }
        //                 Serial.print("altiFiltered");
        //                 Serial.println(altiFiltered);

        //                 temperature = varioImu->getTemp();
        //                 accel = varioImu->getAccel();
        //                 unsigned long myTime = millis();
        //                 varioImu->updateKalman(altiFiltered, accel, myTime);
        //                 //       velocity = kalmanvert.getVelocity();
        //                 // calibratedAlti = kalmanvert.getCalibratedPosition();

        //                 velocity = kalmanvert->getVelocity();
        //                 calibratedAlti = kalmanvert->getCalibratedPosition();

        //                 if (calibratedAlti < 0)
        //                     calibratedAlti = 0;
        //             }

        //             // varioGps = new VarioGps();
        //             // varioGps->init();

        //             // varioGps->update(kalmanvert, &lastSentence);
        //             varioBeeper->setVelocity(velocity);
        //             Serial.print("velocity:");
        //             Serial.println(velocity);
        //             vTaskDelay(pdMS_TO_TICKS(100));
    }
}
