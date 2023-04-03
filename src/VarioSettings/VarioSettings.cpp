
#include <Arduino.h>
#include <ArduinoJson.h>
#include "VarioSettings/VarioSettings.h"
#include "HardwareConfig/HardwareConfig.h"
#include "VarioParameter/ParamDefiniton.h"
#include "version.h"
#include "VarioTool/VarioTool.h"
#include "VarioDebug/VarioDebug.h"
#include "VarioParameter/ParamDefiniton.h"

VarioSettings::VarioSettings()
{
}

boolean VarioSettings::readSDSettings(char *fileName)
{
  File fileToRead;
  char settingName[50];
  char settingValue[50];
  bool isName = false;
  bool isValue = false;
  char line[RL_MAX_CHARS];

  fileToRead = SD.open(fileName);
  if (fileToRead)
  {
    while (fileToRead.available())
    {
      if (VarioTool::readLines(&fileToRead, line))
      {
        // settingName[0] = '\0';
        // settingValue[0] = '\0';
        strcpy(settingName, "");
        strcpy(settingValue, "");

        isName = false;
        isValue = false;
        for (uint8_t i = 0; i < sizeof(line); i++)
        {
          if (line[i] == '[')
          {
            isName = true;
            i++;
          }
          if (!isValue && line[i] == '=')
          {
            isName = false;
            isValue = true;
            i++;
          }
          if (line[i] == ']')
          {
            break;
          }
          if (isName)
          {
            VarioTool::appendChar(settingName, line[i]);
          }
          if (isValue)
          {
            VarioTool::appendChar(settingValue, line[i]);
          }
        }
        if (strlen(settingName) > 0)
        {

          VARIO_SDCARD_DEBUG_DUMP(settingName);
          VARIO_SDCARD_DEBUG_DUMP(settingValue);
          applySetting(settingName, settingValue);
        }
      }
    }

    // close the file:
    fileToRead.close();
    return true;
  }
  else
  {
    // if the file didn't open, print an error:
    VARIO_SDCARD_DEBUG_PRINTLN("error opening settings FILE");
    return false;
  }
}

boolean VarioSettings::readSDSettingsSound(char *fileName)
{
  uint8_t tmpValue = 0;
  return tmpValue;
}

