
#ifndef _VARIO_SD_H
#define _VARIO_SD_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

class VarioSD
{
public:
    bool init(void);
    void end(void);
    //   void printDirectory();
    //   void printAllDirectory(File dir, int numTabs);

private:
    SPIClass spiSdCard{HSPI};
};

#endif //_VARIO_SD_H