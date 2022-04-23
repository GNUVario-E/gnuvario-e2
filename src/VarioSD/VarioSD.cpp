#include "VarioSD.h"

// #include "FS.h"
#include "SD.h"

#include "HardwareConfig/HardwareConfig.h"
#include "VarioDebug/VarioDebug.h"

bool VarioSD::init(void)
{
    spiSdCard.begin(PIN_SDCARD_SCK, PIN_SDCARD_MISO, PIN_SDCARD_MOSI, PIN_SDCARD_CS);
    return (SD.begin(PIN_SDCARD_CS, spiSdCard, 80000000));
}

void VarioSD::end(void)
{
    spiSdCard.end();
}

bool VarioSD::info(void)
{
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
 //   Serial.println("No SD card attached");
    VARIO_PROG_DEBUG_PRINTLN("No SD card attached");
    return false;
  }

//  Serial.print("SD Card Type: ");
  VARIO_PROG_DEBUG_PRINTLN("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
//    Serial.println("MMC");
    VARIO_PROG_DEBUG_PRINTLN("MMC");
  }
  else if (cardType == CARD_SD)
  {
//  Serial.println("SDSC");
    VARIO_PROG_DEBUG_PRINTLN("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
//    Serial.println("SDHC");
    VARIO_PROG_DEBUG_PRINTLN("SDHC");
  }
  else
  {
//    Serial.println("UNKNOWN");
      VARIO_PROG_DEBUG_PRINTLN("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
#ifdef SDCARD_DEBUG
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
#endif

  return true;
}

/*
void VarioSD::printDirectory()
  {
    File root;
    root = SD.open("/"); //, FILE_READ);
 //   File entry;
 //   entry = dir.openNextFile(FILE_READ);
 //   Serial.println(entry.name());
    root.rewindDirectory();
    printAllDirectory(root, 0);

    Serial.println("done!");
}


void VarioSD::printAllDirectory(File dir, int numTabs) 
{

  while (true) {

    File entry =  dir.openNextFile();

    if (! entry) {

      // no more files

      if (numTabs == 0) Serial.println("** Done **");

      break;

    }

    for (uint8_t i = 0; i < numTabs; i++) {

      Serial.print('\t');

    }

    Serial.print(entry.name());

    if (entry.isDirectory()) {

      Serial.println("/");

      printAllDirectory(entry, numTabs + 1);

    } else {

      // files have sizes, directories do not

      Serial.print("\t\t");

      Serial.println(entry.size(), DEC);

    }

    entry.close();

  }
}*/

void VarioSD::test()
{
  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  removeDir(SD, "/mydir");
  listDir(SD, "/", 2);
  writeFile(SD, "/hello.txt", "Hello ");
  appendFile(SD, "/hello.txt", "World!\n");
  readFile(SD, "/hello.txt");
  deleteFile(SD, "/foo.txt");
  renameFile(SD, "/hello.txt", "/foo.txt");
  readFile(SD, "/foo.txt");
  testFileIO(SD, "/test.txt");
#ifdef SDCARD_DEBUG
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
#endif
}

void VarioSD::listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Listing directory: %s\n", dirname);
#endif

  File root = fs.open(dirname);
  if (!root)
  {
//    Serial.println("Failed to open directory");
    VARIO_PROG_DEBUG_PRINTLN("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
//    Serial.println("Not a directory");
    VARIO_PROG_DEBUG_PRINTLN("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
#ifdef SDCARD_DEBUG
      Serial.print("  DIR : ");
      Serial.println(file.name());
#endif
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
#ifdef SDCARD_DEBUG
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
#endif
    }
    file = root.openNextFile();
  }
}

void VarioSD::createDir(fs::FS &fs, const char *path)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Creating Dir: %s\n", path);
#endif
  if (fs.mkdir(path))
  {
//    Serial.println("Dir created");
    VARIO_PROG_DEBUG_PRINTLN("Dir created");
  }
  else
  {
//    Serial.println("mkdir failed");
    VARIO_PROG_DEBUG_PRINTLN("mkdir failed");
  }
}

void VarioSD::removeDir(fs::FS &fs, const char *path)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Removing Dir: %s\n", path);
#endif
  if (fs.rmdir(path))
  {
//    Serial.println("Dir removed");
    VARIO_PROG_DEBUG_PRINTLN("Dir removed");
  }
  else
  {
//    Serial.println("rmdir failed");
    VARIO_PROG_DEBUG_PRINTLN("rmdir failed");
  }
}

void VarioSD::readFile(fs::FS &fs, const char *path)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Reading file: %s\n", path);
#endif

  File file = fs.open(path);
  if (!file)
  {
//    Serial.println("Failed to open file for reading");
    VARIO_PROG_DEBUG_PRINTLN("Failed to open file for reading");
    return;
  }

//  Serial.print("Read from file: ");
  VARIO_PROG_DEBUG_PRINTLN("Read from file: ");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}

void VarioSD::writeFile(fs::FS &fs, const char *path, const char *message)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Writing file: %s\n", path);
#endif

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
//    Serial.println("Failed to open file for writing");
    VARIO_PROG_DEBUG_PRINTLN("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
//    Serial.println("File written");
    VARIO_PROG_DEBUG_PRINTLN("File written");
  }
  else
  {
//    Serial.println("Write failed");
    VARIO_PROG_DEBUG_PRINTLN("Write failed");
  }
  file.close();
}

void VarioSD::appendFile(fs::FS &fs, const char *path, const char *message)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Appending to file: %s\n", path);
#endif

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
//    Serial.println("Failed to open file for appending");
    VARIO_PROG_DEBUG_PRINTLN("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
//    Serial.println("Message appended");
    VARIO_PROG_DEBUG_PRINTLN("Message appended");
  }
  else
  {
//    Serial.println("Append failed");
    VARIO_PROG_DEBUG_PRINTLN("Append failed");
  }
  file.close();
}

void VarioSD::renameFile(fs::FS &fs, const char *path1, const char *path2)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Renaming file %s to %s\n", path1, path2);
#endif
  if (fs.rename(path1, path2))
  {
//    Serial.println("File renamed");
    VARIO_PROG_DEBUG_PRINTLN("File renamed");
  }
  else
  {
//    Serial.println("Rename failed");
    VARIO_PROG_DEBUG_PRINTLN("Rename failed");
  }
}

void VarioSD::deleteFile(fs::FS &fs, const char *path)
{
#ifdef SDCARD_DEBUG
  Serial.printf("Deleting file: %s\n", path);
#endif
  if (fs.remove(path))
  {
//    Serial.println("File deleted");
    VARIO_PROG_DEBUG_PRINTLN("File deleted");
  }
  else
  {
//    Serial.println("Delete failed");
    VARIO_PROG_DEBUG_PRINTLN("Delete failed");
  }
}

void VarioSD::testFileIO(fs::FS &fs, const char *path)
{
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file)
  {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len)
    {
      size_t toRead = len;
      if (toRead > 512)
      {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
#ifdef SDCARD_DEBUG
    Serial.printf("%u bytes read for %u ms\n", flen, end);
#endif
    file.close();
  }
  else
  {
  //  Serial.println("Failed to open file for reading");
    VARIO_PROG_DEBUG_PRINTLN("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file)
  {
//    Serial.println("Failed to open file for writing");
    VARIO_PROG_DEBUG_PRINTLN("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++)
  {
    file.write(buf, 512);
  }
  end = millis() - start;
#ifdef SDCARD_DEBUG
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
#endif
  file.close();
}
