#include "AltiWidget.h"

bool AltiWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    // title = variol
    if (fc.getVarioAlti() != oldAlt)
    {
        // sprintf(localText, "Alt : %d", fc.alt);
        sprintf(localText, "%d m", fc.getVarioAlti());

        setText(localText);
        oldAlt = fc.getVarioAlti();

        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
