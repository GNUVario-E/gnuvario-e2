#include "VarioTool.h"
#include <Arduino.h>
#include "VarioDebug/VarioDebug.h"
#include <SD.h>

#define R_M 6378137 // distance moyenne de la Terre à l'équateur en m

void VarioTool::appendChar(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

boolean VarioTool::readLines(File *file, char *line)
{
    int bytesRead = file->readBytesUntil('\n', line, RL_MAX_CHARS);
    line[bytesRead] = '\0';

    if (bytesRead > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void VarioTool::deleteRecursive(const char *path)
{
    VARIO_SDCARD_DEBUG_PRINT("deleteRecursive path : <");
    VARIO_SDCARD_DEBUG_PRINT(path);
    VARIO_SDCARD_DEBUG_PRINTLN(">");

    File fileSD;

    fileSD = SD.open(path, FILE_READ);

    if (!fileSD.isDirectory())
    {
        fileSD.close();
        SD.remove(path);
        return;
    }

    fileSD.rewindDirectory();

    while (true)
    {
        File entry;
        if (!(entry = fileSD.openNextFile(FILE_READ)))
        {
            break;
        }

        // const char *const entryPath = entry.name();
        String entryPath = entry.name();
        if (entry.isDirectory())
        {
            entry.close();
            deleteRecursive(entryPath.c_str());
        }
        else
        {
            entry.close();
            SD.remove(entryPath);
        }
    }

    SD.rmdir(path);
    fileSD.close();
}

void VarioTool::bearingToOrdinal2c(char *bearingTxt, float bearing)
{

    if (bearing >= 0.0 && bearing < 22.5)
    {
        sprintf(bearingTxt, "N");
    }
    else if (bearing >= 22.5 && bearing < 67.5)
    {
        sprintf(bearingTxt, "NE");
    }
    else if (bearing >= 67.5 && bearing < 112.5)
    {
        sprintf(bearingTxt, "E");
    }
    else if (bearing >= 112.5 && bearing < 157.5)
    {
        sprintf(bearingTxt, "SE");
    }
    else if (bearing >= 157.5 && bearing < 202.5)
    {
        sprintf(bearingTxt, "S");
    }
    else if (bearing >= 202.5 && bearing < 247.5)
    {
        sprintf(bearingTxt, "SW");
    }
    else if (bearing >= 247.5 && bearing < 292.5)
    {
        sprintf(bearingTxt, "W");
    }
    else if (bearing >= 292.5 && bearing < 337.5)
    {
        sprintf(bearingTxt, "NW");
    }
    else if (bearing >= 337.5 && bearing < 360)
    {
        sprintf(bearingTxt, "N");
    }
    else
    {
        sprintf(bearingTxt, "N");
    }
}

String VarioTool::getDeviceID()
{
    char deviceid[21];
    uint64_t chipid;
    chipid = ESP.getEfuseMac();
    sprintf(deviceid, "%" PRIu64, chipid);
    String thisID(deviceid);
    return thisID;
}

double VarioTool::convertIGCDDMMmmmToDecimal(int32_t valeur)
{
    double decimal = 0.0;
    int32_t deg = valeur / 100000;
    int32_t min = valeur % 100000;
    decimal = deg + (double)min / 60000.0;

    return decimal;
}

double VarioTool::convertGPSnmeaToDecimalDegrees(double nmea)
{
    int degrees = nmea / 100;
    double minutes = nmea - (degrees * 100);
    return degrees + minutes / 60;
}

/**
 * @brief Calculate the distance between two GPS points
 *
 * @param lat1Rad en radians
 * @param lon1Rad  en radians
 * @param lat2Rad  en radians
 * @param lon2Rad  en radians
 * @return double
 */
double VarioTool::calculateDistance(double lat1Rad, double lon1Rad, double lat2Rad, double lon2Rad)
{
    // Appliquer la formule Haversine
    double dlon = lon2Rad - lon1Rad;
    double dlat = lat2Rad - lat1Rad;
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1Rad) * cos(lat2Rad) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    double distance = R_M * c;

    return distance;
}