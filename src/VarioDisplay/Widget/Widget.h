#pragma once

// font generator
// https://rop.nl/truetype2gfx/

#include <Arduino.h>
#include "GxEPD2_GFX.h"
#include <glcdfont.c>
#include "VarioLanguage/VarioLanguage.h"
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include "VarioDisplay/fonts/FreeSansBold8pt7b.h"
#include "VarioDisplay/fonts/NotoSans6pt7b.h"
#include "VarioDisplay/fonts/Gnuvarioe18pt7b.h"
#include "VarioDisplay/fonts/gnuvarioe26pt7b.h"
#include "VarioDisplay/fonts/FreeSansBold32pt7b.h"
#include "FC.h"

struct DisplayZone
{
    int16_t topx;
    int16_t topy;
    int16_t width;
    int16_t height;
};

// template <typename Self, typename Finished>
class Widget
{
protected:
    int16_t topx;
    int16_t topy;
    int16_t width;
    int16_t height;

    bool isActif;
    bool isBorder;
    bool forceClear = false;

    bool hasBeenModified = false;

    DisplayZone lastDisplayZone = {0, 0, 0, 0};

    void storeLastDiplayZone(GxEPD2_GFX &_display, int16_t w, int16_t h);
    void clearLastDiplayZone(GxEPD2_GFX &_display);

    void drawborder(GxEPD2_GFX &_display);
    VarioLanguage *varioLanguage;
    uint8_t TITLE_NAME_INDEX = 99;
    uint8_t altWidgetIndex = 99;

public:
    Widget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height);
    void setPosition(int16_t _topx, int16_t _topy, int16_t _width, int16_t _height);
    virtual void addToBuffer(GxEPD2_GFX &_display) = 0;
    virtual bool isRefreshNeeded(uint32_t lastDisplayTime) = 0;
    void setForceRefresh();
    void setForceClearZone();
    void setIsActif(bool _isactif);
    void setIsBorder(bool _isborder);
    bool getIsActif();
    uint8_t getAltWidgetIndex();
    uint32_t getTimeout();
};

extern FC fc;