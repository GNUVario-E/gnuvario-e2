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

    // Affiche parametre
    VARIO_PROG_DEBUG_DUMP(varioData.getParam(PARAM_PILOT_NAME)->getValueChar());

    varioLanguage->init(varioData.getParam(PARAM_LANGUAGE)->getValueUInt8());

    // initialisation des paramètres de manière global
    // lecture des fichiers correspondant
    char fileParams[] = "/params.jso";
    char fileWifi[] = "/wifi.cfg";
    char fileCalib[] = "/variocal.cfg";
    char fileScreen[] = "/screen.json";
    varioSettings.loadConfigurationVario(fileParams);
    varioSettings.readSDSettings(fileWifi);
    varioSettings.readSDSettings(fileCalib);
    varioSettings.loadScreenVario(fileScreen);

    varioButton->registerObserver(&fsm);
    varioButton->startTask();

    // initialisation de l'affichage
    varioDisplay->init(varioLanguage);

    fsm.initfsm(varioDisplay);

    fsm.registerObserver(this);
    fsm.registerObserver(varioBeeper);
    fsm.startTask();

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

    launchTimers();

    return true;
}

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
    if (fc.power.tension != 0)
    {
        // lissage
        fc.power.tension = fc.power.tension + 0.1 * (varioPower->getTension() - fc.power.tension);
    }
    else
    {
        fc.power.tension = varioPower->getTension();
    }

    if (fc.power.capacite != 0)
    {
        // lissage
        fc.power.capacite = fc.power.capacite + 0.1 * (varioPower->getCapacitePct() - fc.power.capacite);
    }
    else
    {
        fc.power.capacite = varioPower->getCapacitePct();
    }
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
            VARIO_CAL_DEBUG_PRINTLN("Démarrage de la calibration");
            varioCalibration = new VarioCalibration();
            varioCalibration->begin(varioBeeper);
            varioCalibration->startTask();
            break;
        case VARIO_START_ASKED:
            VARIO_CAL_DEBUG_PRINTLN("Démarrage du vario");
            variometer = new Variometer(varioBeeper);
            variometer->init();
            variometer->startTask();
        default:
            break;
        }
    }
}
