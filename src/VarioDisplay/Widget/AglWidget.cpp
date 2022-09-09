#include "AglWidget.h"

bool AglWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.getAglAltTimestamp() > getTimeout())
    {
        if (fc.getAglAlt() != oldAlt)
        {
            sprintf(localText, "%d m", fc.getAglAlt());

            setText(localText);
            oldAlt = fc.getAglAlt();

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
