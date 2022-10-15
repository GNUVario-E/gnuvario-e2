#include "BearingWidget.h"

void BearingWidget::addToBuffer(GxEPD2_GFX &_display)
{
    TextWidget::addToBuffer(_display);
}

bool BearingWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{

    if (fc.getGpsKmph() > 5 && fc.getGpsKmphTimestamp() > getTimeout() && fc.getGpsHeadingDegTimestamp() > getTimeout())
    {
        // cas ou on a une vitesse GPS supérieur à 5km/h valide depuis moins d'une seconde
        if (fc.getGpsHeadingDeg() != oldBearing)
        {
            if (TITLE_NAME_INDEX != TITRE_CAP)
            {
                TITLE_NAME_INDEX = TITRE_CAP;
                initAutoTitleFromIndex();
            }
            sprintf(localText, "%s %d", fc.getGpsHeadingTxt(), fc.getGpsHeadingDeg());
            setText(localText);
            oldBearing = fc.getGpsHeadingDeg();

            return true;
        }
    }
    else
    {
        if (fc.getVarioBearing() != oldBearing)
        {
            if (TITLE_NAME_INDEX != TITRE_COMPAS)
            {
                TITLE_NAME_INDEX = TITRE_COMPAS;
                initAutoTitleFromIndex();
            }
            sprintf(localText, "%s %d", fc.getVarioBearingTxt(), fc.getVarioBearing());
            setText(localText);
            oldBearing = fc.getVarioBearing();

            return true;
        }
    }

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}