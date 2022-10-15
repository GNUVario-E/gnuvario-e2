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
        if (strcmp(empty, getText()) != 0)
        {
            setText("");
            return true;
        }
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
