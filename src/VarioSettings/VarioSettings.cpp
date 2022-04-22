
#include <Arduino.h>
<<<<<<< HEAD
#include <VarioSettings.h>
#include <HardwareConfig.h>
#include "VarioData.h"
#include "ParamsDefinition.h"
=======
#include "VarioSettings.h"
#include "HardwareConfig/HardwareConfig.h"
#include <EEPROM.h>
>>>>>>> 4cc2a87 (add files to be able to compile)
#include <ArduinoJson.h>
#include "version.h"
#include "VarioTool/VarioTool.h"
#include "VarioDebug/VarioDebug.h"

<<<<<<< HEAD
=======
#define SOUND_EEPROM_ADDR 0xC8
#define SOUND_EEPROM_TAG 9806

>>>>>>> 4cc2a87 (add files to be able to compile)
VarioSettings::VarioSettings()
{
}

<<<<<<< HEAD
boolean VarioSettings::init()
{
=======
VarioParam *VarioSettings::getParam(const char *paramName)
{
  for (int i = 0; i < sizeof(Params) / sizeof(VarioParam); i++)
  {
    if (strcmp(paramName, Params[i].getName()) == 0)
    {
      return &Params[i];
    }
  }

  VARIO_SDCARD_DEBUG_PRINTLN("Parameter not found");

  return NULL;
}

boolean VarioSettings::init()
{
  if (!EEPROM.begin(512))
  {
    return false;
  }

>>>>>>> 4cc2a87 (add files to be able to compile)
  return true;
}

boolean VarioSettings::readSDSettings(char *FileName)
{
  File fileToRead;
  char settingName[50];
  char settingValue[50];
  bool isName = false;
  bool isValue = false;
  char line[RL_MAX_CHARS];

  fileToRead = SD.open(FileName);
  if (fileToRead)
  {
    while (fileToRead.available())
    {
      if (VarioTool::readLines(&fileToRead, line))
      {
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
        if (strcmp(settingName, "") != 0)
        {
<<<<<<< HEAD
          varioData.applySettingParam(settingName, settingValue);

/*#ifdef SDCARD_DEBUG
=======
          applySetting(settingName, settingValue);

#ifdef SDCARD_DEBUG
>>>>>>> 4cc2a87 (add files to be able to compile)
          Serial.print("Name:");
          Serial.println(settingName);
          Serial.print("Value :");
          Serial.println(settingValue);
<<<<<<< HEAD
#endif //SDCARD_DEBUG*/
          VARIO_SDCARD_DEBUG_DUMP(settingName);
          VARIO_SDCARD_DEBUG_DUMP(settingValue);
=======
#endif // SDCARD_DEBUG
>>>>>>> 4cc2a87 (add files to be able to compile)
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
<<<<<<< HEAD
/*#ifdef SDCARD_DEBUG
    Serial.println("error opening settings.txt");
#endif //SDCARD_DEBUG*/
    VARIO_SDCARD_DEBUG_PRINTLN("error opening settings FILE");
=======
#ifdef SDCARD_DEBUG
    Serial.println("error opening settings.txt");
#endif // SDCARD_DEBUG
>>>>>>> 4cc2a87 (add files to be able to compile)
    return false;
  }
}

<<<<<<< HEAD
boolean VarioSettings::readSDSettingsSound(char *FileName)
{
  uint8_t tmpValue = 0;
  return tmpValue;
}

=======
/* Apply the value to the parameter by searching for the parameter name */
boolean VarioSettings::applySetting(const char *settingName, const char *settingValue)
{
  VarioParam *vparam;
  vparam = getParam(settingName);

  if (vparam != NULL)
  {
    return vparam->setValue(settingValue);
  }

  Serial.print("Param introuvable :");
  Serial.println(settingName);

  return false;
}

uint8_t VarioSettings::soundSettingRead(void)
{
  /* check tag */
  uint16_t eepromTag;
  uint8_t tmpValue = 0;

  eepromTag = EEPROM.read(SOUND_EEPROM_ADDR);
  eepromTag <<= 8;
  eepromTag += EEPROM.read(SOUND_EEPROM_ADDR + 0x01);

  if (eepromTag != SOUND_EEPROM_TAG)
  {
    // Memoire vide
    tmpValue = DEFAULT_BEEP_VOLUME;
    soundSettingWrite(tmpValue);
    // #ifdef SOUND_DEBUG
    //     SerialPort.print("Memoire vide - sound value : ");
    //     SerialPort.println(tmpValue);
    // #endif //SOUND_DEBUG
  }
  else
  {
    /* read calibration settings */
    tmpValue = EEPROM.read(SOUND_EEPROM_ADDR + 0x02);
    // #ifdef SOUND_DEBUG
    //     SerialPort.print("Read sound value : ");
    //     SerialPort.println(tmpValue);
    // #endif //SOUND_DEBUG
  }

  // #ifdef SOUND_DEBUG
  //   SerialPort.print("Sound value : ");
  //   SerialPort.println(tmpValue);
  // #endif //SOUND_DEBUG

  if ((tmpValue < 0) || (tmpValue > 10))
  {
    tmpValue = 5;
  }
  return tmpValue;
}

void VarioSettings::soundSettingWrite(uint8_t volume)
{
#ifdef SOUND_DEBUG
  Serial.print("Write sound volume : ");
  Serial.println(volume);
#endif // SDCARD_DEBUG

  /* write tag */
  uint16_t eepromTag = SOUND_EEPROM_TAG;
  EEPROM.write(SOUND_EEPROM_ADDR, (eepromTag >> 8) & 0xff);
  EEPROM.write(SOUND_EEPROM_ADDR + 0x01, eepromTag & 0xff);

  EEPROM.write(SOUND_EEPROM_ADDR + 0x02, volume);

  EEPROM.commit();

  getParam(PARAM_BEEP_VOLUME)->setValue(volume);
}

>>>>>>> 4cc2a87 (add files to be able to compile)
void VarioSettings::loadConfigurationVario(char *filename)
{
  // Open file for reading
  boolean isFileParamsOK = true;

  File file = SD.open(filename, FILE_READ);
  if (!file)
  {
<<<<<<< HEAD
    //Serial.println(F("Failed to read file"));
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to read file");
=======
    Serial.println(F("Failed to read file"));
>>>>>>> 4cc2a87 (add files to be able to compile)
    return;
  }
  // Clearing Buffer
  VarioTool::jsonDoc.clear();

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(VarioTool::jsonDoc, file);
  if (error)
  {
<<<<<<< HEAD
 //   Serial.println(F("Failed to read file, using default configuration"));
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to read file, using default configuration");
=======
    Serial.println(F("Failed to read file, using default configuration"));
>>>>>>> 4cc2a87 (add files to be able to compile)
    file.close();
    return;
  }

<<<<<<< HEAD
/*#ifdef SDCARD_DEBUG
//  Serial.println("Paramètres : ");
#endif*/
  VARIO_SDCARD_DEBUG_PRINTLN("Paramètres : ");
=======
#ifdef SDCARD_DEBUG
  Serial.println("Paramètres : ");
#endif
>>>>>>> 4cc2a87 (add files to be able to compile)

  const char *GnuvarioE_version = VarioTool::jsonDoc["gnuvarioe"]["version"]; // "1.0"
  if (strcmp(GnuvarioE_version, PARAMS_VERSION) != 0)
  {
    isFileParamsOK = false;
  }

#ifdef SDCARD_DEBUG
  Serial.print("Version du fichier: ");
  Serial.println(GnuvarioE_version);
  if (!isFileParamsOK)
  {
    Serial.println("AVEC MISE A JOUR DU FICHIER");
  }
#endif

  //*****    SYSTEME *****

<<<<<<< HEAD
/*#ifdef SDCARD_DEBUG
  Serial.println("****** Systeme *******");
#endif*/

  VARIO_SDCARD_DEBUG_PRINTLN("****** Systeme *******");

  JsonObject Systeme = VarioTool::jsonDoc["systeme"];

  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_BT_ENABLE)->setParameterFromJsonObject(&Systeme, "BT_ENABLE");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_NO_RECORD)->setParameterFromJsonObject(&Systeme, "NO_RECORD");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_ALARM_SDCARD)->setParameterFromJsonObject(&Systeme, "ALARM_SDCARD");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_BEEP_GPSFIX)->setParameterFromJsonObject(&Systeme, "BEEP_GPSFIX");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_BEEP_FLYBEGIN)->setParameterFromJsonObject(&Systeme, "BEEP_FLYBEGIN");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_BEEP_VARIOBEGIN)->setParameterFromJsonObject(&Systeme, "BEEP_VARIOBEGIN");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_MUTE_VARIOBEGIN)->setParameterFromJsonObject(&Systeme, "MUTE_VARIOBEGIN");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SLEEP_TIMEOUT_MINUTES)->setParameterFromJsonObject(&Systeme, "SLEEP_TIMEOUT_MINUTES");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SLEEP_THRESHOLD_CPS)->setParameterFromJsonObject(&Systeme, "SLEEP_THRESHOLD_CPS");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_MULTIDISPLAY_DURATION)->setParameterFromJsonObject(&Systeme, "MULTIDISPLAY_DURATION");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_DISPLAY_STAT_DURATION)->setParameterFromJsonObject(&Systeme, "DISPLAY_STAT_DURATION");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_URL_UPDATE)->setParameterFromJsonObject(&Systeme, "URL_UPDATE");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_LANGUAGE)->setParameterFromJsonObject(&Systeme, "LANGUAGE");

  //*****    GENERAL *****

 // Serial.println("****** General *******");

  VARIO_SDCARD_DEBUG_PRINTLN("****** General *******");
