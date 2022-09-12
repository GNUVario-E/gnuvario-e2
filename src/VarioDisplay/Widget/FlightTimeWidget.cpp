#include "FlightTimeWidget.h"

void FlightTimeWidget::addToBuffer(GxEPD2_GFX &_display)
{
    TextWidget::addToBuffer(_display);

    // image unité
    uint8_t imgW = 16;
    uint8_t imgH = 24;
    _display.drawBitmap(topx + (width / 2) - (imgW / 2), topy + (height / 2), hourdoticons, imgW, imgH, GxEPD_BLACK);
}

bool FlightTimeWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    uint8_t h = fc.getFlightTimeDurationHour();
    uint8_t m = fc.getFlightTimeDurationMinute();
    uint8_t s = fc.getFlightTimeDurationSecond();

    if (fc.getIsFlightStart() && fc.getGpsIsFixed() && fc.getGpsIsFixedTimestamp() > getTimeout())
    {
        if (h >= 1 && (h != oldTime[0] || m != oldTime[1]))
        {
            // more than one hour of flight hh:mm
            sprintf(localText, "%02d %02d", h, m);
            setText(localText);
            oldTime[0] = h;
            oldTime[1] = m;
            oldTime[2] = s;

            return true;
        }
        else if (h < 1 && (fc.getFlightTimeDurationMinute() != oldTime[1] || s != oldTime[2]))
        {
            // less than one hour of flight mm:ss
            sprintf(localText, "%02d %02d", m, s);
            setText(localText);
            oldTime[0] = 0;
            oldTime[1] = m;
            oldTime[2] = s;

            return true;
        }
    }
    else
    {
        sprintf(localText, "00 00");
        setText(localText);
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
