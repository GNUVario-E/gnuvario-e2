#pragma once

#include "FC.h"
#include <NimBLEDevice.h>

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_SERVER_NAME "GNUVARIOE2"

class VarioBle
{
private:
    static bool deviceConnected;
    static bool oldDeviceConnected;
    const TickType_t delayT100 = pdMS_TO_TICKS(100);
    const TickType_t delayT2 = pdMS_TO_TICKS(2);
    TaskHandle_t _taskVarioGPSHandle = NULL;
    static void startTaskImpl(void *);
    void task();
    BLEServer *pServer = NULL;
    BLECharacteristic *pTxCharacteristic;
    void sendTrameLXWP0();
    uint32_t lastTimestampGpsSentence = 0;
    void sendTrames();

public:
    VarioBle();
    void init();
    static void deviceConnect();
    static void deviceDisConnect();
    void startTask();
};

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        VarioBle::deviceConnect();
    };

    void onDisconnect(BLEServer *pServer)
    {
        VarioBle::deviceDisConnect();
    }
};

extern FC fc;