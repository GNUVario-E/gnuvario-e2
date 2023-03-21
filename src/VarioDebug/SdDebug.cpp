#include "SdDebug.h"

SdDebug::SdDebug()
{
}

// size_t SdDebug::printf(const char *format, ...) __attribute__((format(printf, 2, 3))) {}

void SdDebug::print(const __FlashStringHelper *)
{
}

void SdDebug::print(const String &s)
{
    toSd((char *)s.c_str());
}

void SdDebug::print(const char c[])
{
    toSd((char *)c);
}

void SdDebug::print(char c)
{
    toSd((char *)String(c).c_str());
}

void SdDebug::print(unsigned char c, int)
{
    toSd((char *)String(c).c_str());
}

void SdDebug::print(int i, int)
{
    toSd((char *)String(i).c_str());
}

void SdDebug::print(unsigned int i, int)
{
    toSd((char *)String(i).c_str());
}

void SdDebug::print(long l, int)
{
    toSd((char *)String(l).c_str());
}

void SdDebug::print(unsigned long l, int)
{
    toSd((char *)String(l).c_str());
}

void SdDebug::print(long long l, int)
{
    toSd((char *)String(l).c_str());
}

void SdDebug::print(unsigned long long l, int)
{
    toSd((char *)String(l).c_str());
}

void SdDebug::print(double d, int)
{
    toSd((char *)String(d).c_str());
}

void SdDebug::print(const Printable &)
{
}

void SdDebug::print(struct tm *timeinfo, const char *format)
{
}

void SdDebug::println(const __FlashStringHelper *)
{
}

void SdDebug::println(const String &s)
{
    toSd((char *)s.c_str(), true);
}

void SdDebug::println(const char c[])
{
    toSd((char *)String(c).c_str(), true);
}

void SdDebug::println(char c)
{
    toSd((char *)String(c).c_str(), true);
}

void SdDebug::println(unsigned char c, int)
{
    toSd((char *)String(c).c_str(), true);
}

void SdDebug::println(int i, int)
{
    toSd((char *)String(i).c_str(), true);
}

void SdDebug::println(unsigned int i, int)
{
    toSd((char *)String(i).c_str(), true);
}

void SdDebug::println(long l, int)
{
    toSd((char *)String(l).c_str(), true);
}

void SdDebug::println(unsigned long l, int)
{
    toSd((char *)String(l).c_str(), true);
}

void SdDebug::println(long long l, int)
{
    toSd((char *)String(l).c_str(), true);
}

void SdDebug::println(unsigned long long l, int)
{
    toSd((char *)String(l).c_str(), true);
}

void SdDebug::println(double d, int)
{
    toSd((char *)String(d).c_str(), true);
}

void SdDebug::println(const Printable &)
{
}

void SdDebug::println(struct tm *timeinfo, const char *format)
{
}

void SdDebug::println(void)
{
}

void SdDebug::init()
{
    Serial.println("SdDebug::SdDebug()");
    if (!SD.exists(logsDir))
    {
        SD.mkdir(logsDir);
    }

    if (getNextFileName(filename))
    {
        strcpy(fullPath, logsDir);
        strcat(fullPath, "/");
        strcat(fullPath, filename);
        {
            if ((currentFile = SD.open(fullPath, FILE_WRITE)))
            {
                // currentFile.close();
                isCreated = true;
                // VARIO_IGC_DEBUG_PRINTLN("Open OK");

                // return true;
            }
            else
            {
                // VARIO_IGC_DEBUG_PRINTLN("Open KO");

                // return false;
            }
        }
    }
}

void SdDebug::toSd(char *line, bool withNewLine)
{
    if (VarioSD::isStarted == true && !isCreated)
    {
        init();
    }
    if (isCreated)
    {
        uint8_t l = strlen(line);
        if (withNewLine)
        {
            l += 2;
        }
        char lLine[l];
        strcpy(lLine, line);
        if (withNewLine)
        {
            strcat(lLine, "\r\n");
        }

        if (!currentFile.print(lLine))
        {
            isCreated = false;
            currentFile.close();
        }

        currentFile.flush();
    }
}

bool SdDebug::getNextFileName(char *filename)
{
    char lFilename[12];
    uint16_t suffix = 0;

    for (uint16_t i = suffix; i < 100; i++)
    {
        sprintf(lFilename, "log_%03d.txt", i);
        // VARIO_IGC_DEBUG_PRINT("lFilename:");
        // VARIO_IGC_DEBUG_PRINTLN(lFilename);

        strcpy(fullPath, logsDir);
        strcat(fullPath, "/");
        strcat(fullPath, lFilename);

        if (!SD.exists(fullPath))
        {

            strcpy(filename, lFilename);
            return true;
        }
    }

    return false;
}

SdDebug sdDebug;