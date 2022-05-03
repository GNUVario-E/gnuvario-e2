#include "AltWidget.h"

bool AltWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    // title = variol
    if (fc.alt != oldAlt)
    {
        // sprintf(localText, "Alt : %d", fc.alt);
        sprintf(localText, "Cap : %d", fc.power.capacite);

        setText(localText);
        oldAlt = fc.alt;
        // sprintf(text, "Alt : %d", 10);
        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
