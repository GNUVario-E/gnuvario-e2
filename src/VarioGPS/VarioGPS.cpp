#include "VarioGPS.h"
#include "VarioDebug/VarioDebug.h"
#include <esp32-hal-uart.h>

#define GPS_TASK_PRIORITY 10

void VarioGPS::init()
{
    Serial2.begin(9600, SERIAL_8N1, NMEA_RX_PIN, NMEA_TX_PIN);
    Serial.println("Serial Txd is on pin: " + String(TX));
    Serial.println("Serial Rxd is on pin: " + String(RX));
}

void VarioGPS::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskVarioGPS started");
    xTaskCreate(this->startTaskImpl, "TaskVarioGPS", 2048, this, GPS_TASK_PRIORITY, &_taskVarioGPSHandle);
}

void VarioGPS::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<VarioGPS *>(parm)->task();
}

void VarioGPS::task()
{
    startTaskTime = millis();
    while (1)
    {
        // This sketch displays information every time a new sentence is correctly encoded.
        while (Serial2.available() > 0)
        {
            if (gps.encode(Serial2.read()))
            {
                displayInfo();
            }
        }

        if ((millis() - startTaskTime) > 5000 && gps.charsProcessed() < 10)
        {
            Serial.println(F("No GPS detected: check wiring."));
            while (true)
            {
                vTaskDelay(delayT100);
            }
        }

        // give time to other tasks
        vTaskDelay(delayT100);
    }
}

void VarioGPS::displayInfo()
{
    Serial.print(F("Location: "));
    if (gps.location.isValid())
    {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if ((gps.satellites.value() > 0) && gps.date.isValid())
    {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.println("\n");
    Serial.print("Nb satellites: ");
    Serial.println(gps.satellites.value());

    if ((gps.satellites.value() > 0) && gps.time.isValid())
    {
        if (gps.time.hour() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}