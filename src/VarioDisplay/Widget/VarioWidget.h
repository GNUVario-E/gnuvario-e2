#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class VarioWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    bool isText = false;
    uint32_t lastDisplayTime = 0;
    float oldVario = -999.0;

public:
    VarioWidget(int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(topx, topy, width, height)
    {
        TITLE_NAME_INDEX = TITRE_VARIO;
    }

    bool isRefreshNeeded();
};