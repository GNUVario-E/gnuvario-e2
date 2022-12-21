#include "TextWidget.h"

void TextWidget::addToBuffer(GxEPD2_GFX &_display)
{
    if (forceClear)
    {
        _display.fillRect(topx, topy, width, height, GxEPD_WHITE);
        forceClear = false;
    }

    _display.setTextSize(1);

    if (blinkFreq == 0 || isText == false)
    {
        uint8_t fontSize6 = 6;
        uint8_t fontSize9 = 9;
        uint8_t fontSize18 = 18;
        uint8_t fontSize26 = 26;
        uint8_t fontSize32 = 32;
        uint8_t paddingTop = 0;
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
            paddingTop = 6;
        }

        switch (fontSize)
        {
        case 1:
            _display.setFont(&NotoSans6pt7b);
            paddingTop += fontSize6;
            break;
        case 2:
            _display.setFont(&FreeSansBold9pt7b);
            paddingTop += fontSize9;
            break;
        case 4:
            _display.setFont(&Gnuvarioe18pt7b);
            paddingTop += fontSize18;
            break;
        case 5:
            _display.setFont(&Gnuvarioe26pt7b);
            paddingTop += fontSize26;
            break;
        case 6:
            _display.setFont(&FreeSansBold32pt7b);
            paddingTop += fontSize32;
            break;
        default:
            break;
        }

        _display.setTextSize(1);                                    // boundary box window
        _display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)

        _display.setCursor(topx - marginTextRight + (width - tbw) / 2 - tbx, topy + (height - tbh) / 2 + paddingTop + marginTitle * 2);
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

    hasBeenModified = false;
}

void TextWidget::setText(const char *_text)
{
    strncpy(text, _text, sizeof(text));
}

const char *TextWidget::getText()
{
    return text;
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
    }
    else
    {
        title = nullptr;
    }
}

void TextWidget::initAutoTitleFromIndex()
{
    // le titre
    if (TITLE_NAME_INDEX != 99)
    {
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

    bool hasChange = false;

    switch (indexTxtFC)
    {
    case 1:
        if (fc.getIsText1())
        {
            setText(fc.getText1());
            fc.setText1(false, "");
            hasChange = true;
        }
        break;
    case 2:
        if (fc.getIsText2())
        {
            setText(fc.getText2());
            fc.setText2(false, "");
            hasChange = true;
        }
        break;
    case 3:
        if (fc.getIsText3())
        {
            setText(fc.getText3());
            fc.setText3(false, "");
            hasChange = true;
        }
        break;
    case 4:
        if (fc.getIsText4())
        {
            setText(fc.getText4());
            fc.setText4(false, "");
            hasChange = true;
        }
        break;
    case 5:
        if (fc.getIsText5())
        {
            setText(fc.getText5());
            fc.setText5(false, "");
            hasChange = true;
        }
        break;
    case 6:
        if (fc.getIsText6())
        {
            setText(fc.getText6());
            fc.setText6(false, "");
            hasChange = true;
        }
        break;
    case 7:
        if (fc.getIsText7())
        {
            setText(fc.getText7());
            fc.setText7(false, "");
            hasChange = true;
        }
        break;
    case 8:
        if (fc.getIsText8())
        {
            setText(fc.getText8());
            fc.setText8(false, "");
            hasChange = true;
        }
        break;
    }
    if (hasChange)
    {
        return true;
    }

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

void TextWidget::setMarginTextRight(int8_t _margin)
{
    marginTextRight = _margin;
}

void TextWidget::setIndexTxtFC(uint8_t _index)
{
    indexTxtFC = _index;
}
