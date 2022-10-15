#include "VarioIgcFile.h"
#include <SD.h>
#include "VarioDebug/VarioDebug.h"

VarioIgcFile::VarioIgcFile()
{
}

bool VarioIgcFile::createNewIgcFile(uint8_t day, uint8_t month, uint8_t year)
{
    VARIO_IGC_DEBUG_PRINT("flightDir:");
    VARIO_IGC_DEBUG_PRINTLN(flightDir);

    if (!SD.exists(flightDir))
    {
        SD.mkdir(flightDir);
    }

    getNextFileName(filename, day, month, year);

    VARIO_IGC_DEBUG_PRINT("filename to create:");
    VARIO_IGC_DEBUG_PRINTLN(filename);

    String path = String(flightDir) + "/" + filename;

    if ((currentFile = SD.open(path.c_str(), FILE_WRITE)))
    {
        currentFile.close();
        isCreated = true;
        VARIO_IGC_DEBUG_PRINTLN("Open OK");

        return true;
    }
    else
    {
        VARIO_IGC_DEBUG_PRINTLN("Open KO");

        return false;
    }
}

bool VarioIgcFile::getNextFileName(char *filename, uint8_t day, uint8_t month, uint8_t year)
{
    Serial.println("getNextFileName");

    // 22090701.IGC
    char lFilename[13];
    uint16_t suffix = 0;

    VARIO_IGC_DEBUG_PRINT("day:");
    VARIO_IGC_DEBUG_PRINTLN(day);

    VARIO_IGC_DEBUG_PRINT("month:");
    VARIO_IGC_DEBUG_PRINTLN(month);

    VARIO_IGC_DEBUG_PRINT("year:");
    VARIO_IGC_DEBUG_PRINTLN(year);

    for (uint16_t i = suffix; i < 100; i++)
    {
        sprintf(lFilename, "%02d%02d%02d%02d.IGC", year, month, day, i);
        VARIO_IGC_DEBUG_PRINT("lFilename:");
        VARIO_IGC_DEBUG_PRINTLN(lFilename);
        String path = String(flightDir) + "/" + lFilename;
        if (!SD.exists(path))
        {
            num = i;
            strcpy(filename, lFilename);
            return true;
        }
    }

    return false;
}

bool VarioIgcFile::getIsCreated()
{
    return isCreated;
}

bool VarioIgcFile::writeline(char *line)
{
    if (isCreated)
    {
        uint8_t l = strlen(line);
        char lLine[l + 2];
        strcpy(lLine, line);
        strcat(lLine, "\r\n");

        char fullFilename[100] = "";
        strcat(fullFilename, flightDir);
        strcat(fullFilename, "/");
        strcat(fullFilename, filename);

        currentFile = SD.open(fullFilename, FILE_APPEND);
        if (!currentFile)
        {
            return false;
        }
        if (!currentFile.print(lLine))
        {
            currentFile.close();
            return false;
        }
        currentFile.close();

        return true;
    }
    else
    {
        return false;
    }
}
