#pragma once

#include "Widget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class TextWidget : public Widget
{
private:
    uint8_t blinkFreq = 0;
    uint32_t lastDisplayTime = 0;
    uint8_t fontSize = 2;
    bool isText = false;
    int8_t marginTextRight = 0;
    uint8_t indexTxtFC = 0;

protected:
    char text[50];
    char *title = nullptr;

public:
    TextWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t topx, int16_t topy, int16_t width, int16_t height) : Widget(_variolanguage, altWidgetIndex, topx, topy, width, height)
    {
    }
    void addToBuffer(GxEPD2_GFX &_display);
    void setText(const char *text);
    void setTextSize(uint8_t size);
    void setTitle(char *title);
    void initAutoTitleFromIndex();
    void setBlinkFreq(uint8_t freq);
    void setMarginTextRight(int8_t margin);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
    void setIndexTxtFC(uint8_t _index);
};