=======
#ifdef SDCARD_DEBUG
  Serial.println("****** Systeme *******");
#endif

  JsonObject Systeme = VarioTool::jsonDoc["systeme"];

  isFileParamsOK = isFileParamsOK && getParam(PARAM_BT_ENABLE)->setParameterFromJsonObject(&Systeme, "BT_ENABLE");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_NO_RECORD)->setParameterFromJsonObject(&Systeme, "NO_RECORD");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_ALARM_SDCARD)->setParameterFromJsonObject(&Systeme, "ALARM_SDCARD");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_BEEP_GPSFIX)->setParameterFromJsonObject(&Systeme, "BEEP_GPSFIX");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_BEEP_FLYBEGIN)->setParameterFromJsonObject(&Systeme, "BEEP_FLYBEGIN");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_BEEP_VARIOBEGIN)->setParameterFromJsonObject(&Systeme, "BEEP_VARIOBEGIN");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_MUTE_VARIOBEGIN)->setParameterFromJsonObject(&Systeme, "MUTE_VARIOBEGIN");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_SLEEP_TIMEOUT_MINUTES)->setParameterFromJsonObject(&Systeme, "SLEEP_TIMEOUT_MINUTES");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_SLEEP_THRESHOLD_CPS)->setParameterFromJsonObject(&Systeme, "SLEEP_THRESHOLD_CPS");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_MULTIDISPLAY_DURATION)->setParameterFromJsonObject(&Systeme, "MULTIDISPLAY_DURATION");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_DISPLAY_STAT_DURATION)->setParameterFromJsonObject(&Systeme, "DISPLAY_STAT_DURATION");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_URL_UPDATE)->setParameterFromJsonObject(&Systeme, "URL_UPDATE");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_LANGUAGE)->setParameterFromJsonObject(&Systeme, "LANGUAGE");

  //*****    GENERAL *****

  Serial.println("****** General *******");
