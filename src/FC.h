#pragma once

#include <Arduino.h>

struct power_data
{
    int capacite = 0;
    uint32_t capaciteTimestamp = 0;
    float tension = .0;
    uint32_t tensionTimestamp = 0;
};

struct sound_data
{
    bool isMute;
    float volume;
};

struct vario_data
{
    int16_t alti;
    uint32_t altiTimestamp = 0;
    float velocity;
    uint32_t velocityTimestamp = 0;
    int16_t bearing;
    char bearingTxt[3];
    uint32_t bearingTimestamp = 0;
};

struct wifi_data
{
    bool connected = false;
    char ssid[32];
    char ip[16];
};

struct text_data
{
    bool isText1 = false;
    char text1[50];
    bool isText2 = false;
    char text2[50];
    bool isText3 = false;
    char text3[50];
    bool isText4 = false;
    char text4[50];
    bool isText5 = false;
    char text5[50];
    bool isText6 = false;
    char text6[50];
    bool isText7 = false;
    char text7[50];
    bool isText8 = false;
    char text8[50];
};

struct gps_data
{
    double locLat = -999;
    double locLon = -999;
    uint32_t locTimestamp = 0;

    uint8_t dateDay;
    uint8_t dateMonth;
    uint8_t dateYear;
    uint32_t dateTimestamp = 0;

    uint8_t timeHour;
    uint8_t timeMinute;
    uint8_t timeSecond;
    uint32_t timeTimestamp = 0;

    double kmph;
    uint32_t kmphTimestamp = 0;

    int32_t headingDeg;
    char headingTxt[3];
    uint32_t headingDegTimestamp = 0;

    double altiMeters;
    uint32_t altiMetersTimestamp = 0;

    double hdop;
    uint32_t hdopTimestamp = 0;

    uint32_t satellitesCount;
    uint32_t satellitesTimestamp = 0;
};

struct agl_data
{
    int agl;
    uint32_t aglTimestamp = 0;
    int groundLvl = 0;
};

struct fcdata_t
{
    vario_data vario;
    power_data power;
    sound_data sound;
    int8_t time[3] = {0, 0, 0};
    int8_t flighttime[3] = {0, 0, 0};
    wifi_data wifi;
    text_data text;
    gps_data gps;
    agl_data agl;
};
