#include <Arduino.h>

#include "VarioManager.h"
#include "VarioDebug/VarioDebug.h"
#include "event.h"
// TaskHandle_t VarioManager::vmTaskHandler;
// SemaphoreHandle_t VarioManager::vmMutex;

VarioManager::VarioManager()
{
    varioBeeper = new VarioBeeper();
    varioPower = new VarioPower();
    varioSD = new VarioSD();
    varioDisplay = new VarioDisplay();
    varioButton = new VarioButton();
    varioLanguage = new VarioLanguage();
}

boolean VarioManager::init()
{
    VARIO_PROG_DEBUG_PRINTLN("vm init");
    VARIO_PROG_DEBUG_TRACE();

    // VARIO_PROG_DEBUG_PRINTLN("vm init task");
    // vmMutex = xSemaphoreCreateBinary();
    // xSemaphoreGive(vmMutex);
    // xTaskCreate(this->startTaskImpl, "TaskManager", 4000, this, 10, &vmTaskHandler);

    varioBeeper->init(5);
    varioBeeper->generateToneSuccess();

    // init mesure tension
    varioPower->init();
    // inscription des valeurs mesurées dans FC
    setPowerDataToFC();

    if (!varioSD->init())
    {
        VARIO_PROG_DEBUG_PRINTLN("Echec initialisation SD");
        varioBeeper->generateToneFailure();
        esp_deep_sleep_start();
        return false;
    }
    else
    {
        VARIO_PROG_DEBUG_PRINTLN("Succes initialisation SD");
        varioBeeper->generateToneSuccess();
    }

    // varioSD->test();

    if (!varioSettings.init())
    {
        varioBeeper->generateToneFailure();
        esp_deep_sleep_start();
        return false;
    }

    /***** Affiche parametre  ***/
    VARIO_PROG_DEBUG_DUMP(varioData.getParam(PARAM_PILOT_NAME)->getValueChar());

    varioLanguage->init(varioData.getParam(PARAM_LANGUAGE)->getValueUInt8());
    
    varioButton->registerObserver(&fsm);
    varioButton->startTask();

    // initialisation de l'affichage
    varioDisplay->init(varioLanguage);

    fsm.initfsm(varioDisplay);

    fsm.registerObserver(this);
    fsm.registerObserver(varioBeeper);

    // if (esp32FOTA.isArchWwwExist())
    // {
    //     varioBeeper->generateTone(440, 100);
    //     varioBeeper->generateTone(880, 100);
    //     varioBeeper->generateTone(440, 100);
    //     varioDisplay->displayPageMessage(varioLanguage->getText(TITRE_UPDATEWWW).c_str());
    //     if (esp32FOTA.UpdateWwwDirectoryFromGz() == 1)
    //     {
    //         //Mise à jour
    //         varioBeeper->generateTone(659, 150);
    //         varioBeeper->generateTone(1318, 150);
    //         varioBeeper->generateTone(2636, 150);

    //         VARIO_PROG_DEBUG_PRINTLN("RESTART ESP32");
    //         setCurrentPage(STATE_PAGE_REBOOT);
    //     }
    // }

    // démarrage de l'ecoute des boutons
    // varioButton->startTask();
    // VARIO_PROG_DEBUG_PRINTLN("Démarrage boutons");

    // setCurrentPage(STATE_PAGE_INIT_DONE);

    // variometer = new Variometer(varioBeeper);
    // variometer->init();
    launchTimers();

    return true;
}

// void VarioManager::startTaskImpl(void *parm)
// {
//     // wrapper for task
//     static_cast<VarioManager *>(parm)->vmTask();
// }

// void VarioManager::vmTask()
// {
//     while (true)
//     {
//         /* wait */
//         xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

//         /* launch interrupt */
//         xSemaphoreTake(vmMutex, portMAX_DELAY);

