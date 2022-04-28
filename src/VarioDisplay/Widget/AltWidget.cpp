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

    if (hasBeenModified)
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
