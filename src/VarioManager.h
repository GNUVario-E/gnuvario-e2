#pragma once

#include "FC.h"
#include "VarioFSM/VarioFSM.h"
#include "VarioSettings/VarioSettings.h"
#include "VarioBeeper/VarioBeeper.h"
#include "VarioDisplay/VarioDisplay.h"
#include "VarioPower/VarioPower.h"
#include "VarioButton/VarioButton.h"
// #include "VarioSD/VarioSD.h"
// #include "VarioWifi/VarioWifi.h"
// #include "VarioLanguage/VarioLanguage.h"
// #include "VarioCalibration/VarioCalibration.h"
// #include "Variometer/Variometer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

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

// #define MODE_INIT 1U
// #define MODE_WIFI 2U
// #define MODE_CALIBRATION 3U

// #define BTN_SHORT_A 1U
// #define BTN_LONG_A 2U
// #define BTN_SHORT_B 3U
// #define BTN_LONG_B 4U
// #define BTN_SHORT_C 5U
// #define BTN_LONG_C 6U

class VarioManager
{
private:
    static void startTaskImpl(void *);
    TaskHandle_t vmTaskHandler;
    SemaphoreHandle_t vmMutex;
    void vmTask();
    const TickType_t delayT100 = pdMS_TO_TICKS(100);
    // uint8_t currentStatePage = STATE_PAGE_NONE;
    // uint16_t currentMode = MODE_INIT;
    void launchTimers();
    static void startTimers10sImpl(void *);
    void timer10s();
    void setPowerDataToFC();

public:
    VarioManager();

    VarioBeeper *varioBeeper;
    VarioPower *varioPower;
    VarioDisplay *varioDisplay;
    VarioButton *varioButton;
    // VarioSD *varioSD;
    // VarioWifi *varioWifi;
    // VarioLanguage *varioLanguage;
    // VarioCalibration *varioCalibration;
    // Variometer *variometer;

    // void setCurrentPage(uint8_t page);
    // uint8_t getCurrentPageState();
    // void handleButton(uint8_t btn);
    boolean init();
};

extern fcdata_t fc;
extern VarioFSM fsm;