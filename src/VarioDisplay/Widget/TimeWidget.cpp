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

    // title = variol
    if (fc.time[0] != oldTime[0] || fc.time[1] != oldTime[1] || fc.time[2] != oldTime[2])
    {
        sprintf(localText, "%02d %02d", fc.time[0], fc.time[1]);
        setText(localText);
        oldTime[0] = fc.time[0];
        oldTime[1] = fc.time[1];
        oldTime[2] = fc.time[2];

        return true;
    }
    else
    {
        sprintf(localText, "00 00");
        setText(localText);
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
