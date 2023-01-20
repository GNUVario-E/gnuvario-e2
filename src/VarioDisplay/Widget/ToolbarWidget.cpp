#include "ToolbarWidget.h"

void ToolbarWidget::addToBuffer(GxEPD2_GFX &_display)
{
    clearLastDiplayZone(_display);

    // center img
    int16_t imgWidth = 32;
    int16_t imgHeight = 32;

    int16_t imgWidthSm = 16;
    int16_t imgHeightSm = 16;

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

    if (params->P_BT_ENABLE->getValue())
    {
        _display.drawInvertedBitmap(topx + imgWidthSm / 2, topy + imgHeight - 1, bluetoothicons, imgWidthSm, imgHeightSm, GxEPD_BLACK);
    }

    _display.setFont(&NotoSans6pt7b);
    _display.setTextSize(1);

    int16_t tbx, tby;
    uint16_t tbw, tbh;

    // wait for record and record in progress
    if (isReadyToStartRecording)
    {
        // wait for start
        _display.drawInvertedBitmap(topx + (imgWidth), topy, waitrecordicons, imgWidth, imgHeight, GxEPD_BLACK);
    }
    else if (isFlightIsStarted)
    {
        // record in progress
        _display.drawInvertedBitmap(topx + (imgWidth), topy, recordicons, imgWidth, imgHeight, GxEPD_BLACK);
    }

    // satellites count
    if (isGpsFixed)
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

    if (fc.getPowerCapacite() >= 75)
    {
        batIcon = bat4icons;
    }
    else if (fc.getPowerCapacite() >= 50)
    {
        batIcon = bat3icons;
    }
    else if (fc.getPowerCapacite() >= 25)
    {
        batIcon = bat2icons;
    }
    else if (fc.getPowerCapacite() >= 10)
    {
        batIcon = bat1icons;
    }

    if (oldBatIcon != batIcon)
    {
        oldBatIcon = batIcon;
        hasChange = true;
    }

    if (oldBatPct != fc.getPowerCapacite())
    {
        batPct = fc.getPowerCapacite();
        oldBatPct = batPct;
        hasChange = true;
    }

    if (oldVolume != fc.getSoundVolume())
    {
        volume = fc.getSoundVolume();
        oldVolume = volume;
        hasChange = true;
    }

    if (oldIsMute != fc.getSoundIsMute())
    {
        isMute = fc.getSoundIsMute();
        oldIsMute = isMute;
        hasChange = true;
    }

    if (oldSatCount != fc.getGpsSatellitesCount())
    {
        satCount = fc.getGpsSatellitesCount();
        oldSatCount = fc.getGpsSatellitesCount();
        hasChange = true;
    }

    isReadyToStartRecording = false;
    isFlightIsStarted = false;
    if (fc.getGpsIsFixed() && fc.getGpsIsFixedTimestamp() > (millis() - getTimeout()) && !fc.getIsFlightStart())
    {
        isReadyToStartRecording = true;
    }
    else if (fc.getIsFlightStart())
    {
        isFlightIsStarted = true;
    }

    if (oldIsReadyToStartRecording != isReadyToStartRecording)
    {
        oldIsReadyToStartRecording = isReadyToStartRecording;
        hasChange = true;
    }

    if (oldIsFlightIsStarted != isFlightIsStarted)
    {
        oldIsFlightIsStarted = isFlightIsStarted;
        hasChange = true;
    }

    if (fc.getGpsIsFixed() && fc.getGpsIsFixedTimestamp() > getTimeout())
    {
        isGpsFixed = true;
    }
    else
    {
        isGpsFixed = false;
    }

    if (oldIsGpsFixed != isGpsFixed)
    {
        oldIsGpsFixed = isGpsFixed;
        hasChange = true;
    }

    if (hasChange)
    {
        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}