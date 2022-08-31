#include "LatWidget.h"

bool LatWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    if (fc.gps.locTimestamp > getTimeout())
    {
        if (fc.gps.locLat != oldLat)
        {
            sprintf(localText, "%d.%06d", (int)fc.gps.locLat, (int)(fc.gps.locLat * 1000000) % 1000000);

            setText(localText);
            oldLat = fc.gps.locLat;

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
