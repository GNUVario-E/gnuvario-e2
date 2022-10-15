#pragma once
#include "VarioSD/VarioSD.h"

class VarioIgcFile
{

private:
    uint16_t num;
    const char flightDir[9] = "/flights";
    char filename[13];
    File currentFile;
    bool isCreated = false;
    bool getNextFileName(char *filename, uint8_t day, uint8_t month, uint8_t year);
    void initHeaders(const char *pilot, const char *glider, uint8_t day, uint8_t month, uint8_t year);
    bool appendToFile(const char *message);

public:
    VarioIgcFile();
    bool createNewIgcFile(uint8_t day, uint8_t month, uint8_t year);
    bool getIsCreated();
    bool writeline(char *line);
};
