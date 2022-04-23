#ifndef _VARIO_SETTINGS_H_
#define _VARIO_SETTINGS_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

class VarioSettings
{
private:
  File myFile;

public:
  VarioSettings();
  boolean init();

  boolean readSDSettings(char *FileName);
  void loadConfigurationVario(char *filename);
  void saveConfigurationVario(char *filename);
  boolean readSDSettingsSound(char *FileName);
};

extern VarioSettings varioSettings;
#endif
