#pragma once
#include <Arduino.h>
#include "VarioSD/VarioSD.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class SdDebug
{
private:
    const char logsDir[6] = "/logs";
    char filename[12];
    File currentFile;
    void init();
    void toSd(char *line, bool withNewLine = false);
    char fullPath[25];
    bool getNextFileName(char *filename);
    bool isCreated = false;

public:
    SdDebug();

    // size_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));

    void print(const __FlashStringHelper *);
    void print(const String &);
    void print(const char[]);
    void print(char);
    void print(unsigned char, int = DEC);
    void print(int, int = DEC);
    void print(unsigned int, int = DEC);
    void print(long, int = DEC);
    void print(unsigned long, int = DEC);
    void print(long long, int = DEC);
    void print(unsigned long long, int = DEC);
    void print(double, int = 2);
    void print(const Printable &);
    void print(struct tm *timeinfo, const char *format = NULL);

    void println(const __FlashStringHelper *);
    void println(const String &s);
    void println(const char[]);
    void println(char);
    void println(unsigned char, int = DEC);
    void println(int, int = DEC);
    void println(unsigned int, int = DEC);
    void println(long, int = DEC);
    void println(unsigned long, int = DEC);
    void println(long long, int = DEC);
    void println(unsigned long long, int = DEC);
    void println(double, int = 2);
    void println(const Printable &);
    void println(struct tm *timeinfo, const char *format = NULL);
    void println(void);
};