>>>>>>> 4cc2a87 (add files to be able to compile)

  JsonObject General = VarioTool::jsonDoc["general"];
  JsonObject General_GLIDER = General["GLIDER"];

<<<<<<< HEAD
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_PILOT_NAME)->setParameterFromJsonObject(&General, "PILOT_NAME");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_GLIDER_SELECT)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_SELECT");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_GLIDER_NAME1)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME1");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_GLIDER_NAME2)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME2");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_GLIDER_NAME3)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME3");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_GLIDER_NAME4)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME4");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_TIME_ZONE)->setParameterFromJsonObject(&General, "TIME_ZONE");

  //*****  VARIO *****

  VARIO_SDCARD_DEBUG_PRINTLN("****** Vario *******");
  // Serial.println("****** Vario *******");

  JsonObject Vario = VarioTool::jsonDoc["vario"];

  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SINKING_THRESHOLD)->setParameterFromJsonObject(&Vario, "SINKING_THRESHOLD");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_CLIMBING_THRESHOLD)->setParameterFromJsonObject(&Vario, "CLIMBING_THRESHOLD");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_NEAR_CLIMBING_SENSITIVITY)->setParameterFromJsonObject(&Vario, "NEAR_CLIMBING_SENSITIVITY");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_ALARM)->setParameterFromJsonObject(&Vario, "ENABLE_NEAR_CLIMBING_ALARM");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_BEEP)->setParameterFromJsonObject(&Vario, "ENABLE_NEAR_CLIMBING_BEEP");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_DISPLAY_INTEGRATED_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "DISPLAY_INTEGRATED_CLIMB_RATE");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RATIO_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "RATIO_CLIMB_RATE");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_CLIMB_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "CLIMB_PERIOD_COUNT");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SETTINGS_GLIDE_RATIO_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "SETTINGS_GLIDE_RATIO_PERIOD_COUNT");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RATIO_MAX_VALUE)->setParameterFromJsonObject(&Vario, "RATIO_MAX_VALUE");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RATIO_MIN_SPEED)->setParameterFromJsonObject(&Vario, "RATIO_MIN_SPEED");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_VARIOMETER_ENABLE_AGL)->setParameterFromJsonObject(&Vario, "VARIOMETER_ENABLE_AGL");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SENT_LXNAV_SENTENCE)->setParameterFromJsonObject(&Vario, "SENT_LXNAV_SENTENCE");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->setParameterFromJsonObject(&Vario, "ACCELERATION_MEASURE_STANDARD_DEVIATION");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_VARIOMETER_INTEGRATED_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "VARIOMETER_INTEGRATED_CLIMB_RATE");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_SETTINGS_VARIO_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "SETTINGS_VARIO_PERIOD_COUNT");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_BLUETOOTH_SEND_CALIBRATED_ALTITUDE)->setParameterFromJsonObject(&Vario, "BLUETOOTH_SEND_CALIBRATED_ALTITUDE");

  //*****  FLIGHT START *****

