#pragma once

#include "Widget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class LogoWidget : public Widget
{
private:
    int16_t imgWidth;
    int16_t imgHeight;

public:
    LogoWidget(int16_t _topx, int16_t _topy, int16_t _width, int16_t _height, int16_t _imgWidth, int16_t _imgHeight) : Widget(_topx, _topy, _width, _height)
    {
        imgWidth = _imgWidth;
        imgHeight = _imgHeight;
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded();
};