bool VarioSettings::loadConfigurationVario(char *filename)
{
  DynamicJsonDocument doc(2048);

  // Open file for reading
  boolean isFileParamsOK = true;

  // Check if file exists
  if (!SD.exists(filename))
  {
    VARIO_SDCARD_DEBUG_PRINTLN("File not found");
    saveConfigurationVario(filename);
    return false;
  }

  File file = SD.open(filename, FILE_READ);
  if (!file)
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to read file");
    return false;
  }
  // Clearing Buffer
  doc.clear();

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to read file, using default configuration");
    VARIO_SDCARD_DEBUG_PRINTLN(error.c_str());
    file.close();
    return false;
  }

  VARIO_SDCARD_DEBUG_PRINTLN("Paramètres : ");

  const char *GnuvarioE_version = doc["gnuvarioe"]["version"]; // "1.0"
  if (strcmp(GnuvarioE_version, PARAMS_VERSION) != 0)
  {
    isFileParamsOK = false;
  }

  VARIO_SDCARD_DEBUG_PRINT("Version du fichier: ");
  VARIO_SDCARD_DEBUG_PRINTLN(GnuvarioE_version);
  if (!isFileParamsOK)
  {
    VARIO_SDCARD_DEBUG_PRINTLN("AVEC MISE A JOUR DU FICHIER");
  }

  //*****    SYSTEME *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** Systeme *******");

  JsonObject Systeme = doc["systeme"];

  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_BT_ENABLE);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_NO_RECORD);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_BEEP_GPSFIX);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_BEEP_FLYBEGIN);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_BEEP_VARIOBEGIN);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_MUTE_VARIOBEGIN);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_SLEEP_TIMEOUT_MINUTES);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_SLEEP_THRESHOLD_CPS);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_MULTIDISPLAY_DURATION);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_DISPLAY_STAT_DURATION);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_URL_UPDATE);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Systeme, params->P_LANGUAGE);

  //*****    GENERAL *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** General *******");

  JsonObject General = doc["general"];
  JsonObject General_GLIDER = General["GLIDER"];

  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&General, params->P_PILOT_NAME);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&General_GLIDER, params->P_GLIDER_SELECT);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&General_GLIDER, params->P_GLIDER_NAME1);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&General_GLIDER, params->P_GLIDER_NAME2);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&General_GLIDER, params->P_GLIDER_NAME3);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&General_GLIDER, params->P_GLIDER_NAME4);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&General, params->P_TIME_ZONE);

  //*****  VARIO *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** Vario *******");

  JsonObject Vario = doc["vario"];

  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SINKING_THRESHOLD)->setParameterFromJsonObject(&Vario, "SINKING_THRESHOLD");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_CLIMBING_THRESHOLD)->setParameterFromJsonObject(&Vario, "CLIMBING_THRESHOLD");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_NEAR_CLIMBING_SENSITIVITY)->setParameterFromJsonObject(&Vario, "NEAR_CLIMBING_SENSITIVITY");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_ALARM)->setParameterFromJsonObject(&Vario, "ENABLE_NEAR_CLIMBING_ALARM");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_BEEP)->setParameterFromJsonObject(&Vario, "ENABLE_NEAR_CLIMBING_BEEP");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_DISPLAY_INTEGRATED_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "DISPLAY_INTEGRATED_CLIMB_RATE");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RATIO_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "RATIO_CLIMB_RATE");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_CLIMB_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "CLIMB_PERIOD_COUNT");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SETTINGS_GLIDE_RATIO_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "SETTINGS_GLIDE_RATIO_PERIOD_COUNT");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RATIO_MAX_VALUE)->setParameterFromJsonObject(&Vario, "RATIO_MAX_VALUE");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RATIO_MIN_SPEED)->setParameterFromJsonObject(&Vario, "RATIO_MIN_SPEED");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_VARIOMETER_ENABLE_AGL)->setParameterFromJsonObject(&Vario, "VARIOMETER_ENABLE_AGL");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SENT_LXNAV_SENTENCE)->setParameterFromJsonObject(&Vario, "SENT_LXNAV_SENTENCE");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->setParameterFromJsonObject(&Vario, "ACCELERATION_MEASURE_STANDARD_DEVIATION");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_VARIOMETER_INTEGRATED_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "VARIOMETER_INTEGRATED_CLIMB_RATE");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SETTINGS_VARIO_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "SETTINGS_VARIO_PERIOD_COUNT");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_BLUETOOTH_SEND_CALIBRATED_ALTITUDE)->setParameterFromJsonObject(&Vario, "BLUETOOTH_SEND_CALIBRATED_ALTITUDE");

  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&Vario, params->P_ACCELERATION_MEASURE_STANDARD_DEVIATION);

  //*****  FLIGHT START *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** Flight start *******");

  JsonObject FlightStart = doc["flightstart"];

  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_MIN_TIMESTAMP)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_MIN_TIMESTAMP");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_VARIO_LOW_THRESHOLD)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_VARIO_LOW_THRESHOLD");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_VARIO_HIGH_THRESHOLD)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_VARIO_HIGH_THRESHOLD");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_MIN_SPEED)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_MIN_SPEED");
  // isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RECORD_WHEN_FLIGHT_START)->setParameterFromJsonObject(&FlightStart, "RECORD_WHEN_FLIGHT_START");

  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&FlightStart, params->P_FLIGHT_START_VARIO_LOW_THRESHOLD);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&FlightStart, params->P_FLIGHT_START_VARIO_HIGH_THRESHOLD);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&FlightStart, params->P_FLIGHT_START_MIN_SPEED);
  isFileParamsOK = isFileParamsOK && setParameterFromJsonObject(&FlightStart, params->P_RECORD_WHEN_FLIGHT_START);

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();

#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif

  // Clearing Buffer
  doc.clear();

