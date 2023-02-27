#include "VolumeTextWidget.h"
#include "VarioDebug/VarioDebug.h"

void VolumeTextWidget::addToBuffer(GxEPD2_GFX &_display)
{
    VARIO_PROG_DEBUG_PRINTLN("Refresh VolumeTextWidget");
    TextWidget::addToBuffer(_display);
    _display.fillTriangle(0, topy + height / 4, 15, topy, 15, topy + height / 2, GxEPD_BLACK);
    _display.fillTriangle(width, topy + height / 4, width - 15, topy, width - 15, topy + height / 2, GxEPD_BLACK);
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

    return false;
}
