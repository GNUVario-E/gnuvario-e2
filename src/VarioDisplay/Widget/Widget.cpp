#include "Widget.h"

Widget::Widget(int16_t _topx, int16_t _topy, int16_t _width, int16_t _height)
{
    topx = _topx;
    topy = _topy;
    width = _width;
    height = _height;
    isFirstRefreshDone = false;
}

void Widget::setVarioLangage(VarioLanguage *_varioLanguage)
{
    varioLanguage = _varioLanguage;
}

void Widget::setIsActif(bool _isactif)
{
    isActif = _isactif;
}

void Widget::setForceRefresh()
{
    isFirstRefreshDone = false;
}

void Widget::setPosition(int16_t _topx, int16_t _topy, int16_t _width, int16_t _height)
{
    topx = _topx;
    topy = _topy;
    width = _width;
    height = _height;
}

void Widget::storeLastDiplayZone(GxEPD2_GFX &_display, int16_t w, int16_t h)
{
    if (isBorder)
    {
        lastDisplayZone.topx = topx + 1;
        lastDisplayZone.topy = topy + 1;
        lastDisplayZone.width = w - 2;
        lastDisplayZone.height = h - 2;
    }
    else
    {
        lastDisplayZone.topx = topx;
        lastDisplayZone.topy = topy;
        lastDisplayZone.width = w;
        lastDisplayZone.height = h;
    }
}

void Widget::clearLastDiplayZone(GxEPD2_GFX &_display)
{
    if (lastDisplayZone.topx != 0 && lastDisplayZone.topy != 0 && lastDisplayZone.width != 0 && lastDisplayZone.height != 0)
    {
        _display.fillRect(lastDisplayZone.topx, lastDisplayZone.topy, lastDisplayZone.width, lastDisplayZone.height, GxEPD_WHITE);
    }
}

void Widget::drawborder(GxEPD2_GFX &_display)
{
    _display.drawRect(topx, topy, width, height, GxEPD_BLACK);
}

bool Widget::getIsActif()
{
    return isActif;
}

void Widget::setIsborder(bool _isborder)
{
    isBorder = _isborder;
}

// void Widget::addToBuffer(GxEPD2_GFX &_display)
// {
// }
