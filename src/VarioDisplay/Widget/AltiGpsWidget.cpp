#include "AltiGpsWidget.h"

bool AltiGpsWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    // title = variol
    if (fc.gps.altiMetersTimestamp > getTimeout())
    {
        if (fc.gps.altiMeters != oldAlt)
        {
            sprintf(localText, "%d m", (int)fc.gps.altiMeters);

            setText(localText);
            oldAlt = fc.gps.altiMeters;

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
