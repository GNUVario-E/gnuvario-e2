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
    if (fc.getGpsKmphTimestamp() > getTimeout())
    {
        if (fc.getGpsKmph() != oldSpeed)
        {
            sprintf(localText, "%d ", (int)fc.getGpsKmph());
            setText(localText);
            oldSpeed = fc.getGpsKmph();

            return true;
        }
    }
    else
    {
        if (strcmp(empty, getText()))
        {
            setText("");
            return true;
        }
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}