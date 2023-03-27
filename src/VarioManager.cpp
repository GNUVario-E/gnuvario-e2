#include <Arduino.h>
#include "VarioManager.h"
#include "VarioDebug/VarioDebug.h"
#include "event.h"
#include "driver/rtc_io.h"

#define POWER_DOWN_TENSION 3.5

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

    if (params->P_BEEP_VARIOBEGIN->getValue())
    {
        varioBeeper->generateToneSuccess();
    }

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
        VarioSD::isStarted = true;
        VARIO_PROG_DEBUG_PRINTLN("Succes initialisation SD");
        varioBeeper->generateToneSuccess();
    }

    // Affiche parametre
    VARIO_PROG_DEBUG_DUMP(params->P_PILOT_NAME->getValue());

    varioLanguage->init(params->P_LANGUAGE->getValue());

    // initialisation des paramètres de manière global
    // lecture des fichiers correspondant
    char fileParams[] = "/params.jso";
    char fileWifi[] = "/wifi.cfg";
    char fileCalib[] = "/variocal.cfg";
    char fileScreen[] = "/screen.json";
    if (!varioSettings.loadConfigurationVario(fileParams))
    {
        // Cannot read settings
        varioBeeper->generateToneFailure();
        esp_deep_sleep_start();

        return false;
    }
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
        "timer10Sec",         /* name */
        pdMS_TO_TICKS(10000), /* period/time */
        pdTRUE,               /* auto reload */
        (void *)this,         /* timer ID */
        startTimers10sImpl);  /* callback */

    if (xTimerStart(timerHndl10Sec, 0) != pdPASS)
    {
        for (;;)
            ; /* failure!?! */
    }
}

void VarioManager::startTimers10sImpl(TimerHandle_t timerHndl10Sec)
{
    // wrapper for timer
    VarioManager *obj;
    obj = (VarioManager *)pvTimerGetTimerID(timerHndl10Sec);
    // pvTimerGetTimerID(timerHndl10Sec);
    obj->timer10s();
}

void VarioManager::timer10s()
{
    setPowerDataToFC();
}

void VarioManager::setPowerDataToFC()
{
    if (fc.getPowerTension() != 0)
    {
        if (fc.getPowerTension() < POWER_DOWN_TENSION)
        {
            deepSleep("Low battery", "");
        }

        // lissage
        fc.setPowerTension(fc.getPowerTension() + 0.1 * (varioPower->getTension() - fc.getPowerTension()), millis());
    }
    else
    {
        fc.setPowerTension(varioPower->getTension(), millis());
    }

    if (fc.getPowerCapacite() != 0)
    {
        // lissage
        fc.setPowerCapacite(fc.getPowerCapacite() + 0.1 * (varioPower->getCapacitePct() - fc.getPowerCapacite()), millis());
    }
    else
    {
        fc.setPowerCapacite(varioPower->getCapacitePct(), millis());
    }
}

void VarioManager::onSignalReceived(uint8_t _val)
{
    switch (_val)
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
    case HISTO_START_ASKED:
        varioStat = new VarioStat(varioLanguage->getText(TITRE_STAT));
        break;
    case VARIO_START_ASKED:
        VARIO_CAL_DEBUG_PRINTLN("Démarrage du vario");
        varioDisplay->setMinTimeRefresh(1000);
        variometer = new Variometer(varioBeeper, varioSD);
        fc.registerObserver(this);
        variometer->init();
        variometer->startTask();
        varioWind = new VarioWind();
        aglManager = new AglManager();
        aglManager->init();

        break;
    case AGL_INIT_ASKED:
        VARIO_CAL_DEBUG_PRINTLN("AGL init altitude");
        variometer->initFromAgl();
        break;
    case GPS_NEW_POSITION:
        aglManager->setLatitude(fc.getGpsLat());
        aglManager->setLongitude(fc.getGpsLon());

        varioWind->addPoint(fc.getGpsLat(), fc.getGpsLon(), fc.getGpsLocTimestamp());
        fc.setAglAlt(aglManager->getAgl(), millis());
        break;
    case GPS_NO_POSITION:
        aglManager->setLatitude(-1);
        aglManager->setLongitude(-1);
        fc.setAglAlt(-1, 0);
        break;
    case GPS_NEW_ALTI_METERS:
        aglManager->setAltiGps(fc.getGpsAltiMeters());
        aglManager->setAlti(fc.getVarioAlti());
        fc.setAglGroundLvl(aglManager->getGroundLevel(), millis());
        break;
    case VARIO_NEW_ALTI:
        break;
    case GPS_FIXED:
        if (params->P_BEEP_GPSFIX->getValue())
        {
            varioBeeper->generateToneSuccess();
        }
        break;
    case GPS_LOST_FIXED:
        if (params->P_BEEP_GPSFIX->getValue())
        {
            varioBeeper->generateToneSoftFailure();
        }
        break;
    case FLIGHT_START:
        if (params->P_BEEP_FLYBEGIN->getValue())
        {
            varioBeeper->generateToneSuccess();
            varioBeeper->generateToneSuccess();
        }
        if (params->P_MUTE_VARIOBEGIN->getValue())
        {
            varioBeeper->unMute();
        }
        if (!params->P_NO_RECORD->getValue())
        {
            // start IGC recording
            varioIgc = new VarioIgc();

            if (varioIgc->createNewIgcFile(params->P_PILOT_NAME->getValue(), params->P_GLIDER_NAME1->getValue(), fc.getGpsDateDay(), fc.getGpsDateMonth(), fc.getGpsDateYear(), params->P_TIME_ZONE->getValue()))
            {
                varioIgc->startTimer();
            }
        }
        varioWind->init();

        break;
    case FLIGHT_START_ASKED:
        fc.checkFlightStart(true);
        break;
    case DEEP_SLEEP_ASKED:
        deepSleep("Standby", "Middle button to wake");
        break;
    case HISTO_DISPLAY_ENTER:
        // set text values for histo screen
        varioStat->setValuesToFc();
        break;
    case VOLUME_EDIT_ENTER:
        fc.setSoundIsInEditMode(true);
        break;
    case VOLUME_EDIT_EXIT:
        fc.setSoundIsInEditMode(false);
        break;
    default:
        break;
    }
}

void VarioManager::deepSleep(const char *txt1, const char *txt2 = "")
{
    varioBeeper->generateToneFailure();

    esp_sleep_enable_ext0_wakeup(GPIO_BUTTON_B, 0); // 1 = High, 0 = PIN_BUTTON_B

    fc.setText1(true, txt1);
    fc.setText2(true, txt2);
    varioDisplay->displayScreen(varioDisplay->messageScreen);

    delay(1000);

    pinMode(PIN_POWER, OUTPUT);
    digitalWrite(PIN_POWER, !PIN_POWER_STATE); // turn off POWER

    varioBeeper->mute();

    variometer->disableAcquisition();

    rtc_gpio_isolate(GPIO_BUTTON_A);
    rtc_gpio_isolate(GPIO_BUTTON_C);

    rtc_gpio_isolate(GPIO_MPU_INT_PIN);

    varioDisplay->powerOff();

    delay(1000);

    esp_deep_sleep_start();
}