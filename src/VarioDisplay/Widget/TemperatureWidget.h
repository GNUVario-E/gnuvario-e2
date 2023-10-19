#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class TemperatureWidget : public TextWidget
{
private:
    char localText[10];
    uint8_t blinkFreq = 0;
    int16_t oldTemperature = -999;

public:
    TemperatureWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_TEMP;
        initAutoTitleFromIndex();
        setTextSize(4);
    }

    bool isRefreshNeeded(uint32_t lastDisplayTime);
};