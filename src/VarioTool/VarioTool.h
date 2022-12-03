#pragma once

#include <SD.h>
#include <ArduinoJson.h>

#define RL_MAX_CHARS 128

class VarioTool
{
public:
    static void appendChar(char *s, char c);
    static boolean readLines(File *file, char *line);
    static void deleteRecursive(const char *path);
    static StaticJsonDocument<20000> jsonDoc;
    static void bearingToOrdinal2c(char *bearingTxt, float bearing);
    static String getDeviceID();
};