#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif

  // Mise à jour du fichier params.json
  if (!isFileParamsOK)
  {
    saveConfigurationVario(filename);
  }

  return true;
}

void VarioSettings::loadScreenVario(char *filename)
{

  // Open file for reading
  File file = SD.open(filename, FILE_READ);
  if (!file)
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to read file");
    return;
  }

  if (!readJsonSectionToScreenData("boot", file, &bootScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("wifi", file, &wifiScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("calibration", file, &calibrationScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("vario1", file, &vario1ScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("vario2", file, &vario2ScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("vario3", file, &vario3ScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("sound", file, &soundScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("statistic", file, &statisticScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("reboot", file, &rebootScreenData))
  {
    file.close();
    return;
  }

  if (!readJsonSectionToScreenData("message", file, &messageScreenData))
  {
    file.close();
    return;
  }
}

bool VarioSettings::readJsonSectionToScreenData(const char *sectionName, File &input, ScreenData *screenData)
{
  DynamicJsonDocument doc(5000);

  // Rewind the file pointer to the beginning of the file
  input.seek(0);

  doc.clear();

  StaticJsonDocument<64> filter;
  filter[sectionName] = true;
  DeserializationError error = deserializeJson(doc, input, DeserializationOption::Filter(filter));

  if (error)
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to parse file");
    VARIO_SDCARD_DEBUG_PRINTLN(error.c_str());

    return false;
  }

  JsonObject obj;
  obj = doc[sectionName];
  setScreenDataValues(&obj, screenData);

  return true;
}

void VarioSettings::setScreenDataValues(JsonObject *obj, ScreenData *screenData)
{
  JsonObject sec = *obj;
  JsonObject objInside;

  objInside = sec["logo"];
  screenData->logo = getScreenDataInsideValues(&objInside);

  objInside = sec["txt1"];
  screenData->txt1 = getScreenDataInsideValues(&objInside);

  objInside = sec["txt2"];
  screenData->txt2 = getScreenDataInsideValues(&objInside);

  objInside = sec["txt3"];
  screenData->txt3 = getScreenDataInsideValues(&objInside);

  objInside = sec["txt4"];
  screenData->txt4 = getScreenDataInsideValues(&objInside);

  objInside = sec["txt5"];
  screenData->txt5 = getScreenDataInsideValues(&objInside);

  objInside = sec["txt6"];
  screenData->txt6 = getScreenDataInsideValues(&objInside);

  objInside = sec["txt7"];
  screenData->txt7 = getScreenDataInsideValues(&objInside);

  objInside = sec["txt8"];
  screenData->txt8 = getScreenDataInsideValues(&objInside);

  objInside = sec["alti"];
  screenData->alti = getScreenDataInsideValues(&objInside);

  objInside = sec["vario"];
  screenData->vario = getScreenDataInsideValues(&objInside);

  objInside = sec["toolbar"];
  screenData->toolbar = getScreenDataInsideValues(&objInside);

  objInside = sec["speed"];
  screenData->speed = getScreenDataInsideValues(&objInside);

  objInside = sec["bearing"];
  screenData->bearing = getScreenDataInsideValues(&objInside);

  objInside = sec["time"];
  screenData->time = getScreenDataInsideValues(&objInside);

  objInside = sec["flighttime"];
  screenData->flighttime = getScreenDataInsideValues(&objInside);

  objInside = sec["lat"];
  screenData->lat = getScreenDataInsideValues(&objInside);

  objInside = sec["lon"];
  screenData->lon = getScreenDataInsideValues(&objInside);

  objInside = sec["altigps"];
  screenData->altigps = getScreenDataInsideValues(&objInside);

  objInside = sec["agl"];
  screenData->agl = getScreenDataInsideValues(&objInside);

  objInside = sec["wind"];
  screenData->wind = getScreenDataInsideValues(&objInside);

  objInside = sec["circle"];
  screenData->circle = getScreenDataInsideValues(&objInside);

  objInside = sec["volIcon"];
  if (!objInside.isNull())
  {
    screenData->volumeIcon = getScreenDataInsideValues(&objInside);
  }

  objInside = sec["volText"];
  if (!objInside.isNull())
  {
    screenData->volumeText = getScreenDataInsideValues(&objInside);
  }
}

S_WIDGET_DATA VarioSettings::getScreenDataInsideValues(JsonObject *objInside)
{
  JsonObject sec = *objInside;
  return {
      sec["a"].as<bool>(),
      sec["b"].as<bool>(),
      sec["x"].as<unsigned short>(),
      sec["y"].as<unsigned short>(),
      sec["w"].as<unsigned short>(),
      sec["h"].as<unsigned short>(),
      sec["n"].as<unsigned char>()};
}

// Saves the configuration to a file
void VarioSettings::saveConfigurationVario(char *filename)
{
  DynamicJsonDocument doc(2048);

  // Delete existing file, otherwise the configuration is appended to the file
  if (SD.exists(filename))
  {
    SD.remove(filename);
  }

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file)
  {

    VARIO_SDCARD_DEBUG_PRINTLN("Failed to create file");
    return;
  }

  VARIO_SDCARD_DEBUG_PRINTLN("****** SAUVEGARDE params.json *******");
#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif

  // Clearing Buffer
  doc.clear();

  VARIO_SDCARD_DEBUG_PRINTLN("****** GnuvarioE *******");

  // Set the values in the document
  JsonObject GnuvarioE = doc.createNestedObject("gnuvarioe");
  GnuvarioE["version"] = PARAMS_VERSION;

  //*****    SYSTEME *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** Systeme *******");

  JsonObject Systeme = doc.createNestedObject("systeme");

  Systeme["BT_ENABLE"] = params->P_BT_ENABLE->getValue() ? 1 : 0;
  Systeme["NO_RECORD"] = params->P_NO_RECORD->getValue() ? 1 : 0;
  Systeme["BEEP_GPSFIX"] = params->P_BEEP_GPSFIX->getValue() ? 1 : 0;
  Systeme["BEEP_FLYBEGIN"] = params->P_BEEP_FLYBEGIN->getValue() ? 1 : 0;
  Systeme["BEEP_VARIOBEGIN"] = params->P_BEEP_VARIOBEGIN->getValue() ? 1 : 0;
  Systeme["MUTE_VARIOBEGIN"] = params->P_MUTE_VARIOBEGIN->getValue() ? 1 : 0;

  Systeme["SLEEP_TIMEOUT_MINUTES"] = params->P_SLEEP_TIMEOUT_MINUTES->getValue();
  Systeme["SLEEP_THRESHOLD_CPS"] = params->P_SLEEP_THRESHOLD_CPS->getValue();
  Systeme["MULTIDISPLAY_DURATION"] = params->P_MULTIDISPLAY_DURATION->getValue();

  Systeme["DISPLAY_STAT_DURATION"] = params->P_DISPLAY_STAT_DURATION->getValue();
  Systeme["URL_UPDATE"] = params->P_URL_UPDATE->getValue();
  Systeme["LANGUAGE"] = params->P_LANGUAGE->getValue();

  //*****    GENERAL *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** General *******");

  JsonObject General = doc.createNestedObject("general");

  General["PILOT_NAME"] = params->P_PILOT_NAME->getValue();

  JsonObject General_GLIDER = General.createNestedObject("GLIDER");

  General_GLIDER["GLIDER_SELECT"] = params->P_GLIDER_SELECT->getValue();
  General_GLIDER["GLIDER_NAME1"] = params->P_GLIDER_NAME1->getValue();
  General_GLIDER["GLIDER_NAME2"] = params->P_GLIDER_NAME2->getValue();
  General_GLIDER["GLIDER_NAME3"] = params->P_GLIDER_NAME3->getValue();
  General_GLIDER["GLIDER_NAME4"] = params->P_GLIDER_NAME4->getValue();

  General["TIME_ZONE"] = params->P_TIME_ZONE->getValue();

  //*****    VARIO *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** Vario *******");

  JsonObject Vario = doc.createNestedObject("vario");

  // Vario["SINKING_THRESHOLD"] = varioData.getParam(PARAM_SINKING_THRESHOLD)->getValueFloat();
  // Vario["CLIMBING_THRESHOLD"] = varioData.getParam(PARAM_CLIMBING_THRESHOLD)->getValueFloat();
  // Vario["NEAR_CLIMBING_SENSITIVITY"] = varioData.getParam(PARAM_NEAR_CLIMBING_SENSITIVITY)->getValueFloat();
  // Vario["ENABLE_NEAR_CLIMBING_ALARM"] = varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_ALARM)->getValueBool() ? 1 : 0;
  // Vario["ENABLE_NEAR_CLIMBING_BEEP"] = varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_BEEP)->getValueBool() ? 1 : 0;
  // Vario["DISPLAY_INTEGRATED_CLIMB_RATE"] = varioData.getParam(PARAM_DISPLAY_INTEGRATED_CLIMB_RATE)->getValueBool() ? 1 : 0;
  // Vario["RATIO_CLIMB_RATE"] = varioData.getParam(PARAM_RATIO_CLIMB_RATE)->getValueUInt8();
  // Vario["CLIMB_PERIOD_COUNT"] = varioData.getParam(PARAM_CLIMB_PERIOD_COUNT)->getValueInt8();
  // Vario["SETTINGS_GLIDE_RATIO_PERIOD_COUNT"] = varioData.getParam(PARAM_SETTINGS_GLIDE_RATIO_PERIOD_COUNT)->getValueInt8();
  // Vario["RATIO_MAX_VALUE"] = varioData.getParam(PARAM_RATIO_MAX_VALUE)->getValueFloat();
  // Vario["RATIO_MIN_SPEED"] = varioData.getParam(PARAM_RATIO_MIN_SPEED)->getValueFloat();
  // Vario["VARIOMETER_ENABLE_AGL"] = varioData.getParam(PARAM_VARIOMETER_ENABLE_AGL)->getValueBool() ? 1 : 0;
  // Vario["SENT_LXNAV_SENTENCE"] = varioData.getParam(PARAM_SENT_LXNAV_SENTENCE)->getValueBool() ? 1 : 0;
  // Vario["ACCELERATION_MEASURE_STANDARD_DEVIATION"] = varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->getValueFloat();
  // Vario["VARIOMETER_INTEGRATED_CLIMB_RATE"] = varioData.getParam(PARAM_VARIOMETER_INTEGRATED_CLIMB_RATE)->getValueBool() ? 1 : 0;
  // Vario["SETTINGS_VARIO_PERIOD_COUNT"] = varioData.getParam(PARAM_SETTINGS_VARIO_PERIOD_COUNT)->getValueUInt8();
  // Vario["BLUETOOTH_SEND_CALIBRATED_ALTITUDE"] = varioData.getParam(PARAM_BLUETOOTH_SEND_CALIBRATED_ALTITUDE)->getValueBool() ? 1 : 0;

  // 2 decimals
  Vario["ACCELERATION_MEASURE_STANDARD_DEVIATION"] = round(params->P_ACCELERATION_MEASURE_STANDARD_DEVIATION->getValue() * 100.0) / 100.0;

  //*****    Flight_Start *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** Flight start *******");

  JsonObject FlightStart = doc.createNestedObject("flightstart");

  // FlightStart["FLIGHT_START_MIN_TIMESTAMP"] = varioData.getParam(PARAM_FLIGHT_START_MIN_TIMESTAMP)->getValueUInt16();
  // FlightStart["FLIGHT_START_VARIO_LOW_THRESHOLD"] = varioData.getParam(PARAM_FLIGHT_START_VARIO_LOW_THRESHOLD)->getValueFloat();
  // FlightStart["FLIGHT_START_VARIO_HIGH_THRESHOLD"] = varioData.getParam(PARAM_FLIGHT_START_VARIO_HIGH_THRESHOLD)->getValueFloat();
  // FlightStart["FLIGHT_START_MIN_SPEED"] = varioData.getParam(PARAM_FLIGHT_START_MIN_SPEED)->getValueFloat();
  // FlightStart["RECORD_WHEN_FLIGHT_START"] = varioData.getParam(PARAM_RECORD_WHEN_FLIGHT_START)->getValueBool() ? 1 : 0;

  FlightStart["FLIGHT_START_VARIO_LOW_THRESHOLD"] = params->P_FLIGHT_START_VARIO_LOW_THRESHOLD->getValue();
  FlightStart["FLIGHT_START_VARIO_HIGH_THRESHOLD"] = params->P_FLIGHT_START_VARIO_HIGH_THRESHOLD->getValue();
  FlightStart["FLIGHT_START_MIN_SPEED"] = params->P_FLIGHT_START_MIN_SPEED->getValue();
  FlightStart["RECORD_WHEN_FLIGHT_START"] = params->P_RECORD_WHEN_FLIGHT_START->getValue() ? 1 : 0;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0)
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to write to file");
  }

  // Close the file
  file.close();
  // Clearing Buffer
  doc.clear();

#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif
}

void VarioSettings::applySetting(const char *settingName, const char *settingValue)
{
  if (strcmp(settingName, PARAM_SSID_1) == 0)
  {
    params->P_SSID_1->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_PASSWORD_1) == 0)
  {
    params->P_PASSWORD_1->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_SSID_2) == 0)
  {
    params->P_SSID_2->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_PASSWORD_2) == 0)
  {
    params->P_PASSWORD_2->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_SSID_3) == 0)
  {
    params->P_SSID_3->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_PASSWORD_3) == 0)
  {
    params->P_PASSWORD_3->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_SSID_4) == 0)
  {
    params->P_SSID_4->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_PASSWORD_4) == 0)
  {
    params->P_PASSWORD_4->setValue(settingValue);
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_00) == 0)
  {
    // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_00->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_01) == 0)
  {
    // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_01->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_02) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_02->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_03) == 0)
  {
    // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_03->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_04) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_04->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_05) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_05->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_06) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_06->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_07) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_07->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_08) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_08->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_09) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_09->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_10) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_10->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_GYRO_CAL_BIAS_11) == 0)
  { // rustine pour le fichier de calibration
    params->P_VERTACCEL_GYRO_CAL_BIAS_11->setValue(correctParseVertAccelBias(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_ACCEL_CAL_BIAS_00) == 0)
  {
    params->P_VERTACCEL_ACCEL_CAL_BIAS_00->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_ACCEL_CAL_BIAS_01) == 0)
  {
    params->P_VERTACCEL_ACCEL_CAL_BIAS_01->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_ACCEL_CAL_BIAS_02) == 0)
  {
    params->P_VERTACCEL_ACCEL_CAL_BIAS_02->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_ACCEL_CAL_SCALE) == 0)
  {
    params->P_VERTACCEL_ACCEL_CAL_SCALE->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_MAG_CAL_BIAS_00) == 0)
  {
    params->P_VERTACCEL_MAG_CAL_BIAS_00->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_MAG_CAL_BIAS_01) == 0)
  {
    params->P_VERTACCEL_MAG_CAL_BIAS_01->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_MAG_CAL_BIAS_02) == 0)
  {
    params->P_VERTACCEL_MAG_CAL_BIAS_02->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_MAG_CAL_PROJ_SCALE) == 0)
  {
    params->P_VERTACCEL_MAG_CAL_PROJ_SCALE->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_ACCEL_CAL_BIAS_MULTIPLIER) == 0)
  {
    params->P_VERTACCEL_ACCEL_CAL_BIAS_MULTIPLIER->setValue(atoi(settingValue));
  }
  else if (strcmp(settingName, PARAM_VERTACCEL_MAG_CAL_BIAS_MULTIPLIER) == 0)
  {
    params->P_VERTACCEL_MAG_CAL_BIAS_MULTIPLIER->setValue(atoi(settingValue));
  }
}

