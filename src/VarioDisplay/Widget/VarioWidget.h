#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class VarioWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    float oldVario = -999.0;

public:
    VarioWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_VARIO;
        initAutoTitleFromIndex();
        setTextSize(4);
        setMarginTextRight(16);
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
};