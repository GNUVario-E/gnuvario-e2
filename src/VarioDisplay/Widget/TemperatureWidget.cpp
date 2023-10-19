#include "TemperatureWidget.h"

bool TemperatureWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    float_t temperature = fc.getVarioTemperature();    
    if (temperature != oldTemperature)
    {
        sprintf(localText, "%.1fº", temperature);

        setText(localText);
        oldTemperature = temperature;

        return true;
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}
