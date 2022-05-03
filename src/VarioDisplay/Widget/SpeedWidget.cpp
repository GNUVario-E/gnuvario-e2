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
    // if (fc.vario != oldVario)
    // {
    //     sprintf(localText, "V:  %d.%02d ", (int)fc.vario, (int)(fabsf(fc.vario) * 100) % 100);
    //     setText(localText);
    //     oldVario = fc.vario;

    //     return true;
    // }
    // char *_title = (char *)varioLanguage->getText(TITRE_VARIO).c_str();

    if (fc.power.tension != oldSpeed)
    {
        sprintf(localText, "%d.%02d ", (int)fc.power.tension, (int)(fabsf(fc.power.tension) * 100) % 100);
        setText(localText);
        oldSpeed = fc.power.tension;

        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}