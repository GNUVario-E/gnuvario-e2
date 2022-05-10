#include "BearingWidget.h"

void BearingWidget::addToBuffer(GxEPD2_GFX &_display)
{
    TextWidget::addToBuffer(_display);
}

bool BearingWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.vario.bearing != oldBearing)
    {

        sprintf(localText, "%d", fc.vario.bearing);
        setText(localText);
        oldBearing = fc.vario.bearing;

        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}