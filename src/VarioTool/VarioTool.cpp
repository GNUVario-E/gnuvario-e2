#include "VarioTool.h"
#include <Arduino.h>
#include "VarioDebug/VarioDebug.h"
#include <SD.h>

StaticJsonDocument<4096> VarioTool::jsonDoc;

void VarioTool::appendChar(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

boolean VarioTool::readLines(File *file, char *line)
{
    char c = file->read();
    uint8_t cIndex = 0;
    while (c != '\n' && cIndex < RL_MAX_CHARS)
    {
        line[cIndex] = c;
        line[cIndex + 1] = '\0';
        cIndex++;
        if (!file->available())
            break;
        c = file->read();
    }

    if (cIndex > 0)
        return true;
    else
        return false;
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