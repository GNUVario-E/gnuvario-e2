#pragma once

#include <Arduino.h>

struct power_data
{
    int capacite = 0;
    float tension = .0;
};

struct sound_data
{
    bool isMute;
    float volume;
};

struct vario_data
{
    int16_t alti;
    float velocity;
    int16_t bearing;
    char bearingTxt[3];
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

struct fcdata_t
{
    double_t speed;
    vario_data vario;
    bool isFixed;
    power_data power;
    sound_data sound;
    int8_t time[3] = {0, 0, 0};
    int8_t flighttime[3] = {0, 0, 0};
    wifi_data wifi;
    text_data text;
};
