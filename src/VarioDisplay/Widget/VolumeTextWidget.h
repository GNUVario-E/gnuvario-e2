#pragma once

#include "TextWidget.h"

class VolumeTextWidget : public TextWidget
{
private:
    char localText[20] = "";
    uint8_t blinkFreq = 0;
    float oldVolume = -99.0;
    bool oldIsEditMode = false;

public:
    VolumeTextWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height)
        : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
        TITLE_NAME_INDEX = 99;
        initAutoTitleFromIndex();
        setTextSize(4);
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
};