uint8_t VarioSettings::correctParseVertAccelBias(const char *settingValue)
{
  // rustine pour le fichier de calibration

  char *end_ptr;
  if (settingValue[0] == '0' && settingValue[1] == 'x')
  {
    return (uint8_t)strtol(settingValue, &end_ptr, 16);
  }
  return 0;
}

bool VarioSettings::setParameterFromJsonObject(JsonObject *section, ConfigParameter<bool> *param)
{
  JsonObject ob = *section;
  bool isFromJson = false;
  const char *tVal;
  const char *key = param->getKey();
  if (section->containsKey(key))
  {
    param->setValue(ob[key].as<bool>());
    isFromJson = true;
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : " + String(param->getValue()));
  }
  else
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : not found");
  }

  return isFromJson;
}

bool VarioSettings::setParameterFromJsonObject(JsonObject *section, ConfigParameter<float> *param)
{
  JsonObject ob = *section;
  bool isFromJson = false;
  const char *tVal;
  const char *key = param->getKey();
  if (section->containsKey(key))
  {
    param->setValue(ob[key].as<float>());
    isFromJson = true;
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : " + String(param->getValue()));
  }
  else
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : not found");
  }

  return isFromJson;
}

bool VarioSettings::setParameterFromJsonObject(JsonObject *section, ConfigParameter<int8_t> *param)
{
  JsonObject ob = *section;
  bool isFromJson = false;
  const char *tVal;
  const char *key = param->getKey();
  if (section->containsKey(key))
  {
    param->setValue(ob[key].as<int8_t>());
    isFromJson = true;
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : " + String(param->getValue()));
  }
  else
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : not found");
  }

  return isFromJson;
}

