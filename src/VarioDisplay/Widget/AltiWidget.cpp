#include "AltiWidget.h"

bool AltiWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    // title = variol
    if (fc.vario.alti != oldAlt)
    {
        // sprintf(localText, "Alt : %d", fc.alt);
        sprintf(localText, "%d m", fc.vario.alti);

        setText(localText);
        oldAlt = fc.vario.alti;

        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
