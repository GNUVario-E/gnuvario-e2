#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class ToolbarWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;

    const unsigned char *batIcon = nullptr;
    const unsigned char *oldBatIcon = nullptr;

    const unsigned char *soundIcon = nullptr;
    const unsigned char *oldSoundIcon = nullptr;

    uint8_t batPct = 0;
    uint8_t oldBatPct = 0;

    uint8_t volume = 0;
    uint8_t oldVolume = 0;

    bool isMute = true;
    bool oldIsMute = true;

public:
    ToolbarWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
};