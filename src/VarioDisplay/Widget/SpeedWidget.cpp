#include "SpeedWidget.h"

void SpeedWidget::addToBuffer(GxEPD2_GFX &_display)
{
    TextWidget::addToBuffer(_display);

    // image unité
    uint8_t imgSize = 24;
    _display.drawBitmap(topx + width - imgSize - 2, topy + (height / 2) - (imgSize / 2), kmhicons, imgSize, imgSize, GxEPD_BLACK);
}

bool SpeedWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.gps.kmphTimestamp > getTimeout())
    {
        if (fc.gps.kmph != oldSpeed)
        {
            sprintf(localText, "%d.%02d ", (int)fc.gps.kmph, (int)(fabsf(fc.gps.kmph) * 100) % 100);
            setText(localText);
            oldSpeed = fc.gps.kmph;

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}