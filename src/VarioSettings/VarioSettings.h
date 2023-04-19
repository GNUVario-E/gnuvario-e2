#ifndef _VARIO_SETTINGS_H_
#define _VARIO_SETTINGS_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "VarioDisplay/VarioScreen/ScreenData.h"
#include "VarioParameter/Parameters.h"
#include "VarioParameter/ConfigParameter.h"

class VarioSettings
{
private:
  Parameters *params = Parameters::getInstance();

  File myFile;
  S_WIDGET_DATA getScreenDataInsideValues(JsonObject *objInside);
  void setScreenDataValues(JsonObject *obj, ScreenData *screenData);
  void applySetting(const char *settingName, const char *settingValue);
  uint8_t correctParseVertAccelBias(const char *settingValue);
  const char *trimSpace(char *str);

  bool setParameterFromJsonObject(JsonObject *section, ConfigParameter<bool> *param);
  bool setParameterFromJsonObject(JsonObject *section, ConfigParameter<float> *param);
  bool setParameterFromJsonObject(JsonObject *section, ConfigParameter<int8_t> *param);
  bool setParameterFromJsonObject(JsonObject *section, ConfigParameter<uint8_t> *param);
  bool setParameterFromJsonObject(JsonObject *section, ConfigParameter<int16_t> *param);
  bool setParameterFromJsonObject(JsonObject *section, ConfigParameter<uint16_t> *param);
  bool setParameterFromJsonObject(JsonObject *section, ConfigParameter<const char *> *param);

public:
  VarioSettings();

  boolean readSDSettings(char *fileName);
  bool loadConfigurationVario(char *filename);
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
extern ScreenData messageScreenData;
#endif
