#include "TextWidget.h"

void TextWidget::addToBuffer(GxEPD2_GFX &_display)
{
    _display.setTextSize(1);

    if (blinkFreq == 0 || isText == false)
    {
        clearLastDiplayZone(_display);

        int16_t tbx, tby;
        uint16_t tbw, tbh;
        _display.setFont(&FreeSansBold9pt7b);
        _display.setTextSize(1);                                    // boundary box window
        _display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)

        _display.setCursor(topx + (width - tbw) / 2, topy + (height - tbh) / 2 + 9);
        _display.print(text);
        // _display.drawRect(topx + (width - tbw) / 2, topy + (height - tbh) / 2, tbw, tbh, GxEPD_BLACK);

        storeLastDiplayZone(_display, width, height);
        if (isborder)
        {
            drawborder(_display);
        }

        isText = true;
    }
    else
    {
        clearLastDiplayZone(_display);

        isText = false;
    }

    lastDisplayTime = millis();

    if (!isFirstRefreshDone)
    {
        isFirstRefreshDone = true;
    }
}

void TextWidget::setText(char *_text)
{
    text = _text;
}

void TextWidget::setBlinkFreq(uint8_t freq)
{
    blinkFreq = freq;
}

bool TextWidget::isRefreshNeeded()
{
    if (!isFirstRefreshDone)
    {
        return true;
    }

    // si pas de clignotement, pas de refresh nécessaire
    if (blinkFreq == 0)
    {
        return false;
    }

    // si le dernier affichage date de plus longtemps que le temps de clignotement, on doit raffraichir
    if ((millis() - lastDisplayTime) > (blinkFreq * 1000))
    {
        return true;
    }

    return false;
}
