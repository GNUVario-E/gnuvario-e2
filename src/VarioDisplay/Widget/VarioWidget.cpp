#include "VarioWidget.h"

void VarioWidget::addToBuffer(GxEPD2_GFX &_display)
{
    TextWidget::addToBuffer(_display);

    // image unité
    uint8_t imgSize = 24;
    _display.drawBitmap(topx + width - imgSize - 2, topy + (height / 2) - (imgSize / 2), msicons, imgSize, imgSize, GxEPD_BLACK);
}

bool VarioWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.getVarioVelocity() != oldVario)
    {
        float positiveVelocity = fabsf(fc.getVarioVelocity());
        char sign[] = "+";
        if (fc.getVarioVelocity() < 0)
        {
            sign[0] = '-';
        }

        sprintf(localText, "%s %d.%d", sign, (int)(positiveVelocity), (int)((positiveVelocity - (int)(positiveVelocity)) * 10));
        setText(localText);
        oldVario = fc.getVarioVelocity();

        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}