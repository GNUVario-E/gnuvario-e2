#include "VarioWidget.h"

bool VarioWidget::isRefreshNeeded()
{
    // if (fc.vario != oldVario)
    // {
    //     sprintf(localText, "V:  %d.%02d ", (int)fc.vario, (int)(fabsf(fc.vario) * 100) % 100);
    //     setText(localText);
    //     oldVario = fc.vario;

    //     return true;
    // }
    // char *_title = (char *)varioLanguage->getText(TITRE_VARIO).c_str();

    if (fc.power.tension != oldVario)
    {
        sprintf(localText, "V:  %d.%02d ", (int)fc.power.tension, (int)(fabsf(fc.power.tension) * 100) % 100);
        setText(localText);
        oldVario = fc.power.tension;

        return true;
    }

    if (!isFirstRefreshDone)
    {
        return true;
    }

    // si pas de clignotement, pas de refresh nécessaire
    if (blinkFreq == 0)
    {
        return false;
    }

    // si le dernier affichage date de plus longtemps que le temps de clignotement, on doit raffraichir
    if ((millis() - lastDisplayTime) > (blinkFreq * 1000))
    {
        return true;
    }

    return false;
}