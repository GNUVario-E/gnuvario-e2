#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class WindWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    int8_t oldSpeed = 0;
    uint16_t oldHeading = 0;
    int8_t oldMeanSpeed = 0;

public:
    WindWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_WIND;
        initAutoTitleFromIndex();
        setTextSize(1);
    }
    bool isRefreshNeeded(uint32_t lastDisplayTime);
    uint32_t getTimeout();
};