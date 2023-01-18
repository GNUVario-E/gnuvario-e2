#include "VarioIgcParser.h"
#include <Arduino.h>
#include <MD5Builder.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "VarioDebug/VarioDebug.h"

QueueHandle_t xQueueParse;

VarioIgcParser::VarioIgcParser(String path)
{
    VARIO_MEMORY_DEBUG_PRINTLN("Free heap VarioIgcParser constructor");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    filePath = path;
}

VarioIgcParser::~VarioIgcParser()
{
    VARIO_MEMORY_DEBUG_PRINTLN("Free heap VarioIgcParser destructor");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());
}

boolean VarioIgcParser::parseFile()
{
    const char IGCHeaderDATE[] PROGMEM = "HFDTE";
    const char IGCHeaderDATENEW[] PROGMEM = "HFDTEDATE:";
    const char IGCHeaderPILOT[] PROGMEM = "HFPLTPILOTINCHARGE:";
    const char IGCHeaderGLIDER[] PROGMEM = "HFGTYGLIDERTYPE:";

    // TickType_t delay = (1) / portTICK_PERIOD_MS;
    File dataFile;
    String buffer;
    char tmpBuffer[100];
    uint8_t tmpBufferPos = 0;
    boolean startHeightSet = false;
    boolean startFlightTimeSet = false;

    uint32_t dataToSend = 1;

    myIgcData.minHeight = 9999;
    myIgcData.maxHeight = -9999;

    isParsed = false;

    if (!(dataFile = SD.open((char *)filePath.c_str(), FILE_READ)))
    {
        return false;
    }
    String tmpFullName = dataFile.name();
    myIgcData.filename = tmpFullName.substring(tmpFullName.lastIndexOf("/") + 1);

    MD5Builder md5b;
    md5b.begin();
    uint16_t size = dataFile.size();
    while (size > 0)
    {
        uint16_t curSize = min(size, (uint16_t)4096);
        md5b.addStream(dataFile, curSize);
        size -= curSize;

        xQueueSend(xQueueParse, &dataToSend, 0);
        // vTaskDelay(delay);
        VARIO_WIFI_DEBUG_PRINT(".");
    }

    md5b.calculate();
    myIgcData.md5 = md5b.toString();

    VARIO_WIFI_DEBUG_PRINTLN("md5 : " + myIgcData.md5);

    // on ferme pour libérer la mémoire
    dataFile.close();
    if (!(dataFile = SD.open((char *)filePath.c_str(), FILE_READ)))
    {
        return false;
    }

    // retour au debut du fichier
    dataFile.seek(0);

    while (dataFile.available())
    {
        xQueueSend(xQueueParse, &dataToSend, 0);

        VARIO_WIFI_DEBUG_PRINT("-");

        tmpBufferPos = 0;
        while (dataFile.available() && dataFile.peek() != '\n' && tmpBufferPos < 99) // note how this also prevents the buffer from overflowing (49 max to leave space for '\0'!)
        {
            tmpBuffer[tmpBufferPos] = dataFile.read();
            tmpBufferPos++;
        }
        if (tmpBufferPos > 0)
        {
            // on lit aussi le \n qui traine
            dataFile.read();

            tmpBuffer[tmpBufferPos] = '\0';
            buffer = String(tmpBuffer);
            buffer.trim();

            VARIO_WIFI_DEBUG_PRINT(".");

            if (buffer.startsWith(IGCHeaderDATENEW))
            {
                // date de la trace
                myIgcData.flightDate = buffer.substring(10);
                myIgcData.flightDate.trim();
                // inversion pour format americain
                myIgcData.flightDate = "20" + myIgcData.flightDate.substring(4, 6) + "-" + myIgcData.flightDate.substring(2, 4) + "-" + myIgcData.flightDate.substring(0, 2);

                VARIO_WIFI_DEBUG_PRINT("flightDate : ");
                VARIO_WIFI_DEBUG_PRINTLN(myIgcData.flightDate);
            }
            else if (buffer.startsWith(IGCHeaderDATE))
            {
                // date de la trace
                myIgcData.flightDate = buffer.substring(5);
                myIgcData.flightDate.trim();
                // inversion pour format americain
                myIgcData.flightDate = "20" + myIgcData.flightDate.substring(4, 6) + "-" + myIgcData.flightDate.substring(2, 4) + "-" + myIgcData.flightDate.substring(0, 2);

                VARIO_WIFI_DEBUG_PRINT("flightDate : ");
                VARIO_WIFI_DEBUG_PRINTLN(myIgcData.flightDate);
            }
            else if (buffer.startsWith(IGCHeaderPILOT))
            {
                // nom du pilote
                myIgcData.pilot = buffer.substring(19);
                myIgcData.pilot.trim();

                VARIO_WIFI_DEBUG_PRINT("pilot : ");
                VARIO_WIFI_DEBUG_PRINTLN(myIgcData.pilot);
            }
            else if (buffer.startsWith(IGCHeaderGLIDER))
            {
                // nom de la voile
                myIgcData.wing = buffer.substring(16);
                myIgcData.wing.trim();

                VARIO_WIFI_DEBUG_PRINT("wing : ");
                VARIO_WIFI_DEBUG_PRINTLN(myIgcData.wing);
            }
            else if (buffer.startsWith("B"))
            {
                // trame trace
                //  B1243314503488N00351234EA0088400927
                //  B 12 43 31 4503488N 00351234E A 00884 00927
                // lat lon DDMMmmmN  DDDMMmmmE
                String hms = buffer.substring(1, 7);
                hms = hms.substring(0, 2) + ":" + hms.substring(2, 4) + ":" + hms.substring(4, 6);
                int16_t nPos = buffer.indexOf("N");
                int16_t ePos = buffer.indexOf("E");
                uint8_t aPos = buffer.indexOf("A");
                String lat = "";
                String lon = "";
                if (nPos != -1 && ePos != -1)
                {
                    lat = buffer.substring(7, nPos);
                    lon = buffer.substring(nPos + 1, ePos);
                }
                int16_t height = buffer.substring(aPos + 1, aPos + 6).toInt();

                if (!startHeightSet)
                {
                    startHeightSet = true;
                    myIgcData.startHeight = height;
                    if (nPos != -1 && ePos != -1)
                    {
                        myIgcData.startLat = lat.substring(0, 2).toDouble() + lat.substring(3, 8).toDouble() / 60000;
                        myIgcData.startLon = lon.substring(0, 3).toDouble() + lon.substring(3, 8).toDouble() / 60000;
                    }
                }
                // correction de l'heure en fonction du param de timezone
                // potentiellement faux si mal réglé
                correctTimeZone(hms);
                if (!startFlightTimeSet)
                {
                    startFlightTimeSet = true;
                    myIgcData.startFlightTime = hms;
                }
                myIgcData.endFlightTime = hms;
                myIgcData.minHeight = min(height, myIgcData.minHeight);
                myIgcData.maxHeight = max(height, myIgcData.maxHeight);
                myIgcData.endHeight = height;
                if (nPos != -1 && ePos != -1)
                {
                    myIgcData.endLat = lat.substring(0, 2).toDouble() + lat.substring(3, 8).toDouble() / 60000;
                    myIgcData.endLon = lon.substring(0, 3).toDouble() + lon.substring(3, 8).toDouble() / 60000;
                }
            }
        }
    }

    dataFile.close();

    VARIO_WIFI_DEBUG_PRINT("startFlightTime : ");
    VARIO_WIFI_DEBUG_PRINTLN(myIgcData.startFlightTime);
    VARIO_WIFI_DEBUG_PRINT("endFlightTime : ");
    VARIO_WIFI_DEBUG_PRINTLN(myIgcData.endFlightTime);
    VARIO_WIFI_DEBUG_PRINT("startHeight : ");
    VARIO_WIFI_DEBUG_PRINTLN(myIgcData.startHeight);
    VARIO_WIFI_DEBUG_PRINT("minHeight : ");
    VARIO_WIFI_DEBUG_PRINTLN(myIgcData.minHeight);
    VARIO_WIFI_DEBUG_PRINT("maxHeight : ");
    VARIO_WIFI_DEBUG_PRINTLN(myIgcData.maxHeight);
    VARIO_WIFI_DEBUG_PRINT("endHeight : ");
    VARIO_WIFI_DEBUG_PRINTLN(myIgcData.endHeight);

    isParsed = true;

    return true;
}

igcdata VarioIgcParser::getIgcdata()
{
    return myIgcData;
}

void VarioIgcParser::correctTimeZone(String &hms)
{
    // hms = "18:57:12"

    int8_t hour = atoi((char *)hms.substring(0, 2).c_str());
    hour += params->P_TIME_ZONE->getValue();
    if (hour < 0)
    {
        hour += 24;
    }
    if (hour >= 24)
    {
        hour -= 24;
    }
    char newHMS[9];
    sprintf(newHMS, "%02d%s", hour, (const char *)hms.substring(2, 8).c_str());
    hms = String(newHMS);
}