#pragma once

#include "FS.h"
#include "SD.h"
#include "SPI.h"

class VarioSD
{
public:
    bool init(void);
    void end(void);
    static bool isStarted;
    //   void printDirectory();
    //   void printAllDirectory(File dir, int numTabs);

private:
    SPIClass spiSdCard{HSPI};
};
