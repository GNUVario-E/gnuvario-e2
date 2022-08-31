#include "ToolbarWidget.h"

void ToolbarWidget::addToBuffer(GxEPD2_GFX &_display)
{
    clearLastDiplayZone(_display);

    // center img
    int16_t imgWidth = 32;
    int16_t imgHeight = 32;

    if (isMute || (volume == 0))
    {
        _display.drawInvertedBitmap(topx, topy, volume0icons, imgWidth, imgHeight, GxEPD_BLACK);
    }
    else if (volume < 5)
    {
        _display.drawInvertedBitmap(topx, topy, volume1icons, imgWidth, imgHeight, GxEPD_BLACK);
    }
    else if (volume < 9)
    {
        _display.drawInvertedBitmap(topx, topy, volume2icons, imgWidth, imgHeight, GxEPD_BLACK);
    }
    else
    {
        _display.drawInvertedBitmap(topx, topy, volume3icons, imgWidth, imgHeight, GxEPD_BLACK);
    }

    _display.setFont(&NotoSans6pt7b);
    _display.setTextSize(1);

    int16_t tbx, tby;
    uint16_t tbw, tbh;

    // satellites count
    if (fc.gps.locTimestamp > getTimeout())
    {
        _display.drawInvertedBitmap(topx + (imgWidth)*2, topy, fixicons, imgWidth, imgHeight, GxEPD_BLACK);
    }
    else
    {
        _display.drawInvertedBitmap(topx + (imgWidth)*2, topy, nofixicons, imgWidth, imgHeight, GxEPD_BLACK);
    }

    _display.getTextBounds(String(satCount), 0, 0, &tbx, &tby, &tbw, &tbh);

    _display.setCursor(topx + (imgWidth)*2 + (imgWidth / 2 - tbw / 2), topy + imgHeight + 10);
    _display.print(satCount);

    // Battery
    _display.drawInvertedBitmap(topx + (imgWidth)*3, topy, batIcon, imgWidth, imgHeight, GxEPD_BLACK);

    _display.setFont(&NotoSans6pt7b);
    _display.setTextSize(1);

    _display.getTextBounds(String(batPct), 0, 0, &tbx, &tby, &tbw, &tbh);

    _display.setCursor(topx + (imgWidth)*3 + (imgWidth / 2 - tbw / 2), topy + imgHeight + 10);
    _display.print(batPct);

    storeLastDiplayZone(_display, width, height);
    if (isBorder)
    {
        drawborder(_display);
    }

    hasBeenModified = false;
}

bool ToolbarWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    bool hasChange = false;

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
        hasChange = true;
    }

    if (oldBatPct != fc.power.capacite)
    {
        batPct = fc.power.capacite;
        oldBatPct = batPct;
        hasChange = true;
    }

    if (oldVolume != fc.sound.volume)
    {
        volume = fc.sound.volume;
        oldVolume = volume;
        hasChange = true;
    }

    if (oldIsMute != fc.sound.isMute)
    {
        isMute = fc.sound.isMute;
        oldIsMute = isMute;
        hasChange = true;
    }

    if (oldSatCount != fc.gps.satellitesCount)
    {
        satCount = fc.gps.satellitesCount;
        oldSatCount = fc.gps.satellitesCount;
        hasChange = true;
    }

    if (hasChange)
    {
        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}