#ifndef VARIO_WIFI_H
#define VARIO_WIFI_H

// #define DEST_FS_USES_SD

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
//#include "VarioSettings/VarioSettings.h"
#include "esp32FOTA2/src/esp32fota2.h"
#include "VarioIgcParser/VarioIgcParser.h"
#include "VarioSqlFlight/VarioSqlFlight.h"
#include "VarioWebHandler.h"

class VarioManager;

class VarioWifi
{
private:
    VarioManager *vm;
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
    VarioWifi(VarioManager *_vm);
    void startTask();
    bool begin();
};

#endif //VARIO_WIFI_H