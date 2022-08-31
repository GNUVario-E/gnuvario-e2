#include "BearingWidget.h"

void BearingWidget::addToBuffer(GxEPD2_GFX &_display)
{
    TextWidget::addToBuffer(_display);
}

bool BearingWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    if (fc.gps.kmph > 5 && fc.gps.kmphTimestamp > getTimeout() && fc.gps.headingDegTimestamp > getTimeout())
    {
        // cas ou on a une vitesse GPS supérieur à 5km/h valide depuis moins d'une seconde
        if (fc.gps.headingDeg != oldBearing)
        {
            if (TITLE_NAME_INDEX != TITRE_CAP)
            {
                TITLE_NAME_INDEX = TITRE_CAP;
                initAutoTitleFromIndex();
            }
            sprintf(localText, "%s %d", fc.gps.headingTxt, fc.gps.headingDeg);
            setText(localText);
            oldBearing = fc.gps.headingDeg;

            return true;
        }
    }
    else
    {
        if (fc.vario.bearing != oldBearing)
        {
            if (TITLE_NAME_INDEX != TITRE_COMPAS)
            {
                TITLE_NAME_INDEX = TITRE_COMPAS;
                initAutoTitleFromIndex();
            }
            sprintf(localText, "%s %d", fc.vario.bearingTxt, fc.vario.bearing);
            setText(localText);
            oldBearing = fc.vario.bearing;

            return true;
        }
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}