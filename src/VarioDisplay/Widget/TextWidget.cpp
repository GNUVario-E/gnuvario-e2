#include "TextWidget.h"

void TextWidget::addToBuffer(GxEPD2_GFX &_display)
{
    _display.setTextSize(1);

    if (blinkFreq == 0 || isText == false)
    {
        uint8_t fontSize6 = 6;
        uint8_t fontSize9 = 9;
        clearLastDiplayZone(_display);

        int16_t tbx, tby;
        uint16_t tbw, tbh;

        bool isTitle = true;

        // le titre
        if (TITLE_NAME_INDEX != 99)
        {
            setTitle((char *)varioLanguage->getText(TITLE_NAME_INDEX).c_str());
        }
        else
        {
            setTitle(nullptr);
        }
        if (isTitle && title[0] != '\0')
        {
            _display.setCursor(topx + 3, topy + fontSize6 + 3);
            _display.setFont(&NotoSans6pt7b);
            _display.setTextSize(1);
            Serial.println(title);
            _display.print(title);
        }

        _display.setFont(&FreeSansBold9pt7b);
        _display.setTextSize(1);                                    // boundary box window
        _display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)

        _display.setCursor(topx + (width - tbw) / 2, topy + (height - tbh) / 2 + fontSize9);
        _display.print(text);

        storeLastDiplayZone(_display, width, height);
        if (isBorder)
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

void TextWidget::setTitle(char *_title)
{
    if (_title != nullptr)
    {
        strcpy(title, _title);
    }
    else
    {
        title[0] = '\0';
    }
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
