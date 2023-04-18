#include "WindWidget.h"
#include "VarioTool/VarioTool.h"

bool WindWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    char bearingTxt[3];

    if (fc.getWindTimestamp() > getTimeout())
    {
        if ((fc.getWindSpeed() != oldSpeed) || (fc.getWindHeading() != oldHeading) || (fc.getWindMeanSpeed() != oldMeanSpeed))
        {
            VarioTool::bearingToOrdinal2c(bearingTxt, fc.getWindHeading());
            sprintf(localText, "%d km/h %s %d km/h", fc.getWindSpeed(), bearingTxt, fc.getWindMeanSpeed());

            setText(localText);
            oldSpeed = fc.getWindSpeed();
            oldHeading = fc.getWindHeading();
            oldMeanSpeed = fc.getWindMeanSpeed();

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

/* timeout of 15s because timer that calulate wind run every 10s */
uint32_t WindWidget::getTimeout()
{
    return millis() - 15000;
}