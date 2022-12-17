#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "VarioLanguage/VarioLanguage.h"
#include "VarioDisplay/VarioScreen/VarioScreen.h"
#include "VarioDisplay/VarioScreen/ScreenData.h"

#define SCREEN_STACK_SIZE 2000
#define SCREEN_CORE 1
#define SCREEN_PRIORITY 10

class VarioDisplay
{
private:
    const TickType_t delayT100 = pdMS_TO_TICKS(100);
    static TaskHandle_t screenTaskHandler;
    static TaskHandle_t bufferTaskHandler;
    static void screenTask(void *parameter);
    static void startTaskBuffer(void *);
    static uint32_t lastDisplayTime;
    static uint32_t lastDisplayTimeToggle;
    static uint16_t minTimeRefresh ;
    void bufferTask();
    void buildScreens();
    void updateScreen(void);
    VarioLanguage *varioLanguage;

public:
    VarioDisplay();
    static SemaphoreHandle_t screenMutex;
    static SemaphoreHandle_t displayMutex;
    void init(VarioLanguage *varioLanguage);
    void displayScreen(VarioScreen *screen);
    void stopDisplay();
    void powerOff();
    void setMinTimeRefresh(uint16_t minTimeRefresh);

    VarioScreen *_currentScreen;
    VarioScreen *wifiScreen;
    VarioScreen *calibrationScreen;
    VarioScreen *bootScreen;
    VarioScreen *vario1Screen;
    VarioScreen *vario2Screen;
    VarioScreen *vario3Screen;
    VarioScreen *soundScreen;
    VarioScreen *statisticScreen;
    VarioScreen *rebootScreen;
    VarioScreen *messageScreen;
};

extern ScreenData bootScreenData;
extern ScreenData wifiScreenData;
extern ScreenData calibrationScreenData;
extern ScreenData vario1ScreenData;
extern ScreenData vario2ScreenData;
extern ScreenData vario3ScreenData;
extern ScreenData soundScreenData;
extern ScreenData statisticScreenData;
extern ScreenData rebootScreenData;
extern ScreenData messageScreenData;