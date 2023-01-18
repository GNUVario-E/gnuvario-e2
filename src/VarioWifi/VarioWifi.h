#pragma once

#include "FC.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include "esp32FOTA2/src/esp32fota2.h"
#include "VarioIgcParser/VarioIgcParser.h"
#include "VarioSqlFlight/VarioSqlFlight.h"
#include "VarioWebHandler.h"
#include "VarioParameter/Parameters.h"

class VarioWifi
{
private:
    Parameters *params = Parameters::getInstance();

    //    static VarioWebHandler *varioWebHandler;
    const TickType_t delayT50 = pdMS_TO_TICKS(50); //(1) / portTICK_PERIOD_MS;
    TaskHandle_t _taskVarioWifiHandle = NULL;
    static void startTaskImpl(void *);
    void task();
    bool connectToWifi();
    bool isIndexFileExist();
    void setClock(void);
    bool checkDbVersion();
    void setMDNS();
    void startWebServer();

public:
    VarioWifi();
    void startTask();
    bool begin();
};

extern FC fc;
