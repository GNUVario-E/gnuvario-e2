#pragma once

#include "VarioDebug/ArduinoTrace.h"

#define VARIO_DEBUG // activate desactivate debuging

//              DEBUGING MODE
#define PROG_DEBUG // debug principal program
//#define BTN_DEBUG  //Boutons
//#define HARDWARE_DEBUG
//#define IMU_DEBUG			  //debug IMU
//#define CAL_DEBUG
//#define I2CDEV_SERIAL_DEBUG   //debug I2Cdev
//#define DEBUG_SERIAL_NMEA_1
//#define SCREEN_DEBUG
//#define SCREEN_DEBUG2
//#define GPS_DEBUG
//#define BUTTON_DEBUG
//#define TONEDAC_DEBUG
//#define MS5611_DEBUG
//#define KALMAN_DEBUG
//#define ACCEL_DEBUG
//#define EEPROM_DEBUG
//#define NMEAPARSER_DEBUG
#define SDCARD_DEBUG
//#define IGC_DEBUG
//#define DATA_DEBUG
//#define BT_DEBUG
#define WIFI_DEBUG
//#define SOUND_DEBUG
//#define AGL_DEBUG
//#define SQL_DEBUG
//#define BEARING_DEBUG
//#define TWOWIRESCH_DEBUG
//#define POWER_DEBUG
//#define MEMORY_DEBUG
// #define FSM_DEBUG

#if !defined(VARIO_DEBUG) || !defined(PROG_DEBUG)
#define VARIO_PROG_DEBUG_PRINTLN(x)
#define VARIO_PROG_DEBUG_PRINTLN2(x, y)
#define VARIO_PROG_DEBUG_PRINT(x)
#define VARIO_PROG_DEBUG_PRINT2(x, y)
#define VARIO_PROG_DEBUG_DUMP(x)
#define VARIO_PROG_DEBUG_DUMP2(x, y)
#define VARIO_PROG_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(BTN_DEBUG)
#define VARIO_BTN_DEBUG_PRINTLN(x)
#define VARIO_BTN_DEBUG_PRINTLN2(x, y)
#define VARIO_BTN_DEBUG_PRINT(x)
#define VARIO_BTN_DEBUG_PRINT2(x, y)
#define VARIO_BTN_DEBUG_DUMP(x)
#define VARIO_BTN_DEBUG_DUMP2(x, y)
#define VARIO_BTN_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(SDCARD_DEBUG)
#define VARIO_SDCARD_DEBUG_PRINTLN(x)
#define VARIO_SDCARD_DEBUG_PRINTLN2(x, y)
#define VARIO_SDCARD_DEBUG_PRINT(x)
#define VARIO_SDCARD_DEBUG_PRINT2(x, y)
#define VARIO_SDCARD_DEBUG_DUMP(x)
#define VARIO_SDCARD_DEBUG_DUMP2(x, y)
#define VARIO_SDCARD_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(SOUND_DEBUG)
#define VARIO_SOUND_DEBUG_PRINTLN(x)
#define VARIO_SOUND_DEBUG_PRINTLN2(x, y)
#define VARIO_SOUND_DEBUG_PRINT(x)
#define VARIO_SOUND_DEBUG_PRINT2(x, y)
#define VARIO_SOUND_DEBUG_DUMP(x)
#define VARIO_SOUND_DEBUG_DUMP2(x, y)
#define VARIO_SOUND_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(SQL_DEBUG)
#define VARIO_SQL_DEBUG_PRINTLN(x)
#define VARIO_SQL_DEBUG_PRINTLN2(x, y)
#define VARIO_SQL_DEBUG_PRINT(x)
#define VARIO_SQL_DEBUG_PRINT2(x, y)
#define VARIO_SQL_DEBUG_DUMP(x)
#define VARIO_SQL_DEBUG_DUMP2(x, y)
#define VARIO_SQL_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(MEMORY_DEBUG)
#define VARIO_MEMORY_DEBUG_PRINTLN(x)
#define VARIO_MEMORY_DEBUG_PRINTLN2(x, y)
#define VARIO_MEMORY_DEBUG_PRINT(x)
#define VARIO_MEMORY_DEBUG_PRINT2(x, y)
#define VARIO_MEMORY_DEBUG_DUMP(x)
#define VARIO_MEMORY_DEBUG_DUMP2(x, y)
#define VARIO_MEMORY_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(CAL_DEBUG)
#define VARIO_CAL_DEBUG_PRINTLN(x)
#define VARIO_CAL_DEBUG_PRINTLN2(x, y)
#define VARIO_CAL_DEBUG_PRINT(x)
#define VARIO_CAL_DEBUG_PRINT2(x, y)
#define VARIO_CAL_DEBUG_DUMP(x)
#define VARIO_CAL_DEBUG_DUMP2(x, y)
#define VARIO_CAL_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(WIFI_DEBUG)
#define VARIO_WIFI_DEBUG_PRINTLN(x)
#define VARIO_WIFI_DEBUG_PRINTLN2(x, y)
#define VARIO_WIFI_DEBUG_PRINT(x)
#define VARIO_WIFI_DEBUG_PRINT2(x, y)
#define VARIO_WIFI_DEBUG_DUMP(x)
#define VARIO_WIFI_DEBUG_DUMP2(x, y)
#define VARIO_WIFI_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(DATA_DEBUG)
#define VARIO_DATA_DEBUG_PRINTLN(x)
#define VARIO_DATA_DEBUG_PRINTLN2(x, y)
#define VARIO_DATA_DEBUG_PRINT(x)
#define VARIO_DATA_DEBUG_PRINT2(x, y)
#define VARIO_DATA_DEBUG_DUMP(x)
#define VARIO_DATA_DEBUG_DUMP2(x, y)
#define VARIO_DATA_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(GPS_DEBUG)
#define VARIO_GPS_DEBUG_PRINTLN(x)
#define VARIO_GPS_DEBUG_PRINTLN2(x, y)
#define VARIO_GPS_DEBUG_PRINT(x)
#define VARIO_GPS_DEBUG_PRINT2(x, y)
#define VARIO_GPS_DEBUG_DUMP(x)
#define VARIO_GPS_DEBUG_DUMP2(x, y)
#define VARIO_GPS_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(NMEAPARSER_DEBUG)
#define VARIO_NMEAPARSER_DEBUG_PRINTLN(x)
#define VARIO_NMEAPARSER_DEBUG_PRINTLN2(x, y)
#define VARIO_NMEAPARSER_DEBUG_PRINT(x)
#define VARIO_NMEAPARSER_DEBUG_PRINT2(x, y)
#define VARIO_NMEAPARSER_DEBUG_DUMP(x)
#define VARIO_NMEAPARSER_DEBUG_DUMP2(x, y)
#define VARIO_NMEAPARSER_DEBUG_TRACE()
#endif

