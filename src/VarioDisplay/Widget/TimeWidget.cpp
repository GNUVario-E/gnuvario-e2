#include "TimeWidget.h"

void TimeWidget::addToBuffer(GxEPD2_GFX &_display)
{
    TextWidget::addToBuffer(_display);

    // image unité
    uint8_t imgW = 16;
    uint8_t imgH = 24;
    _display.drawBitmap(topx + (width / 2) - (imgW / 2), topy + (height / 2), houricons, imgW, imgH, GxEPD_BLACK);
}

bool TimeWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    if (fc.getGpsTimeTimestamp() > getTimeout())
    {
        if (fc.getGpsTimeHour() != oldTime[0] || fc.getGpsTimeMinute() != oldTime[1] || fc.getGpsTimeSecond() != oldTime[2])
        {
            sprintf(localText, "%02d  %02d", fc.getGpsTimeHour(), fc.getGpsTimeMinute());
            setText(localText);
            oldTime[0] = fc.getGpsTimeHour();
            oldTime[1] = fc.getGpsTimeMinute();
            oldTime[2] = fc.getGpsTimeSecond();

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
