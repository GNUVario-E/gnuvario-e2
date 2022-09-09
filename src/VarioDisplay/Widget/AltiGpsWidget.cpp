#include "AltiGpsWidget.h"

bool AltiGpsWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    if (fc.getGpsAltiMetersTimestamp() > getTimeout())
    {
        if (fc.getGpsAltiMeters() != oldAlt)
        {
            sprintf(localText, "%d m", (int)fc.getGpsAltiMeters());

            setText(localText);
            oldAlt = fc.getGpsAltiMeters();

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