bool VarioSettings::setParameterFromJsonObject(JsonObject *section, ConfigParameter<uint8_t> *param)
{
  JsonObject ob = *section;

  bool isFromJson = false;
  const char *tVal;
  const char *key = param->getKey();
  if (section->containsKey(key))
  {
    param->setValue(ob[key].as<uint8_t>());
    isFromJson = true;
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : " + String(param->getValue()));
  }
  else
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : not found");
  }

  return isFromJson;
}

bool VarioSettings::setParameterFromJsonObject(JsonObject *section, ConfigParameter<int16_t> *param)
{
  JsonObject ob = *section;
  bool isFromJson = false;
  const char *tVal;
  const char *key = param->getKey();
  if (section->containsKey(key))
  {
    param->setValue(ob[key].as<int16_t>());
    isFromJson = true;
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : " + String(param->getValue()));
  }
  else
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : not found");
  }

  return isFromJson;
}

bool VarioSettings::setParameterFromJsonObject(JsonObject *section, ConfigParameter<uint16_t> *param)
{
  JsonObject ob = *section;
  bool isFromJson = false;
  const char *tVal;
  const char *key = param->getKey();
  if (section->containsKey(key))
  {
    param->setValue(ob[key].as<uint16_t>());
    isFromJson = true;
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : " + String(param->getValue()));
  }
  else
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : not found");
  }

  return isFromJson;
}

bool VarioSettings::setParameterFromJsonObject(JsonObject *section, ConfigParameter<const char *> *param)
{
  JsonObject ob = *section;
  bool isFromJson = false;
  const char *tVal;
  const char *key = param->getKey();
  if (section->containsKey(key))
  {
    param->setValue(ob[key].as<const char *>());
    isFromJson = true;
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : " + String(param->getValue()));
  }
  else
  {
    VARIO_SDCARD_DEBUG_PRINTLN("Json Recup - " + String(key) + " : not found");
  }

  return isFromJson;
}

const char *VarioSettings::trimSpace(char *str)
{
  char *end;
  /* skip leading whitespace */
  while (isspace(*str))
  {
    str = str + 1;
  }
  /* remove trailing whitespace */
  end = str + strlen(str) - 1;
  while (end > str && isspace(*end))
  {
    end = end - 1;
  }
  /* write null character */
  *(end + 1) = '\0';

  return end;
}

VarioSettings varioSettings;