//  Serial.println("****** Flight start *******");
  VARIO_SDCARD_DEBUG_PRINTLN("****** Flight start *******");

  JsonObject FlightStart = VarioTool::jsonDoc["flightstart"];

  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_MIN_TIMESTAMP)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_MIN_TIMESTAMP");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_VARIO_LOW_THRESHOLD)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_VARIO_LOW_THRESHOLD");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_VARIO_HIGH_THRESHOLD)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_VARIO_HIGH_THRESHOLD");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_FLIGHT_START_MIN_SPEED)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_MIN_SPEED");
  isFileParamsOK = isFileParamsOK && varioData.getParam(PARAM_RECORD_WHEN_FLIGHT_START)->setParameterFromJsonObject(&FlightStart, "RECORD_WHEN_FLIGHT_START");
=======
  isFileParamsOK = isFileParamsOK && getParam(PARAM_PILOT_NAME)->setParameterFromJsonObject(&General, "PILOT_NAME");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_GLIDER_SELECT)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_SELECT");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_GLIDER_NAME1)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME1");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_GLIDER_NAME2)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME2");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_GLIDER_NAME3)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME3");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_GLIDER_NAME4)->setParameterFromJsonObject(&General_GLIDER, "GLIDER_NAME4");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_TIME_ZONE)->setParameterFromJsonObject(&General, "TIME_ZONE");

  //*****  VARIO *****

  Serial.println("****** Vario *******");

  JsonObject Vario = VarioTool::jsonDoc["vario"];

  isFileParamsOK = isFileParamsOK && getParam(PARAM_SINKING_THRESHOLD)->setParameterFromJsonObject(&Vario, "SINKING_THRESHOLD");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_CLIMBING_THRESHOLD)->setParameterFromJsonObject(&Vario, "CLIMBING_THRESHOLD");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_NEAR_CLIMBING_SENSITIVITY)->setParameterFromJsonObject(&Vario, "NEAR_CLIMBING_SENSITIVITY");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_ENABLE_NEAR_CLIMBING_ALARM)->setParameterFromJsonObject(&Vario, "ENABLE_NEAR_CLIMBING_ALARM");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_ENABLE_NEAR_CLIMBING_BEEP)->setParameterFromJsonObject(&Vario, "ENABLE_NEAR_CLIMBING_BEEP");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_DISPLAY_INTEGRATED_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "DISPLAY_INTEGRATED_CLIMB_RATE");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_RATIO_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "RATIO_CLIMB_RATE");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_CLIMB_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "CLIMB_PERIOD_COUNT");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_SETTINGS_GLIDE_RATIO_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "SETTINGS_GLIDE_RATIO_PERIOD_COUNT");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_RATIO_MAX_VALUE)->setParameterFromJsonObject(&Vario, "RATIO_MAX_VALUE");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_RATIO_MIN_SPEED)->setParameterFromJsonObject(&Vario, "RATIO_MIN_SPEED");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_VARIOMETER_ENABLE_AGL)->setParameterFromJsonObject(&Vario, "VARIOMETER_ENABLE_AGL");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_SENT_LXNAV_SENTENCE)->setParameterFromJsonObject(&Vario, "SENT_LXNAV_SENTENCE");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->setParameterFromJsonObject(&Vario, "ACCELERATION_MEASURE_STANDARD_DEVIATION");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_VARIOMETER_INTEGRATED_CLIMB_RATE)->setParameterFromJsonObject(&Vario, "VARIOMETER_INTEGRATED_CLIMB_RATE");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_SETTINGS_VARIO_PERIOD_COUNT)->setParameterFromJsonObject(&Vario, "SETTINGS_VARIO_PERIOD_COUNT");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_BLUETOOTH_SEND_CALIBRATED_ALTITUDE)->setParameterFromJsonObject(&Vario, "BLUETOOTH_SEND_CALIBRATED_ALTITUDE");

  //*****  FLIGHT START *****

  Serial.println("****** Flight start *******");

  JsonObject FlightStart = VarioTool::jsonDoc["flightstart"];

  isFileParamsOK = isFileParamsOK && getParam(PARAM_FLIGHT_START_MIN_TIMESTAMP)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_MIN_TIMESTAMP");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_FLIGHT_START_VARIO_LOW_THRESHOLD)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_VARIO_LOW_THRESHOLD");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_FLIGHT_START_VARIO_HIGH_THRESHOLD)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_VARIO_HIGH_THRESHOLD");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_FLIGHT_START_MIN_SPEED)->setParameterFromJsonObject(&FlightStart, "FLIGHT_START_MIN_SPEED");
  isFileParamsOK = isFileParamsOK && getParam(PARAM_RECORD_WHEN_FLIGHT_START)->setParameterFromJsonObject(&FlightStart, "RECORD_WHEN_FLIGHT_START");
