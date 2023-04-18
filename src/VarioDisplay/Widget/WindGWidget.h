#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"


class WindGWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    int8_t oldSpeed = 0;
    uint16_t oldHeading = 0;
    int8_t oldMeanSpeed = 0;
    void addToBuffer(GxEPD2_GFX &_display);
    void drawThickLine(GxEPD2_GFX &_display, int x1, int y1, int x2, int y2, int thickness, uint16_t color);

public:
    WindGWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_WIND;
        initAutoTitleFromIndex();
        setTextSize(1);
    }
    bool isRefreshNeeded(uint32_t lastDisplayTime);
    uint32_t getTimeout();
};
