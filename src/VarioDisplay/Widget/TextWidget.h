#pragma once

#include "Widget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class TextWidget : public Widget
{
private:
    uint8_t blinkFreq = 0;
    bool isText = false;
    uint32_t lastDisplayTime = 0;

protected:
    char *text;

public:
    TextWidget(int16_t topx, int16_t topy, int16_t width, int16_t height) : Widget(topx, topy, width, height)
    {
    }
    void addToBuffer(GxEPD2_GFX &_display);
    void setText(char *text);
    void setBlinkFreq(uint8_t freq);
    bool isRefreshNeeded();
};