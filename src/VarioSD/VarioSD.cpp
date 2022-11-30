#include "VarioSD.h"

// #include "FS.h"
#include "SD.h"

#include "HardwareConfig/HardwareConfig.h"
#include "VarioDebug/VarioDebug.h"

bool VarioSD::init(void)
{
  // issue with SD not resolved
  // https://github.com/espressif/arduino-esp32/issues/2171
  spiSdCard.begin(PIN_SDCARD_SCK, PIN_SDCARD_MISO, PIN_SDCARD_MOSI, PIN_SDCARD_CS);

  return (SD.begin(PIN_SDCARD_CS, spiSdCard, 80000000));
}

void VarioSD::end(void)
{
  spiSdCard.end();
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
