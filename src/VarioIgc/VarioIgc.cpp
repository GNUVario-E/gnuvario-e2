#include "VarioIgc.h"
#include <Arduino.h>
#include "VarioDebug/VarioDebug.h"
#include "VarioTool/VarioTool.h"

#define IGC_TASK_PRIORITY 8

VarioIgc::VarioIgc()
{
}

void VarioIgc::startTask()
{
    // task creation
    VARIO_IGC_DEBUG_PRINTLN("TaskVarioIgc started");
    // xTaskCreate(this->startTaskImpl, "TaskVarioIgc", 3000, this, IGC_TASK_PRIORITY, &_taskVarioIgcHandle);

    xTaskCreatePinnedToCore(this->startTaskImpl, "TaskVarioIgc", 3000, this, IGC_TASK_PRIORITY, &_taskVarioIgcHandle,1);
}

void VarioIgc::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<VarioIgc *>(parm)->task();
}

void VarioIgc::task()
{
    while (1)
    {
        if (lastTrameSecond != fc.getGpsTimeSecond())
        {
            uint32_t lTimeout = (millis() - 800);
            if (fc.getGpsIsFixed() && fc.getGpsIsFixedTimestamp() > lTimeout && fc.getGpsTimeTimestamp() > lTimeout && fc.getGpsLocTimestamp() > lTimeout)
            {
                addBLine(fc.getGpsTimeHourUTC(), fc.getGpsTimeMinute(), fc.getGpsTimeSecond(), fc.getGpsLat(), fc.getGpsLon(), fc.getVarioAlti(), fc.getGpsAltiMeters());
                lastTrameSecond = fc.getGpsTimeSecond();
            }
        }
        // give time to other tasks
        vTaskDelay(delayT200);
    }
}

bool VarioIgc::createNewIgcFile(const char *pilot, const char *glider, uint8_t day, uint8_t month, uint8_t year, int8_t timezone)
{
    igcFile = new VarioIgcFile();
    if (igcFile->createNewIgcFile(day, month, year))
    {
        if (igcFile->getIsCreated())
        {
            VARIO_IGC_DEBUG_PRINTLN("File created");
            isReadyToWrite = true;
            initHeaders(pilot, glider, day, month, year, timezone);

            return true;
        };
    }

    return false;
}

void VarioIgc::initHeaders(const char *pilot, const char *glider, uint8_t day, uint8_t month, uint8_t year, int8_t timezone)
{
    if (!isReadyToWrite)
    {
        return;
    }
    char line[80];
    const char *A = "A%s %s";
    const char *DTE = "HFDTE%02u%02u%02u";
    const char *PLT = "HFPLTPILOTINCHARGE:%s";
    const char *GTY = "HFGTYGLIDERTYPE:%s";
    const char *DTM = "HFDTMGPSDATUM:WGS84";
    const char *FTY = "HFFTYFRTYPE:GNUVarioE2";
    const char *TZN = "HFTZNTIMEZONE:%+d";

    // A record
    sprintf_P(line, A, "XXX", (char *)VarioTool::getDeviceID().c_str());
    igcFile->writeline(line);

    // H F DTE DATE
    sprintf_P(line, DTE, day, month, year);
    igcFile->writeline(line);

    // H F PLT PILOT IN CHARGE
    sprintf_P(line, PLT, pilot);
    igcFile->writeline(line);

    // H F GTY GLIDER TYPE
    sprintf_P(line, GTY, glider);
    igcFile->writeline(line);

    // H F DTM GPS DATUM
    sprintf_P(line, DTM);
    igcFile->writeline(line);

    // H F FTY FR TYPE
    sprintf_P(line, FTY);
    igcFile->writeline(line);

    // H F TZN
    sprintf_P(line, TZN, timezone);
    igcFile->writeline(line);
}

void VarioIgc::addBLine(uint8_t hour, uint8_t minute, uint8_t second, double locLat, double locLon, double altiVario, double altiGps)
{
    // B 155954 4445039N    00348762E   A 00878 00976
    // B HHMMSS DDMMmmmN/S  DDDMMmmmE/W A PPPPP GGGGG

    if (!isReadyToWrite)
    {
        return;
    }
    VARIO_IGC_DEBUG_PRINTLN("addBLineled");
    uint16_t degLat = abs(locLat);
    float minutesRemainderLat = abs(abs(locLat) - degLat) * 60;
    uint16_t degLon = abs(locLon);
    float minutesRemainderLon = abs(abs(locLon) - degLon) * 60;
    char line[80];
    const char *trame = "B%02d%02d%02d%02d%05d%s%03d%05d%sA%05d%05d";

    sprintf_P(line, trame, hour, minute, second, degLat, (uint32_t)(minutesRemainderLat * 1000), (locLat > 0) ? "N" : "S", degLon, (uint32_t)(minutesRemainderLon * 1000), (locLon > 0) ? "E" : "W", (int)floor(altiVario), (int)floor(altiGps));
    // Serial.println(line);
    if (!igcFile->writeline(line))
    {
        VARIO_IGC_DEBUG_PRINTLN("addBLine failed");
    }
}

void DegreesToDegMinSec(float x)
{
    int deg = x;
    float minutesRemainder = abs(x - deg) * 60;
    int arcMinutes = minutesRemainder;
    float arcSeconds = (minutesRemainder - arcMinutes) * 60;
    // Serial.print(deg);
    // Serial.print("*");
    // Serial.print(arcMinutes);
    // Serial.print("'");
    // Serial.print(arcSeconds, 4);
    // Serial.print('"');
    // Serial.println();
}