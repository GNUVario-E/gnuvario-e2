#include "LatWidget.h"

bool LatWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    if (fc.getGpsLocTimestamp() > getTimeout())
    {
        if (fc.getGpsLat() != oldLat)
        {
            sprintf(localText, "%d.%06d", (int)fc.getGpsLat(), (int)(fc.getGpsLat() * 1000000) % 1000000);

            setText(localText);
            oldLat = fc.getGpsLat();

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
