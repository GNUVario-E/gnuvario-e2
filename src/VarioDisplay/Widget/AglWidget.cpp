#include "AglWidget.h"

bool AglWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.agl.aglTimestamp > getTimeout())
    {
        if (fc.agl.agl != oldAlt)
        {
            // sprintf(localText, "Alt : %d", fc.alt);
            sprintf(localText, "%d m", fc.agl.agl);

            setText(localText);
            oldAlt = fc.agl.agl;

            return true;
        }
    }
    else
    {
        setText("");
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
