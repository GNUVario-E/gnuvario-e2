#pragma once

#include "VarioDebug/ArduinoTrace.h"

#define VARIO_DEBUG // activate desactivate debuging

// #define SD_LOGGING // activate desactivate sd logging

#if defined(SD_LOGGING)
#include "VarioDebug/SdDebug.h"
extern SdDebug sdDebug;
#define SD_PRINT_LN(x) sdDebug.println(x)
#define SD_PRINT_LN2(x, y) sdDebug.println(x, y)
#define SD_PRINT(x) sdDebug.print(x)
#define SD_PRINT2(x, y)
#define SD_DUMP(x)
#define SD_DUMP2(x, y)
#define SD_TRACE()
#else
#define SD_PRINT_LN(x)
#define SD_PRINT_LN2(x, y)
#define SD_PRINT(x)
#define SD_PRINT2(x, y)
#define SD_DUMP(x)
#define SD_DUMP2(x, y)
#define SD_TRACE()
#endif

//              DEBUGING MODE
#define PROG_DEBUG // debug principal program
// #define BTN_DEBUG  //Boutons
// #define HARDWARE_DEBUG
// #define IMU_DEBUG // debug IMU
// #define CAL_DEBUG
// #define GPS_DEBUG
// #define NMEAPARSER_DEBUG
// #define SDCARD_DEBUG
// #define IGC_DEBUG
// #define DATA_DEBUG
// #define WIFI_DEBUG
// #define SOUND_DEBUG
// #define AGL_DEBUG
// #define SQL_DEBUG
#define MEMORY_DEBUG
// #define FSM_DEBUG
// #define PARAM_DEBUG
// #define PREFS_DEBUG
// #define WIND_DEBUG


