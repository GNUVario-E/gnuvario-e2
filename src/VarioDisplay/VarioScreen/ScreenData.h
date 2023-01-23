#pragma once

#include <Arduino.h>


struct S_WIDGET_DATA
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
    S_WIDGET_DATA logo;
    S_WIDGET_DATA txt1;
    S_WIDGET_DATA txt2;
    S_WIDGET_DATA txt3;
    S_WIDGET_DATA txt4;
    S_WIDGET_DATA txt5;
    S_WIDGET_DATA txt6;
    S_WIDGET_DATA txt7;
    S_WIDGET_DATA txt8;
    S_WIDGET_DATA alti;
    S_WIDGET_DATA vario;
    S_WIDGET_DATA toolbar;
    S_WIDGET_DATA speed;
    S_WIDGET_DATA bearing;
    S_WIDGET_DATA time;
    S_WIDGET_DATA flighttime;
    S_WIDGET_DATA lat;
    S_WIDGET_DATA lon;
    S_WIDGET_DATA altigps;
    S_WIDGET_DATA agl;
    S_WIDGET_DATA wind;
    S_WIDGET_DATA circle;
};