>>>>>>> 4cc2a87 (add files to be able to compile)

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();

<<<<<<< HEAD
#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif

   // Clearing Buffer
  VarioTool::jsonDoc.clear();

#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif

  //Mise à jour du fichier params.jso
  if (!isFileParamsOK)
  {
//    Serial.println("Sauvegarde de nouveaux paramètres");
=======
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());

  // Clearing Buffer
  VarioTool::jsonDoc.clear();

  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());

  // Mise à jour du fichier params.jso
  if (!isFileParamsOK)
  {
    Serial.println("Sauvegarde de nouveaux paramètres");
>>>>>>> 4cc2a87 (add files to be able to compile)
    saveConfigurationVario(filename);
  }
}

// Saves the configuration to a file
void VarioSettings::saveConfigurationVario(char *filename)
{

  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file)
  {

<<<<<<< HEAD
  //  Serial.println(F("Failed to create file"));
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to create file");
    return;
  }

  VARIO_SDCARD_DEBUG_PRINTLN("****** SAUVEGARDE params.jso *******");
  //Serial.println("****** SAUVEGARDE params.jso *******");
#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif
=======
    Serial.println(F("Failed to create file"));
    return;
  }

  Serial.println("****** SAUVEGARDE params.jso *******");
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
>>>>>>> 4cc2a87 (add files to be able to compile)

  // Clearing Buffer
  VarioTool::jsonDoc.clear();

<<<<<<< HEAD
  //Serial.println("****** GnuvarioE *******");
  VARIO_SDCARD_DEBUG_PRINTLN("****** GnuvarioE *******");
=======
  Serial.println("****** GnuvarioE *******");
>>>>>>> 4cc2a87 (add files to be able to compile)

  // Set the values in the document
  JsonObject GnuvarioE = VarioTool::jsonDoc.createNestedObject("gnuvarioe");
  GnuvarioE["version"] = PARAMS_VERSION;

  //*****    SYSTEME *****

<<<<<<< HEAD
  //Serial.println("****** Systeme *******");
  VARIO_SDCARD_DEBUG_PRINTLN("****** Systeme *******");

  JsonObject Systeme = VarioTool::jsonDoc.createNestedObject("systeme");

  Systeme["BT_ENABLE"] = varioData.getParam(PARAM_BT_ENABLE)->getValueBool() ? 1 : 0;
  Systeme["NO_RECORD"] = varioData.getParam(PARAM_NO_RECORD)->getValueBool() ? 1 : 0;
  Systeme["ALARM_SDCARD"] = varioData.getParam(PARAM_ALARM_SDCARD)->getValueBool() ? 1 : 0;
  Systeme["BEEP_GPSFIX"] = varioData.getParam(PARAM_BEEP_GPSFIX)->getValueBool() ? 1 : 0;
  Systeme["BEEP_FLYBEGIN"] = varioData.getParam(PARAM_BEEP_FLYBEGIN)->getValueBool() ? 1 : 0;
  Systeme["BEEP_VARIOBEGIN"] = varioData.getParam(PARAM_BEEP_VARIOBEGIN)->getValueBool() ? 1 : 0;
  Systeme["MUTE_VARIOBEGIN"] = varioData.getParam(PARAM_MUTE_VARIOBEGIN)->getValueBool() ? 1 : 0;
  // Systeme["COMPENSATION_TEMP"] = COMPENSATION_TEMP;
  // Systeme["COMPENSATION_GPSALTI"] = COMPENSATION_GPSALTI;
  Systeme["SLEEP_TIMEOUT_MINUTES"] = varioData.getParam(PARAM_SLEEP_TIMEOUT_MINUTES)->getValueUInt8();
  Systeme["SLEEP_THRESHOLD_CPS"] = varioData.getParam(PARAM_SLEEP_THRESHOLD_CPS)->getValueFloat();
  Systeme["MULTIDISPLAY_DURATION"] = varioData.getParam(PARAM_MULTIDISPLAY_DURATION)->getValueInt16();

  Systeme["DISPLAY_STAT_DURATION"] = varioData.getParam(PARAM_DISPLAY_STAT_DURATION)->getValueUInt8();
  Systeme["URL_UPDATE"] = varioData.getParam(PARAM_URL_UPDATE)->getValueChar();
  Systeme["LANGUAGE"] = varioData.getParam(PARAM_LANGUAGE)->getValueChar();

  //*****    GENERAL *****

