#include "LonWidget.h"

bool LonWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.getGpsLocTimestamp() > getTimeout())
    {
        if (fc.getGpsLon() != oldLon)
        {
            sprintf(localText, "%d.%06d", (int)fc.getGpsLon(), (int)(fc.getGpsLon() * 1000000) % 1000000);

            setText(localText);
            oldLon = fc.getGpsLon();

            return true;
        }
    }
    else
    {
        if (strcmp(empty, getText()))
        {
            setText("");
            return true;
        }
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
