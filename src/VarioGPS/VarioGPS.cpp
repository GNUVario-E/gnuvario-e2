#include "VarioGPS.h"
#include "VarioDebug/VarioDebug.h"
#include <esp32-hal-uart.h>
#include "VarioTool/VarioTool.h"
#include "VarioBle/VarioBle.h"
#include "tasksBitsMask.h"

#define GPS_TASK_PRIORITY 8

VarioGPS::VarioGPS()
{
    fc.setTzn(params->P_TIME_ZONE->getValue());
}

void VarioGPS::init()
{
    Serial2.begin(9600, SERIAL_8N1, NMEA_RX_PIN, NMEA_TX_PIN);

    VARIO_GPS_DEBUG_PRINTLN("Serial Rxd is on pin: " + String(RX));
    VARIO_GPS_DEBUG_PRINTLN("Serial Txd is on pin: " + String(TX));
}

void VarioGPS::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskVarioGPS started");
    // xTaskCreate(this->startTaskImpl, "TaskVarioGPS", 4096, this, GPS_TASK_PRIORITY, &_taskVarioGPSHandle);
    xTaskCreatePinnedToCore(this->startTaskImpl, "TaskVarioGPS", 3000, this, GPS_TASK_PRIORITY, &_taskVarioGPSHandle, 1);
}

void VarioGPS::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<VarioGPS *>(parm)->task();
}

void VarioGPS::task()
{
    int c;
    lastCharProcessedTime = millis();
    while (1)
    {
        // This sketch displays information every time a new sentence is correctly encoded.
        while (Serial2.available() > 0)
        {
            c = Serial2.read();
            // if (c == '$')
            // {
            //     Serial.println("\n");
            // }
            // Serial.write(c);

            sentenceBuffer.push(c);
            if (gps.encode(c))
            {
                // we've got a new valid sentence
                // VARIO_GPS_DEBUG_PRINTLN("New valid sentence");
                // VARIO_GPS_DEBUG_PRINTLN("---------------------------------------------");
                // for (decltype(sentenceBuffer)::index_t i = 0; i < sentenceBuffer.size(); i++)
                // {
                //     VARIO_GPS_DEBUG_PRINT(sentenceBuffer[i]);
                // }
                // VARIO_GPS_DEBUG_PRINTLN("");
                // VARIO_GPS_DEBUG_PRINTLN("---------------------------------------------");

                sendSentenceToFC();

                // vidage du buffer
                sentenceBuffer.clear();
                vTaskDelay(delayT10);
                displayInfo();
            }

            lastCharProcessedTime = millis();
        }

        if ((millis() - lastCharProcessedTime) > 5000 && gps.charsProcessed() < 10)
        {
            VARIO_GPS_DEBUG_PRINTLN("No GPS detected: check wiring.");
            while (true)
            {

                vTaskDelay(delayT500);
            }
        }

        // give time to other tasks
        vTaskDelay(delayT10);

        if (uxTaskGetStackHighWaterMark(NULL) < minRemainingStackSize)
        {
            minRemainingStackSize = uxTaskGetStackHighWaterMark(NULL);
            Serial.printf("task GPS: %d", minRemainingStackSize);
        }
    }
}

