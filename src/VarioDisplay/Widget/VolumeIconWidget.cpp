#include "VarioDebug/VarioDebug.h"
#include "VolumeIconWidget.h"

void VolumeIconWidget::addToBuffer(GxEPD2_GFX &_display)
{
    VARIO_PROG_DEBUG_PRINTLN("Refresh the volume widget");
    clearLastDiplayZone(_display);

    _display.drawInvertedBitmap(
            topx + ((width - imgWidth) / 2),
            topy + ((height - imgHeight) / 2),
            volumeIcon,
            imgWidth,
            imgHeight,
            GxEPD_BLACK
    );

    storeLastDiplayZone(_display, width, height);

    if (isBorder)
    {
        drawborder(_display);
    }

    hasBeenModified = false;
}

bool VolumeIconWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    const float volume = fc.getSoundVolume();

    if (oldVolume != volume) {
        setIcon(volume);

        oldVolume = volume;

        VARIO_PROG_DEBUG_PRINT("Refresh needed; volume: ");
        VARIO_PROG_DEBUG_PRINTLN(volume);

        return true;
    }

    return hasBeenModified;
}

void VolumeIconWidget::setIcon(const float volume) {
    if (volume == 0) {
        volumeIcon = volume0icons;
    } else if (volume < 5) {
        volumeIcon = volume1icons;
    } else if (volume < 8) {
        volumeIcon = volume2icons;
    } else {
        volumeIcon = volume3icons;
    }
}
