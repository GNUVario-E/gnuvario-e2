#pragma once

#include "Widget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class LogoWidget : public Widget
{
private:
    int16_t imgWidth;
    int16_t imgHeight;

public:
    LogoWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t _topx, int16_t _topy, int16_t _width, int16_t _height) : Widget(_variolanguage, altWidgetIndex, _topx, _topy, _width, _height)
    {
        varioLanguage = _variolanguage;
        imgWidth = 94;
        imgHeight = 74;
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
};