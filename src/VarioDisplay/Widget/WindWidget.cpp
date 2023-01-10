#include "WindWidget.h"

bool WindWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    if (fc.getWindTimestamp() > getTimeout())
    {
        if ((fc.getWindSpeed() != oldSpeed) || (fc.getWindHeading() != oldHeading) || (fc.getWindMeanSpeed() != oldMeanSpeed))
        {
            sprintf(localText, "%d km/h %d° %d km/h", fc.getWindSpeed(), fc.getWindHeading(), fc.getWindSpeed());

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
