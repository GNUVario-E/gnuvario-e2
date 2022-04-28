#include "LogoWidget.h"

void LogoWidget::addToBuffer(GxEPD2_GFX &_display)
{
    clearLastDiplayZone(_display);

    // center img
    _display.drawInvertedBitmap(topx + ((width - imgWidth) / 2), topy + ((height - imgHeight) / 2), logo_gnuvario, imgWidth, imgHeight, GxEPD_BLACK);
    storeLastDiplayZone(_display, width, height);

    if (isBorder)
    {
        drawborder(_display);
    }

    hasBeenModified = false;
}

bool LogoWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    return hasBeenModified;
}
