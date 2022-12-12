#include "VarioBle.h"
#include "VarioDebug/VarioDebug.h"

#define BLE_TASK_PRIORITY 8

bool VarioBle::deviceConnected = false;
bool VarioBle::oldDeviceConnected = false;

VarioBle::VarioBle()
{
}

void VarioBle::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskVarioBle started");
    // xTaskCreate(this->startTaskImpl, "TaskVarioBle", 5000, this, BLE_TASK_PRIORITY, &_taskVarioGPSHandle);
    xTaskCreatePinnedToCore(this->startTaskImpl, "TaskVarioBle", 5000, this, BLE_TASK_PRIORITY, &_taskVarioGPSHandle, 1);
}

void VarioBle::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<VarioBle *>(parm)->task();
}

void VarioBle::task()
{
    while (1)
    {
        sendTrames();
        // give time to other tasks
        vTaskDelay(delayT100);
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

void VarioBle::sendTrames()
{
    if (deviceConnected)
    {

        //     // trame LXWP0
        // sendTrameLXWP0();
        // delay(50);

        // GPS sentences
        if (fc.getGpsSentenceTimestamp() > lastTimestampGpsSentence)
        {
            lastTimestampGpsSentence = fc.getGpsSentenceTimestamp();
            char sentence[100];
            strcpy(sentence, fc.getGpsSentence());

            uint8_t strSize = strlen(sentence);
            for (int i = 0; i < strSize - 1; i++)
            {
                uint8_t c = sentence[i];
                // Serial.write(c);
                pTxCharacteristic->setValue(&c, 1);

                pTxCharacteristic->notify();

                vTaskDelay(delayT2); // bluetooth stack will go into congestion, if too many packets are sent
            }
        }
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        vTaskDelay(delayT100 * 5);   // give the bluetooth stack the chance to get things ready
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

void VarioBle::sendTrameLXWP0()
{
    // format de la trame LXWP0
    // $LXWP0,loger_stored (Y/N), IAS(kph), baroaltitude (m), vario (m/s),,,,,,heading of plane,windcourse (deg),windspeed(kph)*checksum\r\n
    // $LXWP0,Y,22.3,1665.5,1.71,1.71,1.71,1.71,1.71,1.71,239,174,10.1

    float coeff = random(80, 100) / 100.0;

    // generate a random number between 5 and 20.5
    double speed = (rand() % 155 + 5) / 10.0;
    // generate a random number between 1000 and 1300.5
    double alti = (rand() % 305 + 1000) / 10.0;
    // generate a random number between -2 and 3.6
    double vario1 = (rand() % 56 - 20) / 10.0;
    double vario2 = vario1 * coeff;
    double vario3 = vario2 * coeff;
    double vario4 = vario3 * coeff;
    double vario5 = vario4 * coeff;
    double vario6 = vario5 * coeff;
    // generate a random number between 170 and 190
    uint16_t heading = (rand() % 20 + 170);
    double windspeed = 0;

    char line[100];
    uint8_t prefix = '*';
    uint8_t parity = 0;

    sprintf_P(line, PSTR("$LXWP0,Y,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d,%d.%02d"),
              (int)speed, abs((int)(speed * 100) % 100),
              (int)alti, abs((int)(alti * 100) % 100),
              (int)vario1, abs((int)(vario1 * 100) % 100),
              (int)vario2, abs((int)(vario2 * 100) % 100),
              (int)vario3, abs((int)(vario3 * 100) % 100),
              (int)vario4, abs((int)(vario4 * 100) % 100),
              (int)vario5, abs((int)(vario5 * 100) % 100),
              (int)vario6, abs((int)(vario6 * 100) % 100),
              heading,
              (int)windspeed, (int)(windspeed * 100) % 100);

    uint8_t strSize = strlen(line);
    for (int i = 0; i < strSize - 1; i++)
    {
        uint8_t c = line[i];
        Serial.write(c);
        pTxCharacteristic->setValue(&c, 1);

        pTxCharacteristic->notify();

        if (i > 0)
        {
            parity ^= c;
        }

        delay(2); // bluetooth stack will go into congestion, if too many packets are sent

        if (i == (strSize - 2))
        {
            Serial.write(prefix);
            Serial.write(parity);
            Serial.println();
            pTxCharacteristic->setValue(&prefix, 1);
            pTxCharacteristic->notify();
            delay(5); // bluetooth stack will go into congestion, if too many packets are sent

            char buffer[3];

            sprintf(buffer, "%02x", parity);
            c = buffer[0];
            pTxCharacteristic->setValue(&c, 1);
            pTxCharacteristic->notify();
            delay(5); // bluetooth stack will go into congestion, if too many packets are sent
            c = buffer[1];
            pTxCharacteristic->setValue(&c, 1);
            pTxCharacteristic->notify();
            delay(5); // bluetooth stack will go into congestion, if too many packets are sent

            pTxCharacteristic->setValue("\r\n");
            pTxCharacteristic->notify();
        }
    }
}

void VarioBle::deviceConnect()
{
    deviceConnected = true;
}

void VarioBle::deviceDisConnect()
{
    deviceConnected = false;
}