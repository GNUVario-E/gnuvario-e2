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

    // title = variol
    if (fc.getIsFlightStart())
    {
        if (fc.getFlightTimeDurationHour() > 1 && (fc.getFlightTimeDurationHour() != oldTime[0] || fc.getFlightTimeDurationMinute() != oldTime[1]))
        {
            // more than one hour of flight
            sprintf(localText, "%02d %02d", fc.getFlightTimeDurationHour(), fc.getFlightTimeDurationMinute());
            setText(localText);
            oldTime[0] = fc.getFlightTimeDurationHour();
            oldTime[1] = fc.getFlightTimeDurationMinute();
            oldTime[2] = fc.getFlightDurationSecond();

            return true;
        }
        else if (fc.getFlightTimeDurationHour() < 1 && (fc.getFlightTimeDurationMinute() != oldTime[1] || fc.getFlightDurationSecond() != oldTime[2]))
        {
            // less than one hour of flight
            sprintf(localText, "%02d %02d", fc.getFlightTimeDurationMinute(), fc.getFlightDurationSecond());
            setText(localText);
            oldTime[0] = fc.getFlightTimeDurationHour();
            oldTime[1] = fc.getFlightTimeDurationMinute();
            oldTime[2] = fc.getFlightDurationSecond();

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
