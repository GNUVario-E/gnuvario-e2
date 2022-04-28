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
        uint8_t marginTitle = 0;
        if (isTitle && title != nullptr)
        {
            marginTitle = 4;
            _display.setCursor(topx + marginTitle, topy + fontSize6 + marginTitle);
            _display.setFont(&NotoSans6pt7b);
            _display.setTextSize(1);
            _display.print(title);
        }

        switch (fontSize)
        {
        case 1:
            _display.setFont(&NotoSans6pt7b);
            break;
        case 2:
            _display.setFont(&FreeSansBold9pt7b);
            break;
        default:
            break;
        }

        _display.setTextSize(1);                                    // boundary box window
        _display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)

        _display.setCursor(topx + (width - tbw) / 2, topy + (height - tbh) / 2 + fontSize9 + marginTitle * 2);
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

    hasBeenModified = false;
}

void TextWidget::setText(const char *_text)
{
    strncpy(text, _text, sizeof(text));
}

void TextWidget::setTextSize(uint8_t _size)
{
    fontSize = _size;
}

void TextWidget::setTitle(char *_title)
{
    if (_title != nullptr)
    {
        title = _title;
        // strncpy(title, _title, sizeof(title));
    }
    else
    {
        title = nullptr;
        // title[0] = '\0';
    }
}

void TextWidget::initAutoTitleFromIndex()
{
    // le titre
    if (TITLE_NAME_INDEX != 99)
    {
        Serial.println(varioLanguage->getText(TITLE_NAME_INDEX));
        setTitle(varioLanguage->getText(TITLE_NAME_INDEX));
    }
    else
    {
        setTitle(nullptr);
    }
}

void TextWidget::setBlinkFreq(uint8_t freq)
{
    blinkFreq = freq;
}

bool TextWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (hasBeenModified)
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
