#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class FlightTimeWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    int8_t oldTime[3] = {0, 0, 0};

public:
    FlightTimeWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_TDV;
        initAutoTitleFromIndex();
        setTextSize(4);
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
};