#if !defined(VARIO_DEBUG) || !defined(PROG_DEBUG)
#define VARIO_PROG_DEBUG_PRINTLN(x)
#define VARIO_PROG_DEBUG_PRINTLN2(x, y)
#define VARIO_PROG_DEBUG_PRINT(x)
#define VARIO_PROG_DEBUG_PRINT2(x, y)
#define VARIO_PROG_DEBUG_DUMP(x)
#define VARIO_PROG_DEBUG_DUMP2(x, y)
#define VARIO_PROG_DEBUG_TRACE()
#else
#define VARIO_PROG_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_PROG_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_PROG_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_PROG_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_PROG_DEBUG_DUMP(x) DUMP(x)
#define VARIO_PROG_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_PROG_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(IMU_DEBUG)
#define VARIO_IMU_DEBUG_PRINTLN(x)
#define VARIO_IMU_DEBUG_PRINTLN2(x, y)
#define VARIO_IMU_DEBUG_PRINT(x)
#define VARIO_IMU_DEBUG_PRINT2(x, y)
#define VARIO_IMU_DEBUG_DUMP(x)
#define VARIO_IMU_DEBUG_DUMP2(x, y)
#define VARIO_IMU_DEBUG_TRACE()
#else
#define VARIO_IMU_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_IMU_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_IMU_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_IMU_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_IMU_DEBUG_DUMP(x) DUMP(x)
#define VARIO_IMU_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_IMU_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(BTN_DEBUG)
#define VARIO_BTN_DEBUG_PRINTLN(x)
#define VARIO_BTN_DEBUG_PRINTLN2(x, y)
#define VARIO_BTN_DEBUG_PRINT(x)
#define VARIO_BTN_DEBUG_PRINT2(x, y)
#define VARIO_BTN_DEBUG_DUMP(x)
#define VARIO_BTN_DEBUG_DUMP2(x, y)
#define VARIO_BTN_DEBUG_TRACE()
#else
#define VARIO_BTN_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_BTN_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_BTN_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_BTN_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_BTN_DEBUG_DUMP(x) DUMP(x)
#define VARIO_BTN_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_BTN_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(SDCARD_DEBUG)
#define VARIO_SDCARD_DEBUG_PRINTLN(x)
#define VARIO_SDCARD_DEBUG_PRINTLN2(x, y)
#define VARIO_SDCARD_DEBUG_PRINT(x)
#define VARIO_SDCARD_DEBUG_PRINT2(x, y)
#define VARIO_SDCARD_DEBUG_DUMP(x)
#define VARIO_SDCARD_DEBUG_DUMP2(x, y)
#define VARIO_SDCARD_DEBUG_TRACE()
#else
#define VARIO_SDCARD_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_SDCARD_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_SDCARD_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_SDCARD_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_SDCARD_DEBUG_DUMP(x) DUMP(x)
#define VARIO_SDCARD_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_SDCARD_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(SOUND_DEBUG)
#define VARIO_SOUND_DEBUG_PRINTLN(x)
#define VARIO_SOUND_DEBUG_PRINTLN2(x, y)
#define VARIO_SOUND_DEBUG_PRINT(x)
#define VARIO_SOUND_DEBUG_PRINT2(x, y)
#define VARIO_SOUND_DEBUG_DUMP(x)
#define VARIO_SOUND_DEBUG_DUMP2(x, y)
#define VARIO_SOUND_DEBUG_TRACE()
#else
#define VARIO_SOUND_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_SOUND_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_SOUND_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_SOUND_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_SOUND_DEBUG_DUMP(x) DUMP(x)
#define VARIO_SOUND_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_SOUND_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(SQL_DEBUG)
#define VARIO_SQL_DEBUG_PRINTLN(x)
#define VARIO_SQL_DEBUG_PRINTLN2(x, y)
#define VARIO_SQL_DEBUG_PRINT(x)
#define VARIO_SQL_DEBUG_PRINT2(x, y)
#define VARIO_SQL_DEBUG_DUMP(x)
#define VARIO_SQL_DEBUG_DUMP2(x, y)
#define VARIO_SQL_DEBUG_TRACE()
#else
#define VARIO_SQL_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_SQL_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_SQL_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_SQL_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_SQL_DEBUG_DUMP(x) DUMP(x)
#define VARIO_SQL_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_SQL_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(MEMORY_DEBUG)
#define VARIO_MEMORY_DEBUG_PRINTLN(x)
#define VARIO_MEMORY_DEBUG_PRINTLN2(x, y)
#define VARIO_MEMORY_DEBUG_PRINT(x)
#define VARIO_MEMORY_DEBUG_PRINT2(x, y)
#define VARIO_MEMORY_DEBUG_DUMP(x)
#define VARIO_MEMORY_DEBUG_DUMP2(x, y)
#define VARIO_MEMORY_DEBUG_TRACE()
#else
#define VARIO_MEMORY_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_MEMORY_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_MEMORY_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_MEMORY_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_MEMORY_DEBUG_DUMP(x) DUMP(x)
#define VARIO_MEMORY_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_MEMORY_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(CAL_DEBUG)
#define VARIO_CAL_DEBUG_PRINTLN(x)
#define VARIO_CAL_DEBUG_PRINTLN2(x, y)
#define VARIO_CAL_DEBUG_PRINT(x)
#define VARIO_CAL_DEBUG_PRINT2(x, y)
#define VARIO_CAL_DEBUG_DUMP(x)
#define VARIO_CAL_DEBUG_DUMP2(x, y)
#define VARIO_CAL_DEBUG_TRACE()
#else
#define VARIO_CAL_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_CAL_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_CAL_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_CAL_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_CAL_DEBUG_DUMP(x) DUMP(x)
#define VARIO_CAL_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_CAL_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(WIFI_DEBUG)
#define VARIO_WIFI_DEBUG_PRINTLN(x)
#define VARIO_WIFI_DEBUG_PRINTLN2(x, y)
#define VARIO_WIFI_DEBUG_PRINT(x)
#define VARIO_WIFI_DEBUG_PRINT2(x, y)
#define VARIO_WIFI_DEBUG_DUMP(x)
#define VARIO_WIFI_DEBUG_DUMP2(x, y)
#define VARIO_WIFI_DEBUG_TRACE()
#else
#define VARIO_WIFI_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_WIFI_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_WIFI_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_WIFI_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_WIFI_DEBUG_DUMP(x) DUMP(x)
#define VARIO_WIFI_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_WIFI_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(DATA_DEBUG)
#define VARIO_DATA_DEBUG_PRINTLN(x)
#define VARIO_DATA_DEBUG_PRINTLN2(x, y)
#define VARIO_DATA_DEBUG_PRINT(x)
#define VARIO_DATA_DEBUG_PRINT2(x, y)
#define VARIO_DATA_DEBUG_DUMP(x)
#define VARIO_DATA_DEBUG_DUMP2(x, y)
#define VARIO_DATA_DEBUG_TRACE()
#else
#define VARIO_DATA_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_DATA_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_DATA_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_DATA_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_DATA_DEBUG_DUMP(x) DUMP(x)
#define VARIO_DATA_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_DATA_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(GPS_DEBUG)
#define VARIO_GPS_DEBUG_PRINTLN(x)
#define VARIO_GPS_DEBUG_PRINTLN2(x, y)
#define VARIO_GPS_DEBUG_PRINT(x)
#define VARIO_GPS_DEBUG_PRINT2(x, y)
#define VARIO_GPS_DEBUG_DUMP(x)
#define VARIO_GPS_DEBUG_DUMP2(x, y)
#define VARIO_GPS_DEBUG_TRACE()
#else
#define VARIO_GPS_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_GPS_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_GPS_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_GPS_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_GPS_DEBUG_DUMP(x) DUMP(x)
#define VARIO_GPS_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_GPS_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(NMEAPARSER_DEBUG)
#define VARIO_NMEAPARSER_DEBUG_PRINTLN(x)
#define VARIO_NMEAPARSER_DEBUG_PRINTLN2(x, y)
#define VARIO_NMEAPARSER_DEBUG_PRINT(x)
#define VARIO_NMEAPARSER_DEBUG_PRINT2(x, y)
#define VARIO_NMEAPARSER_DEBUG_DUMP(x)
#define VARIO_NMEAPARSER_DEBUG_DUMP2(x, y)
#define VARIO_NMEAPARSER_DEBUG_TRACE()
#else
#define VARIO_NMEAPARSER_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_NMEAPARSER_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_NMEAPARSER_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_NMEAPARSER_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_NMEAPARSER_DEBUG_DUMP(x) DUMP(x)
#define VARIO_NMEAPARSER_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_NMEAPARSER_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(FSM_DEBUG)
#define VARIO_FSM_DEBUG_PRINTLN(x)
#define VARIO_FSM_DEBUG_PRINTLN2(x, y)
#define VARIO_FSM_DEBUG_PRINT(x)
#define VARIO_FSM_DEBUG_PRINT2(x, y)
#define VARIO_FSM_DEBUG_DUMP(x)
#define VARIO_FSM_DEBUG_DUMP2(x, y)
#define VARIO_FSM_DEBUG_TRACE()
#else
#define VARIO_FSM_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x)
#define VARIO_FSM_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_FSM_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_FSM_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_FSM_DEBUG_DUMP(x) DUMP(x)
#define VARIO_FSM_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_FSM_DEBUG_TRACE() TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(AGL_DEBUG)
#define VARIO_AGL_DEBUG_PRINTLN(x)
#define VARIO_AGL_DEBUG_PRINTLN2(x, y)
#define VARIO_AGL_DEBUG_PRINT(x)
#define VARIO_AGL_DEBUG_PRINT2(x, y)
#define VARIO_AGL_DEBUG_DUMP(x)
#define VARIO_AGL_DEBUG_DUMP2(x, y)
#define VARIO_AGL_DEBUG_TRACE()
#else
#define VARIO_AGL_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x);
#define VARIO_AGL_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_AGL_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_AGL_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_AGL_DEBUG_DUMP(x) DUMP(x)
#define VARIO_AGL_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_AGL_DEBUG_TRACE() TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(IGC_DEBUG)
#define VARIO_IGC_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x);
#define VARIO_IGC_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_IGC_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_IGC_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_IGC_DEBUG_DUMP(x) DUMP(x)
#define VARIO_IGC_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_IGC_DEBUG_TRACE() TRACE()
#else
#define VARIO_IGC_DEBUG_PRINTLN(x)
#define VARIO_IGC_DEBUG_PRINTLN2(x, y)
#define VARIO_IGC_DEBUG_PRINT(x)
#define VARIO_IGC_DEBUG_PRINT2(x, y)
#define VARIO_IGC_DEBUG_DUMP(x)
#define VARIO_IGC_DEBUG_DUMP2(x, y)
#define VARIO_IGC_DEBUG_TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(PARAM_DEBUG)
#define VARIO_PARAM_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x);
#define VARIO_PARAM_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_PARAM_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_PARAM_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_PARAM_DEBUG_DUMP(x) DUMP(x)
#define VARIO_PARAM_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_PARAM_DEBUG_TRACE() TRACE()
#else
#define VARIO_PARAM_DEBUG_PRINTLN(x)
#define VARIO_PARAM_DEBUG_PRINTLN2(x, y)
#define VARIO_PARAM_DEBUG_PRINT(x)
#define VARIO_PARAM_DEBUG_PRINT2(x, y)
#define VARIO_PARAM_DEBUG_DUMP(x)
#define VARIO_PARAM_DEBUG_DUMP2(x, y)
#define VARIO_PARAM_DEBUG_TRACE()
#endif