#if !defined(VARIO_DEBUG) || !defined(FSM_DEBUG)
#define VARIO_FSM_DEBUG_PRINTLN(x)
#define VARIO_FSM_DEBUG_PRINTLN2(x, y)
#define VARIO_FSM_DEBUG_PRINT(x)
#define VARIO_FSM_DEBUG_PRINT2(x, y)
#define VARIO_FSM_DEBUG_DUMP(x)
#define VARIO_FSM_DEBUG_DUMP2(x, y)
#define VARIO_FSM_DEBUG_TRACE()
#endif
/*************************************************/

#if defined(VARIO_DEBUG) && defined(PROG_DEBUG)
#define VARIO_PROG_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_PROG_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_PROG_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_PROG_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_PROG_DEBUG_DUMP(x) DUMP(x)
#define VARIO_PROG_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_PROG_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(BTN_DEBUG)
#define VARIO_BTN_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_BTN_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_BTN_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_BTN_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_BTN_DEBUG_DUMP(x) DUMP(x)
#define VARIO_BTN_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_BTN_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(SDCARD_DEBUG)
#define VARIO_SDCARD_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_SDCARD_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_SDCARD_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_SDCARD_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_SDCARD_DEBUG_DUMP(x) DUMP(x)
#define VARIO_SDCARD_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_SDCARD_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(SOUND_DEBUG)
#define VARIO_SOUND_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_SOUND_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_SOUND_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_SOUND_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_SOUND_DEBUG_DUMP(x) DUMP(x)
#define VARIO_SOUND_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_SOUND_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(SQL_DEBUG)
#define VARIO_SQL_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_SQL_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_SQL_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_SQL_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_SQL_DEBUG_DUMP(x) DUMP(x)
#define VARIO_SQL_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_SQL_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(MEMORY_DEBUG)
#define VARIO_MEMORY_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_MEMORY_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_MEMORY_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_MEMORY_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_MEMORY_DEBUG_DUMP(x) DUMP(x)
#define VARIO_MEMORY_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_MEMORY_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(CAL_DEBUG)
#define VARIO_CAL_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_CAL_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_CAL_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_CAL_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_CAL_DEBUG_DUMP(x) DUMP(x)
#define VARIO_CAL_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_CAL_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(WIFI_DEBUG)
#define VARIO_WIFI_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_WIFI_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_WIFI_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_WIFI_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_WIFI_DEBUG_DUMP(x) DUMP(x)
#define VARIO_WIFI_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_WIFI_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(DATA_DEBUG)
#define VARIO_DATA_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_DATA_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_DATA_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_DATA_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_DATA_DEBUG_DUMP(x) DUMP(x)
#define VARIO_DATA_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_DATA_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(GPS_DEBUG)
#define VARIO_GPS_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_GPS_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_GPS_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_GPS_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_GPS_DEBUG_DUMP(x) DUMP(x)
#define VARIO_GPS_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_GPS_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(NMEAPARSER_DEBUG)
#define VARIO_NMEAPARSER_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_NMEAPARSER_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_NMEAPARSER_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_NMEAPARSER_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_NMEAPARSER_DEBUG_DUMP(x) DUMP(x)
#define VARIO_NMEAPARSER_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_NMEAPARSER_DEBUG_TRACE(x) TRACE()
#endif

#if defined(VARIO_DEBUG) && defined(FSM_DEBUG)
#define VARIO_FSM_DEBUG_PRINTLN(x) Serial.println(x)
#define VARIO_FSM_DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define VARIO_FSM_DEBUG_PRINT(x) Serial.print(x)
#define VARIO_FSM_DEBUG_PRINT2(x, y) Serial.print(x, y)
#define VARIO_FSM_DEBUG_DUMP(x) DUMP(x)
#define VARIO_FSM_DEBUG_DUMP2(x, y) DUMP2(x, y)
#define VARIO_FSM_DEBUG_TRACE(x) TRACE()
#endif