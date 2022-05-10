#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class AltWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    int16_t oldAlt = -999;

public:
    AltWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_ALTI;
        initAutoTitleFromIndex();
        setTextSize(4);
    }

    bool isRefreshNeeded(uint32_t lastDisplayTime);
};