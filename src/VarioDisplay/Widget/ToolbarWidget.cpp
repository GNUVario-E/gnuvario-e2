#include "ToolbarWidget.h"

void ToolbarWidget::addToBuffer(GxEPD2_GFX &_display)
{
    clearLastDiplayZone(_display);

    // center img
    int16_t imgWidth = 32;
    int16_t imgHeight = 32;

    _display.drawInvertedBitmap(topx + (imgWidth)*3, topy, batIcon, imgWidth, imgHeight, GxEPD_BLACK);
    // _display.drawInvertedBitmap(topx + imgWidth, topy, bat1icons, imgWidth, imgHeight, GxEPD_BLACK);
    // _display.drawInvertedBitmap(topx + (imgWidth)*2, topy, bat2icons, imgWidth, imgHeight, GxEPD_BLACK);
    // _display.drawInvertedBitmap(topx + (imgWidth)*3, topy, bat3icons, imgWidth, imgHeight, GxEPD_BLACK);

    // _display.drawInvertedBitmap(topx, topy + imgHeight, volume0icons, imgWidth, imgHeight, GxEPD_BLACK);
    // _display.drawInvertedBitmap(topx + imgWidth, topy + imgHeight, volume1icons, imgWidth, imgHeight, GxEPD_BLACK);
    // _display.drawInvertedBitmap(topx + (imgWidth)*2, topy + imgHeight, volume2icons, imgWidth, imgHeight, GxEPD_BLACK);
    // _display.drawInvertedBitmap(topx + (imgWidth)*3, topy + imgHeight, volume3icons, imgWidth, imgHeight, GxEPD_BLACK);

    // _display.drawInvertedBitmap(topx, topy + imgHeight * 2, saticons, imgWidth, imgHeight, GxEPD_BLACK);
    // _display.drawInvertedBitmap(topx + imgWidth * 2, topy + imgHeight * 2, satfixicons, imgWidth, imgHeight, GxEPD_BLACK);

    storeLastDiplayZone(_display, width, height);
    if (isborder)
    {
        drawborder(_display);
    }

    if (!isFirstRefreshDone)
    {
        isFirstRefreshDone = true;
    }

    // topy++;
}

bool ToolbarWidget::isRefreshNeeded()
{
    // if (fc.vario != oldVario)
    // {
    //     sprintf(localText, "V:  %d.%02d ", (int)fc.vario, (int)(fabsf(fc.vario) * 100) % 100);
    //     setText(localText);
    //     oldVario = fc.vario;

    //     return true;
    // }
    if (fc.power.capacite >= 75)
    {
        batIcon = bat4icons;
    }
    else if (fc.power.capacite >= 50)
    {
        batIcon = bat3icons;
    }
    else if (fc.power.capacite >= 25)
    {
        batIcon = bat2icons;
    }
    else if (fc.power.capacite >= 10)
    {
        batIcon = bat1icons;
    }

    if (oldBatIcon != batIcon)
    {
        oldBatIcon = batIcon;
        return true;
    }

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