#pragma once

#include <SD.h>
#include <ArduinoJson.h>

#define RL_MAX_CHARS 128
#define JSON_SIZE 10000

class VarioTool
{
public:
    static void appendChar(char *s, char c);
    static boolean readLines(File *file, char *line);
    static void deleteRecursive(const char *path);
    static StaticJsonDocument<JSON_SIZE> jsonDoc;
    static void bearingToOrdinal2c(char *bearingTxt, float bearing);
    static String getDeviceID();
    static double convertIGCDDMMmmmToDecimal(int32_t valeur);
    static double convertGPSnmeaToDecimalDegrees(double nmea);
    static double calculateDistance(double lat1Rad, double lon1Rad, double lat2Rad, double lon2Rad);
};
