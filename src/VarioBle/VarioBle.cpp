#include "VarioBle.h"
#include "VarioDebug/VarioDebug.h"
#include "tasksBitsMask.h"

#define BLE_TASK_PRIORITY 7
#define BT_PACKET_SIZE 20

bool VarioBle::deviceConnected = false;
bool VarioBle::oldDeviceConnected = false;
TaskHandle_t VarioBle::_taskVarioBleHandle = NULL;

VarioBle::VarioBle()
{
}

void VarioBle::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskVarioBle started");
    xTaskCreatePinnedToCore(this->startTaskImpl, "TaskVarioBle", 5000, this, BLE_TASK_PRIORITY, &_taskVarioBleHandle, 1);
}

void VarioBle::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<VarioBle *>(parm)->task();
}

void VarioBle::task()
{
    uint32_t ulNotifiedValue;

    while (1)
    {
        if (xTaskNotifyWait(pdFALSE, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY) == pdPASS)
        {
            // Serial.println("BLE task notified");
            // Serial.println(ulNotifiedValue, BIN);
            // notify received
            if (deviceConnected)
            {
                if ((ulNotifiedValue & BLE_LXWP0_SENTENCE_BIT) != 0)
                {
                    // Serial.println("send trame LXWP0");
                    // trame LXWP0
                    sendTrameLXWP0();
                    // give time to other tasks
                    vTaskDelay(delayT20);
                }
                else if ((ulNotifiedValue & BLE_GPS_SENTENCE_BIT) != 0)
                {
                    // Serial.println("send trame GPS");
                    // trame GPS
                    sendTramesGps();
                    // give time to other tasks
                    vTaskDelay(delayT20);
                }
            }

            if ((ulNotifiedValue & BLE_CON_DISCON_SENTENCE_BIT) != 0)
            {
                // disconnecting
                if (!deviceConnected && oldDeviceConnected)
                {
                    vTaskDelay(delayT20 * 15);   // give the bluetooth stack the chance to get things ready
                    pServer->startAdvertising(); // restart advertising
                    Serial.println("start advertising");
                    oldDeviceConnected = deviceConnected;
                }
                // connecting
                if (deviceConnected && !oldDeviceConnected)
                {
                    // do stuff here on connecting
                    oldDeviceConnected = deviceConnected;
                }
            }
        }
    }
}

void VarioBle::init()
{
    // Create the BLE Device
    BLEDevice::init(BLE_SERVER_NAME);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        NIMBLE_PROPERTY::NOTIFY);

    /***************************************************
     NOTE: DO NOT create a 2902 descriptor
     it will be created automatically if notifications
     or indications are enabled on a characteristic.

     pCharacteristic->addDescriptor(new BLE2902());
    ****************************************************/

    // pas de caracteristique de recuperation de donnees
    // BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
    //     CHARACTERISTIC_UUID_RX,
    //     NIMBLE_PROPERTY::WRITE);

    // pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}

void VarioBle::sendTramesGps()
{
    uint8_t buffer[BT_PACKET_SIZE];
    uint8_t bufferPos = 0;
    uint8_t i = 0;

    // GPS sentences
    if (fc.getGpsSentenceTimestamp() > lastTimestampGpsSentence)
    {
        lastTimestampGpsSentence = fc.getGpsSentenceTimestamp();
        char sentence[100];
        strcpy(sentence, fc.getGpsSentence());
        sendSentence(sentence);
    }
}

void VarioBle::sendTrameLXWP0()
{
    // format de la trame LXWP0
    // $LXWP0,loger_stored (Y/N), IAS(kph), baroaltitude (m), vario (m/s),,,,,,heading of plane,windcourse (deg),windspeed(kph)*checksum\r\n
    // $LXWP0,Y,22.3,1665.5,1.71,1.71,1.71,1.71,1.71,1.71,239,174,10.1

    char line[100];
    uint8_t prefix = '*';
    uint8_t parity = 0;

    if (fc.getVarioAltiTimestamp() > lastTimestampLXWP0Sentence)
    {
        lastTimestampLXWP0Sentence = fc.getVarioAltiTimestamp();
        sprintf_P(line, PSTR("$LXWP0,Y,,%d.%02d,%d.%02d,,,,,,,,,"),
                  (int)fc.getVarioAlti(), (int)(fc.getVarioAlti() * 100) % 100,
                  (int)fc.getVarioVelocity(), (int)(fc.getVarioVelocity() * 100) % 100);

        uint8_t strSize = strlen(line);
        for (uint8_t i = 1; i < strSize; i++)
        {
            parity ^= line[i];
        }
        char parityBuffer[3];

        sprintf(parityBuffer, "%02x", parity);
        strcat(line, "*");
        strcat(line, parityBuffer);

        sendSentence(line);
    }
}

void VarioBle::deviceConnect()
{
    deviceConnected = true;
    xTaskNotify(VarioBle::_taskVarioBleHandle, BLE_CON_DISCON_SENTENCE_BIT, eSetBits);
}

void VarioBle::deviceDisconnect()
{
    deviceConnected = false;
    xTaskNotify(VarioBle::_taskVarioBleHandle, BLE_CON_DISCON_SENTENCE_BIT, eSetBits);
}

void VarioBle::sendSentence(const char *sentence)
{
    uint8_t buffer[BT_PACKET_SIZE];
    uint8_t bufferPos = 0;
    uint8_t i = 0;

    uint8_t strSize = strlen(sentence);
    if (strSize > 0)
    {
        while (i < strSize - 1)
        {
            while (bufferPos < BT_PACKET_SIZE && i < (strSize - 1))
            {
                buffer[bufferPos] = sentence[i];
                bufferPos++;
                i++;
            }
            pTxCharacteristic->setValue(buffer, bufferPos);
            pTxCharacteristic->notify();
            bufferPos = 0;
            buffer[bufferPos] = '\0';
            vTaskDelay(delayT2); // bluetooth stack will go into congestion, if too many packets are sent
        }
        pTxCharacteristic->setValue("\r\n");
        pTxCharacteristic->notify();

        vTaskDelay(delayT2); // bluetooth stack will go into congestion, if too many packets are sent
    }
}