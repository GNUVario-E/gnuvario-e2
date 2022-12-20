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
    const TickType_t delayT20 = pdMS_TO_TICKS(20);
    const TickType_t delayT2 = pdMS_TO_TICKS(2);

    static void startTaskImpl(void *);
    void task();
    BLEServer *pServer = NULL;
    BLECharacteristic *pTxCharacteristic;
    uint32_t lastTimestampGpsSentence = 0;
    uint32_t lastTimestampLXWP0Sentence = 0;
    void sendTramesGps();
    void sendTrameLXWP0();

public:
    static TaskHandle_t _taskVarioBleHandle;
    VarioBle();
    void init();
    static void deviceConnect();
    static void deviceDisconnect();
    void startTask();
    void sendSentence(const char *sentence);
};

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        VarioBle::deviceConnect();
    };

    void onDisconnect(BLEServer *pServer)
    {
        VarioBle::deviceDisconnect();
    }
};

extern FC fc;