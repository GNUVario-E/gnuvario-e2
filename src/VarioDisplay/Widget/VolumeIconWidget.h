#pragma once

#include "Widget.h"
#include "VarioDisplay/bitmap/bitmap.h"

class VolumeIconWidget : public Widget
{
private:
    int16_t imgWidth = 32;
    int16_t imgHeight = 32;
    float oldVolume = -99.0;
    const unsigned char *volumeIcon = volume0icons;

    /**
     * Sets the icon corresponding to the given volume
     * @param volume
     * @return
     */
    void setIcon(const float volume);

public:
    VolumeIconWidget(VarioLanguage *_variolanguage, uint8_t altWidgetIndex, int16_t _topx, int16_t _topy, int16_t _width, int16_t _height)
    : Widget(_variolanguage, altWidgetIndex, _topx, _topy, _width, _height)
    {
        varioLanguage = _variolanguage;
    }
    void addToBuffer(GxEPD2_GFX &_display);
    bool isRefreshNeeded(uint32_t lastDisplayTime);
};
