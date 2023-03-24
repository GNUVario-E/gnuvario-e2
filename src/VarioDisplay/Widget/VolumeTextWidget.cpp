#include "VolumeTextWidget.h"
#include "VarioDebug/VarioDebug.h"

void VolumeTextWidget::addToBuffer(GxEPD2_GFX &_display)
{
    VARIO_PROG_DEBUG_PRINTLN("Refresh VolumeTextWidget");

    TextWidget::addToBuffer(_display);

    uint8_t margin = 4;
    uint8_t centerHeight = height / 2;
    uint8_t triangleHeight = 30;
    uint8_t triangleWidth = 15;
    int triangleColor;

    if (fc.getSoundIsInEditMode())
    {
        // draw triangles
        triangleColor = GxEPD_BLACK;
    }
    else
    {
        // clear triangles
        triangleColor = GxEPD_WHITE;
    }

    _display.fillTriangle(topx + margin,
                          topy + centerHeight,
                          topx + margin + triangleWidth,
                          topy + centerHeight - triangleHeight / 2,
                          topx + margin + triangleWidth,
                          topy + centerHeight + triangleHeight / 2,
                          triangleColor);

    _display.fillTriangle(topx + width - margin,
                          topy + centerHeight,
                          topx + width - margin - triangleWidth,
                          topy + centerHeight - triangleHeight / 2,
                          topx + width - margin - triangleWidth,
                          topy + centerHeight + triangleHeight / 2,
                          triangleColor);
}

bool VolumeTextWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    float volume = fc.getSoundVolume();
    if (volume != oldVolume)
    {
        sprintf(localText, "%.0f", volume);
        setText(localText);

        oldVolume = volume;

        VARIO_PROG_DEBUG_PRINTLN("Refresh needed for VolumeTextWidget");
        return true;
    }

    if (oldIsEditMode != fc.getSoundIsInEditMode())
    {
        oldIsEditMode = fc.getSoundIsInEditMode();
        VARIO_PROG_DEBUG_PRINTLN("Refresh needed for VolumeTextWidget");
        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
