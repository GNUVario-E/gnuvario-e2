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
#include "AglManager/src/AglManager.h"
#include "VarioIgc/VarioIgc.h"
class VarioManager : public Observer
{
private:
    const TickType_t delayT100 = pdMS_TO_TICKS(100);
    void launchTimers();
    static void startTimers10sImpl(void *);
    void timer10s();
    void setPowerDataToFC();
    void addLineToIgc();

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
    AglManager *aglManager;
    VarioIgc *varioIgc;
    boolean init();
    void onSignalReceived(uint8_t _val);
};

extern FC fc;
extern VarioFSM fsm;