void VarioGPS::displayInfo()
{
    VARIO_GPS_DEBUG_PRINT(F("Location: "));

    if (gps.location.isValid())
    {
        VARIO_GPS_DEBUG_PRINT("age");
        VARIO_GPS_DEBUG_PRINTLN(gps.location.age());
        if (gps.location.isUpdated())
        {
            fc.setGpsLocation(gps.location.lat(), gps.location.lng(), (millis() - gps.location.age()));

            VARIO_GPS_DEBUG_PRINT("latitudelongitude: ");
            VARIO_GPS_DEBUG_PRINT2(gps.location.lat(), 6);
            VARIO_GPS_DEBUG_PRINT(",");
            VARIO_GPS_DEBUG_PRINT2(gps.location.lng(), 6);
            VARIO_GPS_DEBUG_PRINT("\n");
        }
        else
        {
            fc.setGpsLocTimestamp(millis() - gps.location.age());
        }
    }
    else
    {
        fc.setGpsLocTimestamp(0);
        VARIO_GPS_DEBUG_PRINTLN(F("INVALID"));
    }

    VARIO_GPS_DEBUG_PRINT(F("  Date/Time: "));

    if ((gps.satellites.value() > 0 && gps.date.isValid()))
    {
        if (gps.date.isUpdated())
        {
            fc.setGpsDate(gps.date.day(), gps.date.month(), (gps.date.year() % 2000), millis() - gps.date.age());
            VARIO_GPS_DEBUG_PRINT("date:");
            VARIO_GPS_DEBUG_PRINT(gps.date.month());
            VARIO_GPS_DEBUG_PRINT(F("/"));
            VARIO_GPS_DEBUG_PRINT(gps.date.day());
            VARIO_GPS_DEBUG_PRINT(F("/"));
            VARIO_GPS_DEBUG_PRINTLN(gps.date.year());
        }
        else
        {
            fc.setGpsDateTimestamp(millis() - gps.date.age());
        }
    }
    else
    {
        fc.setGpsDateTimestamp(0);
        VARIO_GPS_DEBUG_PRINTLN(F("INVALID"));
    }

    fc.setGpsSatellitesCount(gps.satellites.value(), (millis() - gps.satellites.age()));

    VARIO_GPS_DEBUG_PRINTLN("\n");
    VARIO_GPS_DEBUG_PRINT("Nb satellites: ");

    if (gps.hdop.isValid())
    {
        if (gps.hdop.isUpdated())
        {
            fc.setGpsHdop(gps.hdop.hdop(), (millis() - gps.hdop.age()));
            VARIO_GPS_DEBUG_PRINT("hdop: ");
            VARIO_GPS_DEBUG_PRINTLN(gps.hdop.hdop());
        }
        else
        {
            fc.setGpsHdopTimestamp(millis() - gps.hdop.age());
        }
    }
    else
    {
        fc.setGpsHdop(999, 0);
    }

    if ((gps.satellites.value() > 0) && gps.speed.isValid())
    {
        VARIO_GPS_DEBUG_PRINT("kmphAge:");
        VARIO_GPS_DEBUG_PRINTLN();
        if (gps.speed.isUpdated())
        {
            fc.setGpsKmph(gps.speed.kmph(), (millis() - gps.speed.age()));
            VARIO_GPS_DEBUG_PRINT("speed: ");
            VARIO_GPS_DEBUG_PRINTLN(gps.speed.kmph());
        }
        else
        {
            fc.setGpsKmphTimestamp(millis() - gps.speed.age());
        }
    }
    else
    {
        fc.setGpsKmphTimestamp(0);
    }

    if ((gps.satellites.value() > 0) && gps.time.isValid())
    {
        if (gps.time.isUpdated())
        {
            fc.setGpsTimeUTC(gps.time.hour(), gps.time.minute(), gps.time.second(), millis() - gps.time.age());

            VARIO_GPS_DEBUG_PRINT("hour:");
            if (gps.time.hour() < 10)
                VARIO_GPS_DEBUG_PRINT(F("0"));
            VARIO_GPS_DEBUG_PRINT(gps.time.hour());
            VARIO_GPS_DEBUG_PRINT(F(":"));
            if (gps.time.minute() < 10)
                VARIO_GPS_DEBUG_PRINT(F("0"));
            VARIO_GPS_DEBUG_PRINT(gps.time.minute());
            VARIO_GPS_DEBUG_PRINT(F(":"));
            if (gps.time.second() < 10)
                VARIO_GPS_DEBUG_PRINT(F("0"));
            VARIO_GPS_DEBUG_PRINTLN(gps.time.second());
            // VARIO_GPS_DEBUG_PRINT(F("."));
            // if (gps.time.centisecond() < 10)
            //     VARIO_GPS_DEBUG_PRINT(F("0"));
            // VARIO_GPS_DEBUG_PRINTLN(gps.time.centisecond());
        }
        else
        {
            fc.setGpsTimeTimestamp(millis() - gps.time.age());
        }
    }
    else
    {
        fc.setGpsTimeUTC(0, 0, 0, 0);
        VARIO_GPS_DEBUG_PRINTLN(F("INVALID"));
    }

    if (gps.course.isValid())
    {
        if (gps.course.isUpdated())
        {
            char headingStr[3];
            VarioTool::bearingToOrdinal2c(headingStr, gps.course.deg());
            fc.setGpsHeading(gps.course.deg(), headingStr, millis() - gps.course.age());

            VARIO_GPS_DEBUG_PRINT("course:");
            VARIO_GPS_DEBUG_PRINTLN(gps.course.deg());
        }
        else
        {
            fc.setGpsHeadingTimestamp(millis() - gps.course.age());
        }
    }
    else
    {
        fc.setGpsHeading(0, "", 0);
    }

    if (gps.altitude.isValid())
    {
        if (gps.altitude.isUpdated())
        {
            fc.setGpsAltiMeters(gps.altitude.meters(), millis() - gps.altitude.age());

            VARIO_GPS_DEBUG_PRINT("altitude gps:");
            VARIO_GPS_DEBUG_PRINTLN(fc.getGpsAltiMeters());
        }
        else
        {
            fc.setGpsAltiMetersTimestamp(millis() - gps.altitude.age());
        }
    }
    else
    {
        fc.setGpsAltiMetersTimestamp(0);
        VARIO_GPS_DEBUG_PRINT("altiMetersAge:");
        VARIO_GPS_DEBUG_PRINTLN(fc.getGpsAltiMetersTimestamp());
    }

    VARIO_GPS_DEBUG_PRINTLN();
}

void VarioGPS::sendSentenceToFC()
{
    char c;
    int j = 0;
    bool started = false;
    char sentence[BUFFER_SIZE];
    uint16_t bufferSize = sentenceBuffer.size();

    // clear sentence buffer
    sentence[0] = '\0';

    for (uint16_t i = 0; i < bufferSize; i++)
    {
        // send sentence to FC
        c = sentenceBuffer.shift();
        if (c == '$')
        {
            started = true;
        }

        if (started)
        {
            // append c to sentence
            sentence[j] = c;
            j++;
        }
    }

    if (started)
    {
        sentence[j] = '\0';

        // which GPS sentences to store
        // $GPRMC
        // $GNRMC
        // $GPGGA
        // $GNGGA
        if ((strncmp(sentence, "$GPGGA", 6) == 0) || (strncmp(sentence, "$GNGGA", 6) == 0) || (strncmp(sentence, "$GPRMC", 6) == 0) || (strncmp(sentence, "$GNRMC", 6) == 0))
        {
            fc.setGpsSentence(sentence, millis());
            // Serial.println(sentence);
            if (params->P_BT_ENABLE)
            {
                if (VarioBle::_taskVarioBleHandle != NULL)
                {

                    xTaskNotify(VarioBle::_taskVarioBleHandle, BLE_GPS_SENTENCE_BIT, eSetBits);
                }
            }
        }
    }
}

extern TaskHandle_t _taskVarioBleHandle;