#pragma once

#include "TextWidget.h"

class LatWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    double oldLat = -999;

public:
    LatWidget(VarioLanguage *_variolanguage, uint8_t latWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, latWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_LAT;
        initAutoTitleFromIndex();
        setTextSize(2);
        setText("");
    }

    bool isRefreshNeeded(uint32_t lastDisplayTime);
};