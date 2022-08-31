#pragma once

#include "TextWidget.h"

class LonWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    double oldLon = -999;

public:
    LonWidget(VarioLanguage *_variolanguage, uint8_t lonWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, lonWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_LONG;
        initAutoTitleFromIndex();
        setTextSize(2);
        setText("");
    }

    bool isRefreshNeeded(uint32_t lastDisplayTime);
};