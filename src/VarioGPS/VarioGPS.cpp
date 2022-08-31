#include "VarioGPS.h"
#include "VarioDebug/VarioDebug.h"
#include <esp32-hal-uart.h>
#include "VarioData.h"
#include "VarioTool/VarioTool.h"

#define GPS_TASK_PRIORITY 9

VarioGPS::VarioGPS()
{
    aglManager = new AglManager();
}

void VarioGPS::init()
{
    Serial2.begin(9600, SERIAL_8N1, NMEA_RX_PIN, NMEA_TX_PIN);
    VARIO_GPS_DEBUG_PRINTLN("Serial Txd is on pin: " + String(TX));
    VARIO_GPS_DEBUG_PRINTLN("Serial Rxd is on pin: " + String(RX));

    aglManager->init();
}

void VarioGPS::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskVarioGPS started");
    xTaskCreate(this->startTaskImpl, "TaskVarioGPS", 4096, this, GPS_TASK_PRIORITY, &_taskVarioGPSHandle);
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
            VARIO_GPS_DEBUG_PRINTLN(F("No GPS detected: check wiring."));
            while (true)
            {
                vTaskDelay(delayT500);
            }
        }

        // give time to other tasks
        vTaskDelay(delayT500);
    }
}

void VarioGPS::displayInfo()
{
    VARIO_GPS_DEBUG_PRINT(F("Location: "));

    if (gps.location.isValid())
    {
        fc.gps.locTimestamp = millis() - gps.location.age();
        VARIO_GPS_DEBUG_PRINT("age");
        VARIO_GPS_DEBUG_PRINTLN(fc.gps.locTimestamp);
        if (gps.location.isUpdated())
        {
            fc.gps.locLat = gps.location.lat();
            fc.gps.locLon = gps.location.lng();
            aglManager->setLatitude(fc.gps.locLat);
            aglManager->setLongitude(fc.gps.locLon);
            VARIO_GPS_DEBUG_PRINT("latitudelongitude: ");
            VARIO_GPS_DEBUG_PRINT2(gps.location.lat(), 6);
            VARIO_GPS_DEBUG_PRINT(",");
            VARIO_GPS_DEBUG_PRINT2(gps.location.lng(), 6);
            VARIO_GPS_DEBUG_PRINT("\n");
        }
    }
    else
    {
        fc.gps.locTimestamp = 0;
        fc.gps.locLat = -999.0;
        fc.gps.locLon = -999.0;
        VARIO_GPS_DEBUG_PRINTLN(F("INVALID"));
    }

    VARIO_GPS_DEBUG_PRINT(F("  Date/Time: "));

    if ((gps.satellites.value() > 0 && gps.date.isValid()))
    {
        fc.gps.dateTimestamp = millis() - gps.date.age();
        if (gps.date.isUpdated())
        {
            fc.gps.dateDay = gps.date.day();
            fc.gps.dateMonth = gps.date.month();
            fc.gps.dateYear = gps.date.year();
            VARIO_GPS_DEBUG_PRINT("date:");
            VARIO_GPS_DEBUG_PRINT(fc.gps.dateMonth);
            VARIO_GPS_DEBUG_PRINT(F("/"));
            VARIO_GPS_DEBUG_PRINT(fc.gps.dateDay);
            VARIO_GPS_DEBUG_PRINT(F("/"));
            VARIO_GPS_DEBUG_PRINTLN(fc.gps.dateYear);
        }
    }
    else
    {
        fc.gps.dateTimestamp = 0;
        VARIO_GPS_DEBUG_PRINTLN(F("INVALID"));
    }

    fc.gps.satellitesCount = gps.satellites.value();
    fc.gps.satellitesTimestamp = millis() - gps.satellites.age();
    VARIO_GPS_DEBUG_PRINTLN("\n");
    VARIO_GPS_DEBUG_PRINT("Nb satellites: ");
    VARIO_GPS_DEBUG_PRINTLN(fc.gps.satellitesCount);

    fc.gps.hdopTimestamp = millis() - gps.hdop.age();
    if (gps.hdop.isValid())
    {
        if (gps.hdop.isUpdated())
        {
            fc.gps.hdop = gps.hdop.hdop();
            VARIO_GPS_DEBUG_PRINT("hdop: ");
            VARIO_GPS_DEBUG_PRINTLN(gps.hdop.hdop());
        }
    }

    if ((gps.satellites.value() > 0) && gps.speed.isValid())
    {
        fc.gps.kmphTimestamp = millis() - gps.speed.age();
        VARIO_GPS_DEBUG_PRINT("kmphAge:");
        VARIO_GPS_DEBUG_PRINTLN(fc.gps.kmphTimestamp);
        if (gps.speed.isUpdated())
        {
            fc.gps.kmph = gps.speed.kmph();
            VARIO_GPS_DEBUG_PRINT("speed: ");
            VARIO_GPS_DEBUG_PRINTLN(gps.speed.kmph());
        }
    }
    else
    {
        fc.gps.kmphTimestamp = 0;
    }

    if ((gps.satellites.value() > 0) && gps.time.isValid())
    {
        fc.gps.timeTimestamp = millis() - gps.time.age();
        if (gps.time.isUpdated())
        {
            fc.gps.timeHour = gps.time.hour() + varioData.getParam(PARAM_TIME_ZONE)->getValueInt8();
            fc.gps.timeMinute = gps.time.minute();
            fc.gps.timeSecond = gps.time.second();

            VARIO_GPS_DEBUG_PRINT("hour:");
            if (gps.time.hour() < 10)
                VARIO_GPS_DEBUG_PRINT(F("0"));
            VARIO_GPS_DEBUG_PRINT(fc.gps.timeHour);
            VARIO_GPS_DEBUG_PRINT(F(":"));
            if (gps.time.minute() < 10)
                VARIO_GPS_DEBUG_PRINT(F("0"));
            VARIO_GPS_DEBUG_PRINT(fc.gps.timeMinute);
            VARIO_GPS_DEBUG_PRINT(F(":"));
            if (gps.time.second() < 10)
                VARIO_GPS_DEBUG_PRINT(F("0"));
            VARIO_GPS_DEBUG_PRINTLN(fc.gps.timeSecond);
            // VARIO_GPS_DEBUG_PRINT(F("."));
            // if (gps.time.centisecond() < 10)
            //     VARIO_GPS_DEBUG_PRINT(F("0"));
            // VARIO_GPS_DEBUG_PRINTLN(gps.time.centisecond());
        }
    }
    else
    {
        fc.gps.timeTimestamp = 0;
        VARIO_GPS_DEBUG_PRINTLN(F("INVALID"));
    }

    fc.gps.headingDegTimestamp = millis() - gps.course.age();
    if (gps.course.isValid())
    {
        if (gps.course.isUpdated())
        {
            fc.gps.headingDeg = gps.course.deg();
            VARIO_GPS_DEBUG_PRINT("course:");
            VARIO_GPS_DEBUG_PRINTLN(fc.gps.headingDeg);
            VarioTool::bearingToOrdinal2c(fc.gps.headingTxt, fc.gps.headingDeg);
        }
    }

    if (gps.altitude.isValid())
    {
        fc.gps.altiMetersTimestamp = millis() - gps.altitude.age();
        if (gps.altitude.isUpdated())
        {
            fc.gps.altiMeters = gps.altitude.meters();
            aglManager->setAltiGps(fc.gps.altiMeters);
            aglManager->setAlti(fc.vario.alti);

            VARIO_GPS_DEBUG_PRINT("altitude vario:");
            VARIO_GPS_DEBUG_PRINTLN(fc.vario.alti);
            VARIO_GPS_DEBUG_PRINT("altitude gps:");
            VARIO_GPS_DEBUG_PRINTLN(fc.gps.altiMeters);
            fc.agl.agl = aglManager->getAgl();
            fc.agl.aglTimestamp = millis();
            fc.agl.groundLvl = aglManager->getGroundLevel();
            VARIO_GPS_DEBUG_PRINT("altitude agl:");
            VARIO_GPS_DEBUG_PRINTLN(fc.agl.agl);
            VARIO_GPS_DEBUG_PRINT("altitude groundlevel:");
            VARIO_GPS_DEBUG_PRINTLN(fc.agl.groundLvl);
            VARIO_GPS_DEBUG_PRINT("altitude height:");
            VARIO_GPS_DEBUG_PRINTLN(aglManager->getHeight());
        }
    }
    else
    {
        fc.gps.altiMetersTimestamp = 0;
        fc.agl.aglTimestamp = 0;
        VARIO_GPS_DEBUG_PRINT("altiMetersAge:");
        VARIO_GPS_DEBUG_PRINTLN(fc.gps.altiMetersTimestamp);
    }

    VARIO_GPS_DEBUG_PRINTLN();
}