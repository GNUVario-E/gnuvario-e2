#pragma once

#include <Arduino.h>
#include "GxEPD2_GFX.h"
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
    bool isactif;
    bool isborder;
    bool hasBeenModified = false;
    bool isFirstRefreshDone = false;
    DisplayZone lastDisplayZone = {0, 0, 0, 0};
    void storeLastDiplayZone(GxEPD2_GFX &_display, int16_t w, int16_t h);
    void clearLastDiplayZone(GxEPD2_GFX &_display);
    void drawborder(GxEPD2_GFX &_display);

public:
    Widget(int16_t topx, int16_t topy, int16_t width, int16_t height);
    void setPosition(int16_t _topx, int16_t _topy, int16_t _width, int16_t _height);
    virtual void addToBuffer(GxEPD2_GFX &_display) = 0;
    virtual bool isRefreshNeeded() = 0;
    void setForceRefresh();
    void setIsActif(bool _isactif);
    void setIsborder(bool _isborder);
    bool getIsActif();
    
};

extern fcdata_t fc;