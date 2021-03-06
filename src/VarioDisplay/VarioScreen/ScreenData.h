#pragma once

#include <Arduino.h>

struct S_LOGO
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint16_t iw;
    uint16_t ih;
    uint8_t altWidgetIndex;
};

struct S_TXT
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct S_ALT
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct S_VARIO
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct S_TOOLBAR
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct S_SPEED
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct S_BEARING
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct S_TIME
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct S_FLIGHTTIME
{
    bool isactif;
    bool isborder;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t altWidgetIndex;
};

struct ScreenData
{
    S_LOGO logo;
    S_TXT txt1;
    S_TXT txt2;
    S_TXT txt3;
    S_TXT txt4;
    S_TXT txt5;
    S_TXT txt6;
    S_TXT txt7;
    S_TXT txt8;
    S_ALT alt;
    S_VARIO vario;
    S_TOOLBAR toolbar;
    S_SPEED speed;
    S_BEARING bearing;
    S_TIME time;
    S_FLIGHTTIME flighttime;
};