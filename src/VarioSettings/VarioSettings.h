#ifndef _VARIO_SETTINGS_H_
#define _VARIO_SETTINGS_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "VarioDisplay/VarioScreen/ScreenData.h"

class VarioSettings
{
private:
  File myFile;
  S_WIDGET_DATA getScreenDataInsideValues(JsonObject *objInside);
  void setScreenDataValues(JsonObject *obj, ScreenData *screenData);

public:
  VarioSettings();
  boolean init();

  boolean readSDSettings(char *fileName);
  void loadConfigurationVario(char *filename);
  void loadScreenVario(char *filename);
  void saveConfigurationVario(char *filename);
  boolean readSDSettingsSound(char *FileName);
};

extern VarioSettings varioSettings;
extern ScreenData bootScreenData;
extern ScreenData wifiScreenData;
extern ScreenData calibrationScreenData;
extern ScreenData vario1ScreenData;
extern ScreenData vario2ScreenData;
extern ScreenData vario3ScreenData;
extern ScreenData soundScreenData;
extern ScreenData statisticScreenData;
extern ScreenData rebootScreenData;
#endif
