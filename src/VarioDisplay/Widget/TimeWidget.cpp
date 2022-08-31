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

    if (fc.gps.timeTimestamp > getTimeout())
    {
        if (fc.gps.timeHour != oldTime[0] || fc.gps.timeMinute != oldTime[1] || fc.gps.timeSecond != oldTime[2])
        {
            sprintf(localText, "%02d  %02d", fc.gps.timeHour, fc.gps.timeMinute);
            setText(localText);
            oldTime[0] = fc.gps.timeHour;
            oldTime[1] = fc.gps.timeMinute;
            oldTime[2] = fc.gps.timeSecond;

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
