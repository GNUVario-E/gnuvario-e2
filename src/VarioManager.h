#pragma once

#include <Arduino.h>
#include "Observer/Observer.h"
#include "FC.h"
#include "VarioFSM/VarioFSM.h"
#include "VarioSettings/VarioSettings.h"
#include "VarioBeeper/VarioBeeper.h"
#include "VarioPower/VarioPower.h"
#include "VarioSD/VarioSD.h"
#include "VarioData.h"
#include "VarioDisplay/VarioDisplay.h"
#include "VarioSettings/VarioSettings.h"
#include "VarioWifi/VarioWifi.h"
#include "VarioLanguage/VarioLanguage.h"
#include "VarioButton/VarioButton.h"
#include "VarioCalibration/VarioCalibration.h"
#include "Variometer/Variometer.h"

// #include "freertos/FreeRTOS.h"
// #include "freertos/semphr.h"
// #include "freertos/task.h"

// #define STATE_PAGE_INIT 0U
// #define STATE_PAGE_INIT_DONE 1U
// #define STATE_PAGE_VARIO 2U
// #define STATE_PAGE_CONFIG 3U
// #define STATE_PAGE_STAT 4U
// #define STATE_PAGE_GPSCAL 5U
// #define STATE_PAGE_WEBSERV 6U
// #define STATE_PAGE_CONFIG_SOUND 7U
// #define STATE_PAGE_DEEP_SLEEP 8U
// #define STATE_PAGE_CALIBRATION 9U
// #define STATE_PAGE_CALIBRATE 10U
// #define STATE_PAGE_CHARGE 11U
// #define STATE_PAGE_REBOOT 12U
// #define STATE_PAGE_NONE 99U

// #define BTN_SHORT_A 1U
// #define BTN_LONG_A 2U
// #define BTN_SHORT_B 3U
// #define BTN_LONG_B 4U
// #define BTN_SHORT_C 5U
// #define BTN_LONG_C 6U

class VarioManager : public Observer
{
private:
    const TickType_t delayT100 = pdMS_TO_TICKS(100);
    void launchTimers();
    static void startTimers10sImpl(void *);
    void timer10s();
    void setPowerDataToFC();

public:
    VarioManager();

    VarioBeeper *varioBeeper;
    VarioPower *varioPower;
    VarioSD *varioSD;
    VarioDisplay *varioDisplay;
    VarioButton *varioButton;
    VarioWifi *varioWifi;
    VarioLanguage *varioLanguage;
    VarioCalibration *varioCalibration;
    Variometer *variometer;

    boolean init();
    void onSignalReceived(uint8_t _val);
};

extern fcdata_t fc;
extern VarioFSM fsm;