//         switch (currentStatePage)
//         {
//         case STATE_PAGE_INIT:
//             VARIO_PROG_DEBUG_PRINTLN("STATE_PAGE_INIT");
//             //            varioDisplay->displayPageInit(true);
//             break;
//         case STATE_PAGE_INIT_DONE:
//             VARIO_PROG_DEBUG_PRINTLN("STATE_PAGE_INIT_DONE");
//             //            varioDisplay->displayPageInit(false);
//             break;
//         case STATE_PAGE_WEBSERV:
//             // demarrage du wifi
//             VARIO_PROG_DEBUG_PRINTLN("STATE_PAGE_WEBSERV");
//             //     currentMode = MODE_WIFI;
//             //            varioDisplay->displayPageWifi("", "");
//             //            varioWifi = new VarioWifi(this);
//             varioWifi->startTask();
//             break;
//         case STATE_PAGE_REBOOT:
//             VARIO_PROG_DEBUG_PRINTLN("STATE_PAGE_REBOOT");
//             //            varioDisplay->displayPageReboot("");
//             vTaskDelay(delayT100 * 10);
//             ESP.restart();
//         case STATE_PAGE_CALIBRATION:
//             VARIO_PROG_DEBUG_PRINTLN("STATE_PAGE_CALIBRATION");
//             //     currentMode = MODE_CALIBRATION;
//             varioBeeper->unMute();
//             //            varioDisplay->displayPageMessage(varioLanguage->getText(TITRE_CALIBR).c_str());
//             varioCalibration = new VarioCalibration();
//             // @TODO doit etre mis dans une tache pour ne pas bloquer le manager
//             varioCalibration->begin(varioBeeper);

//             break;
//         }
//         xSemaphoreGive(vmMutex);
//     }
// }

// void VarioManager::setCurrentPage(uint8_t page)
// {
//     if (page == currentStatePage)
//     {
//         return;
//     }

//     currentStatePage = page;

//     if (xSemaphoreTake(vmMutex, portMAX_DELAY) == pdTRUE)
//     {
//         xSemaphoreGive(vmMutex);
//         xTaskNotify(vmTaskHandler, 0, eNoAction);
//     }
// }

// uint8_t VarioManager::getCurrentPageState()
// {
//     return currentStatePage;
// }

// void VarioManager::handleButton(uint8_t btn)
// {
//     varioBeeper->generateTone(500, 150);
//     switch (btn)
//     {
//     case BTN_SHORT_A:
//         VARIO_BTN_DEBUG_PRINTLN("Appui court BTN A");
//         if (currentStatePage == STATE_PAGE_INIT_DONE)
//         {
//             // demarrage du wifi
//             setCurrentPage(STATE_PAGE_WEBSERV);
//         }
//         else if (currentStatePage == STATE_PAGE_WEBSERV)
//         {
//             // redemarrage du vario
//             setCurrentPage(STATE_PAGE_REBOOT);
//         }
//         break;
//     case BTN_SHORT_B:
//         VARIO_BTN_DEBUG_PRINTLN("Appui court BTN B");
//         break;
//     case BTN_SHORT_C:
//         VARIO_BTN_DEBUG_PRINTLN("Appui court BTN C");
//         if (currentStatePage == STATE_PAGE_INIT_DONE)
//         {
//             // demarrage du wifi
//             setCurrentPage(STATE_PAGE_CALIBRATION);
//         }
//         break;
//     case BTN_LONG_A:
//         VARIO_BTN_DEBUG_PRINTLN("Appui long BTN A");
//         if (currentStatePage == STATE_PAGE_CALIBRATION)
//         {
//             // redemarrage du vario
//             setCurrentPage(STATE_PAGE_REBOOT);
//         }
//         break;
//     case BTN_LONG_B:
//         VARIO_BTN_DEBUG_PRINTLN("Appui long BTN B");
//         break;
//     case BTN_LONG_C:
//         VARIO_BTN_DEBUG_PRINTLN("Appui long BTN C");
//         break;
//     }
// }

void VarioManager::launchTimers()
{
    TimerHandle_t timerHndl10Sec;
    timerHndl10Sec = xTimerCreate(
        "timer10Sec",              /* name */
        pdMS_TO_TICKS(10000),      /* period/time */
        pdTRUE,                    /* auto reload */
        static_cast<void *>(this), /* timer ID */
        startTimers10sImpl);       /* callback */

    if (xTimerStart(timerHndl10Sec, 0) != pdPASS)
    {
        for (;;)
            ; /* failure!?! */
    }
}

void VarioManager::startTimers10sImpl(void *parm)
{
    // wrapper for timer
    static_cast<VarioManager *>(parm)->timer10s();
}

void VarioManager::timer10s()
{
    setPowerDataToFC();
}

void VarioManager::setPowerDataToFC()
{
    fc.power.tension = varioPower->getTension();
    fc.power.capacite = varioPower->getCapacitePct();
}

void VarioManager::onSignalReceived(uint8_t _val)
{
    switch (_val)
    {
        {
        case WIFI_START_ASKED:
            VARIO_WIFI_DEBUG_PRINTLN("Démarrage du wifi");
            varioWifi = new VarioWifi();
            varioWifi->startTask();
            break;
        case CALIBRATION_START_ASKED:
            Serial.println("Démarrage de la calibration");
            break;
        default:
            break;
        }
    }
}