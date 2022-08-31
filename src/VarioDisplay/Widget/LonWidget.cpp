#include "LonWidget.h"

bool LonWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.gps.locTimestamp > getTimeout())
    {
        if (fc.gps.locLon != oldLon)
        {
            sprintf(localText, "%d.%06d", (int)fc.gps.locLon, (int)(fc.gps.locLon * 1000000) % 1000000);

            setText(localText);
            oldLon = fc.gps.locLon;

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