//  Serial.println("****** General *******");
  VARIO_SDCARD_DEBUG_PRINTLN("****** General *******");

  JsonObject General = VarioTool::jsonDoc.createNestedObject("general");

  General["PILOT_NAME"] = varioData.getParam(PARAM_PILOT_NAME)->getValueChar();

  JsonObject General_GLIDER = General.createNestedObject("GLIDER");
  General_GLIDER["GLIDER_SELECT"] = varioData.getParam(PARAM_GLIDER_SELECT)->getValueUInt8();
  General_GLIDER["GLIDER_NAME1"] = varioData.getParam(PARAM_GLIDER_NAME1)->getValueChar();
  General_GLIDER["GLIDER_NAME2"] = varioData.getParam(PARAM_GLIDER_NAME2)->getValueChar();
  General_GLIDER["GLIDER_NAME3"] = varioData.getParam(PARAM_GLIDER_NAME3)->getValueChar();
  General_GLIDER["GLIDER_NAME4"] = varioData.getParam(PARAM_GLIDER_NAME4)->getValueChar();

  General["TIME_ZONE"] = varioData.getParam(PARAM_TIME_ZONE)->getValueInt8();

  //*****    VARIO *****

//  Serial.println("****** Vario *******");
  VARIO_SDCARD_DEBUG_PRINTLN("****** Vario *******");

  JsonObject Vario = VarioTool::jsonDoc.createNestedObject("vario");

  Vario["SINKING_THRESHOLD"] = varioData.getParam(PARAM_SINKING_THRESHOLD)->getValueFloat();
  Vario["CLIMBING_THRESHOLD"] = varioData.getParam(PARAM_CLIMBING_THRESHOLD)->getValueFloat();
  Vario["NEAR_CLIMBING_SENSITIVITY"] = varioData.getParam(PARAM_NEAR_CLIMBING_SENSITIVITY)->getValueFloat();
  Vario["ENABLE_NEAR_CLIMBING_ALARM"] = varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_ALARM)->getValueBool() ? 1 : 0;
  Vario["ENABLE_NEAR_CLIMBING_BEEP"] = varioData.getParam(PARAM_ENABLE_NEAR_CLIMBING_BEEP)->getValueBool() ? 1 : 0;
  Vario["DISPLAY_INTEGRATED_CLIMB_RATE"] = varioData.getParam(PARAM_DISPLAY_INTEGRATED_CLIMB_RATE)->getValueBool() ? 1 : 0;
  Vario["RATIO_CLIMB_RATE"] = varioData.getParam(PARAM_RATIO_CLIMB_RATE)->getValueUInt8();
  Vario["CLIMB_PERIOD_COUNT"] = varioData.getParam(PARAM_CLIMB_PERIOD_COUNT)->getValueInt8();
  Vario["SETTINGS_GLIDE_RATIO_PERIOD_COUNT"] = varioData.getParam(PARAM_SETTINGS_GLIDE_RATIO_PERIOD_COUNT)->getValueInt8();
  Vario["RATIO_MAX_VALUE"] = varioData.getParam(PARAM_RATIO_MAX_VALUE)->getValueFloat();
  Vario["RATIO_MIN_SPEED"] = varioData.getParam(PARAM_RATIO_MIN_SPEED)->getValueFloat();
  Vario["VARIOMETER_ENABLE_AGL"] = varioData.getParam(PARAM_VARIOMETER_ENABLE_AGL)->getValueBool() ? 1 : 0;
  Vario["SENT_LXNAV_SENTENCE"] = varioData.getParam(PARAM_SENT_LXNAV_SENTENCE)->getValueBool() ? 1 : 0;
  Vario["ACCELERATION_MEASURE_STANDARD_DEVIATION"] = varioData.getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->getValueFloat();
  Vario["VARIOMETER_INTEGRATED_CLIMB_RATE"] = varioData.getParam(PARAM_VARIOMETER_INTEGRATED_CLIMB_RATE)->getValueBool() ? 1 : 0;
  Vario["SETTINGS_VARIO_PERIOD_COUNT"] = varioData.getParam(PARAM_SETTINGS_VARIO_PERIOD_COUNT)->getValueUInt8();
  Vario["BLUETOOTH_SEND_CALIBRATED_ALTITUDE"] = varioData.getParam(PARAM_BLUETOOTH_SEND_CALIBRATED_ALTITUDE)->getValueBool() ? 1 : 0;

  //*****    Flight_Start *****