#if defined(VARIO_DEBUG) && defined(PREFS_DEBUG)
#define VARIO_PREFS_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x);
#define VARIO_PREFS_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_PREFS_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_PREFS_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_PREFS_DEBUG_DUMP(x) DUMP(x)
#define VARIO_PREFS_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_PREFS_DEBUG_TRACE() TRACE()
#else
#define VARIO_PREFS_DEBUG_PRINTLN(x)
#define VARIO_PREFS_DEBUG_PRINTLN2(x, y)
#define VARIO_PREFS_DEBUG_PRINT(x)
#define VARIO_PREFS_DEBUG_PRINT2(x, y)
#define VARIO_PREFS_DEBUG_DUMP(x)
#define VARIO_PREFS_DEBUG_DUMP2(x, y)
#define VARIO_PREFS_DEBUG_TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(WIND_DEBUG)
#define VARIO_WIND_DEBUG_PRINTLN(x) Serial.println(x); SD_PRINT_LN(x);
#define VARIO_WIND_DEBUG_PRINTLN2(x, y) Serial.println(x, y); SD_PRINT_LN2(x, y)
#define VARIO_WIND_DEBUG_PRINT(x) Serial.print(x); SD_PRINT(x)
#define VARIO_WIND_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_WIND_DEBUG_DUMP(x) DUMP(x)
#define VARIO_WIND_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_WIND_DEBUG_TRACE() TRACE()
#else
#define VARIO_WIND_DEBUG_PRINTLN(x)
#define VARIO_WIND_DEBUG_PRINTLN2(x, y)
#define VARIO_WIND_DEBUG_PRINT(x)
#define VARIO_WIND_DEBUG_PRINT2(x, y)
#define VARIO_WIND_DEBUG_DUMP(x)
#define VARIO_WIND_DEBUG_DUMP2(x, y)
#define VARIO_WIND_DEBUG_TRACE()
#endif