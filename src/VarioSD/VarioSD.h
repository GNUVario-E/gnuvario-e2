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
    bool info(void);
    void test(void);

    void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
    void createDir(fs::FS &fs, const char *path);
    void removeDir(fs::FS &fs, const char *path);
    void readFile(fs::FS &fs, const char *path);
    void writeFile(fs::FS &fs, const char *path, const char *message);
    void appendFile(fs::FS &fs, const char *path, const char *message);
    void renameFile(fs::FS &fs, const char *path1, const char *path2);
    void deleteFile(fs::FS &fs, const char *path);
    void testFileIO(fs::FS &fs, const char *path);

private:
    SPIClass spiSdCard{VSPI}; //HSPI};
};

#endif //_VARIO_SD_H