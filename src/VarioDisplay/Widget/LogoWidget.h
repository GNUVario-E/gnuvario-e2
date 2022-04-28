#pragma once

#include "Widget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class LogoWidget : public Widget
{
private:
    int16_t imgWidth;
    int16_t imgHeight;

public:
    LogoWidget(VarioLanguage *_variolanguage, int16_t _topx, int16_t _topy, int16_t _width, int16_t _height, int16_t _imgWidth, int16_t _imgHeight) : Widget(_variolanguage, _topx, _topy, _width, _height)
    {
        varioLanguage = _variolanguage;
        imgWidth = _imgWidth;
        imgHeight = _imgHeight;
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
};