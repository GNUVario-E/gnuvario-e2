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
    void bufferTask();
    void buildScreens();
    void updateScreen(void);
    bool _doDisplay = false;
    VarioLanguage *varioLanguage;

public:
    VarioDisplay();
    static SemaphoreHandle_t screenMutex;
    void init(VarioLanguage *varioLanguage);
    void displayScreen(VarioScreen *screen);
    void stopDisplay();

    VarioScreen *_currentScreen;
    VarioScreen *wifiScreen;
    VarioScreen *calibrationScreen;
    VarioScreen *bootScreen;
    VarioScreen *vario1Screen;
    VarioScreen *vario2Screen;
    VarioScreen *vario3Screen;
    VarioScreen *soundScreen;
    VarioScreen *statisticScreen;
};
