#pragma once

#include "TextWidget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class ToolbarWidget : public TextWidget
{
private:
    char localText[20];
    uint8_t blinkFreq = 0;
    bool isText = false;
    uint32_t lastDisplayTime = 0;
    
    const unsigned char *batIcon = nullptr;
    const unsigned char *oldBatIcon = nullptr;

    const unsigned char *soundIcon = nullptr;
    const unsigned char *oldSoundIcon = nullptr;

public:
    ToolbarWidget(int16_t topx, int16_t topy, int16_t width, int16_t height) : TextWidget(topx, topy, width, height)
    {
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded();
};