//  Serial.println("****** Flight start *******");
  VARIO_SDCARD_DEBUG_PRINTLN("****** Flight start *******");

  JsonObject FlightStart = VarioTool::jsonDoc.createNestedObject("flightstart");

  FlightStart["FLIGHT_START_MIN_TIMESTAMP"] = varioData.getParam(PARAM_FLIGHT_START_MIN_TIMESTAMP)->getValueUInt16();
  FlightStart["FLIGHT_START_VARIO_LOW_THRESHOLD"] = varioData.getParam(PARAM_FLIGHT_START_VARIO_LOW_THRESHOLD)->getValueFloat();
  FlightStart["FLIGHT_START_VARIO_HIGH_THRESHOLD"] = varioData.getParam(PARAM_FLIGHT_START_VARIO_HIGH_THRESHOLD)->getValueFloat();
  FlightStart["FLIGHT_START_MIN_SPEED"] = varioData.getParam(PARAM_FLIGHT_START_MIN_SPEED)->getValueFloat();
  FlightStart["RECORD_WHEN_FLIGHT_START"] = varioData.getParam(PARAM_RECORD_WHEN_FLIGHT_START)->getValueBool() ? 1 : 0;
=======
  Serial.println("****** Systeme *******");

  JsonObject Systeme = VarioTool::jsonDoc.createNestedObject("systeme");

  Systeme["BT_ENABLE"] = getParam(PARAM_BT_ENABLE)->getValueBool() ? 1 : 0;
  Systeme["NO_RECORD"] = getParam(PARAM_NO_RECORD)->getValueBool() ? 1 : 0;
  Systeme["ALARM_SDCARD"] = getParam(PARAM_ALARM_SDCARD)->getValueBool() ? 1 : 0;
  Systeme["BEEP_GPSFIX"] = getParam(PARAM_BEEP_GPSFIX)->getValueBool() ? 1 : 0;
  Systeme["BEEP_FLYBEGIN"] = getParam(PARAM_BEEP_FLYBEGIN)->getValueBool() ? 1 : 0;
  Systeme["BEEP_VARIOBEGIN"] = getParam(PARAM_BEEP_VARIOBEGIN)->getValueBool() ? 1 : 0;
  Systeme["MUTE_VARIOBEGIN"] = getParam(PARAM_MUTE_VARIOBEGIN)->getValueBool() ? 1 : 0;
  // Systeme["COMPENSATION_TEMP"] = COMPENSATION_TEMP;
  // Systeme["COMPENSATION_GPSALTI"] = COMPENSATION_GPSALTI;
  Systeme["SLEEP_TIMEOUT_MINUTES"] = getParam(PARAM_SLEEP_TIMEOUT_MINUTES)->getValueUInt8();
  Systeme["SLEEP_THRESHOLD_CPS"] = getParam(PARAM_SLEEP_THRESHOLD_CPS)->getValueFloat();
  Systeme["MULTIDISPLAY_DURATION"] = getParam(PARAM_MULTIDISPLAY_DURATION)->getValueInt16();

  Systeme["DISPLAY_STAT_DURATION"] = getParam(PARAM_DISPLAY_STAT_DURATION)->getValueUInt8();
  Systeme["URL_UPDATE"] = getParam(PARAM_URL_UPDATE)->getValueChar();
  Systeme["LANGUAGE"] = getParam(PARAM_LANGUAGE)->getValueChar();

  //*****    GENERAL *****

  Serial.println("****** General *******");

  JsonObject General = VarioTool::jsonDoc.createNestedObject("general");

  General["PILOT_NAME"] = getParam(PARAM_PILOT_NAME)->getValueChar();

  JsonObject General_GLIDER = General.createNestedObject("GLIDER");
  General_GLIDER["GLIDER_SELECT"] = getParam(PARAM_GLIDER_SELECT)->getValueUInt8();
  General_GLIDER["GLIDER_NAME1"] = getParam(PARAM_GLIDER_NAME1)->getValueChar();
  General_GLIDER["GLIDER_NAME2"] = getParam(PARAM_GLIDER_NAME2)->getValueChar();
  General_GLIDER["GLIDER_NAME3"] = getParam(PARAM_GLIDER_NAME3)->getValueChar();
  General_GLIDER["GLIDER_NAME4"] = getParam(PARAM_GLIDER_NAME4)->getValueChar();

  General["TIME_ZONE"] = getParam(PARAM_TIME_ZONE)->getValueInt8();

  //*****    VARIO *****

  Serial.println("****** Vario *******");

  JsonObject Vario = VarioTool::jsonDoc.createNestedObject("vario");

  Vario["SINKING_THRESHOLD"] = getParam(PARAM_SINKING_THRESHOLD)->getValueFloat();
  Vario["CLIMBING_THRESHOLD"] = getParam(PARAM_CLIMBING_THRESHOLD)->getValueFloat();
  Vario["NEAR_CLIMBING_SENSITIVITY"] = getParam(PARAM_NEAR_CLIMBING_SENSITIVITY)->getValueFloat();
  Vario["ENABLE_NEAR_CLIMBING_ALARM"] = getParam(PARAM_ENABLE_NEAR_CLIMBING_ALARM)->getValueBool() ? 1 : 0;
  Vario["ENABLE_NEAR_CLIMBING_BEEP"] = getParam(PARAM_ENABLE_NEAR_CLIMBING_BEEP)->getValueBool() ? 1 : 0;
  Vario["DISPLAY_INTEGRATED_CLIMB_RATE"] = getParam(PARAM_DISPLAY_INTEGRATED_CLIMB_RATE)->getValueBool() ? 1 : 0;
  Vario["RATIO_CLIMB_RATE"] = getParam(PARAM_RATIO_CLIMB_RATE)->getValueUInt8();
  Vario["CLIMB_PERIOD_COUNT"] = getParam(PARAM_CLIMB_PERIOD_COUNT)->getValueInt8();
  Vario["SETTINGS_GLIDE_RATIO_PERIOD_COUNT"] = getParam(PARAM_SETTINGS_GLIDE_RATIO_PERIOD_COUNT)->getValueInt8();
  Vario["RATIO_MAX_VALUE"] = getParam(PARAM_RATIO_MAX_VALUE)->getValueFloat();
  Vario["RATIO_MIN_SPEED"] = getParam(PARAM_RATIO_MIN_SPEED)->getValueFloat();
  Vario["VARIOMETER_ENABLE_AGL"] = getParam(PARAM_VARIOMETER_ENABLE_AGL)->getValueBool() ? 1 : 0;
  Vario["SENT_LXNAV_SENTENCE"] = getParam(PARAM_SENT_LXNAV_SENTENCE)->getValueBool() ? 1 : 0;
  Vario["ACCELERATION_MEASURE_STANDARD_DEVIATION"] = getParam(PARAM_ACCELERATION_MEASURE_STANDARD_DEVIATION)->getValueFloat();
  Vario["VARIOMETER_INTEGRATED_CLIMB_RATE"] = getParam(PARAM_VARIOMETER_INTEGRATED_CLIMB_RATE)->getValueBool() ? 1 : 0;
  Vario["SETTINGS_VARIO_PERIOD_COUNT"] = getParam(PARAM_SETTINGS_VARIO_PERIOD_COUNT)->getValueUInt8();
  Vario["BLUETOOTH_SEND_CALIBRATED_ALTITUDE"] = getParam(PARAM_BLUETOOTH_SEND_CALIBRATED_ALTITUDE)->getValueBool() ? 1 : 0;

  //*****    Flight_Start *****

  Serial.println("****** Flight start *******");

  JsonObject FlightStart = VarioTool::jsonDoc.createNestedObject("flightstart");

  FlightStart["FLIGHT_START_MIN_TIMESTAMP"] = getParam(PARAM_FLIGHT_START_MIN_TIMESTAMP)->getValueUInt16();
  FlightStart["FLIGHT_START_VARIO_LOW_THRESHOLD"] = getParam(PARAM_FLIGHT_START_VARIO_LOW_THRESHOLD)->getValueFloat();
  FlightStart["FLIGHT_START_VARIO_HIGH_THRESHOLD"] = getParam(PARAM_FLIGHT_START_VARIO_HIGH_THRESHOLD)->getValueFloat();
  FlightStart["FLIGHT_START_MIN_SPEED"] = getParam(PARAM_FLIGHT_START_MIN_SPEED)->getValueFloat();
  FlightStart["RECORD_WHEN_FLIGHT_START"] = getParam(PARAM_RECORD_WHEN_FLIGHT_START)->getValueBool() ? 1 : 0;
>>>>>>> 4cc2a87 (add files to be able to compile)

  // Serialize JSON to file
  if (serializeJson(VarioTool::jsonDoc, file) == 0)
  {
<<<<<<< HEAD
//    Serial.println(F("Failed to write to file"));
    VARIO_SDCARD_DEBUG_PRINTLN("Failed to write to file");
=======
    Serial.println(F("Failed to write to file"));
>>>>>>> 4cc2a87 (add files to be able to compile)
  }

  // Close the file
  file.close();
  // Clearing Buffer
  VarioTool::jsonDoc.clear();

<<<<<<< HEAD
#ifdef SDCARD_DEBUG
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
#endif  
=======
  Serial.printf_P(PSTR("free heap memory: %d\n"), ESP.getFreeHeap());
>>>>>>> 4cc2a87 (add files to be able to compile)
}

VarioSettings varioSettings;