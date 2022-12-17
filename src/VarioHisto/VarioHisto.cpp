#include "VarioHisto.h"
#include <Arduino.h>

VarioHisto::VarioHisto(const char *_title)
{
    title = (char *)malloc(strlen(_title) + 1);
    strcpy(title, _title);
    fc.registerObserver(this);
    preferences = new Preferences();
    loadLastHisto();
}

void VarioHisto::loadLastHisto()
{
    isFlightStarted = false;
    // get index of last recorded flight
    preferences->begin(PREFERENCES_NAMESPACE_HISTO, false);
    currentHistoIndex = preferences->getUInt(PREFERENCES_STAT_INDEX_KEY, 0);
    nbRecordedFiles = preferences->getUInt(PREFERENCES_STAT_NB_KEY, 0);
    preferences->end();

    strcpy(currentNamespace, getNamespaceIndexBased(currentHistoIndex));

    varioHistoFlight = new VarioHistoFlight(preferences, currentNamespace);
}

void VarioHisto::newHisto()
{
    isFlightStarted = true;
    // unless histo is empty, increment index
    if (nbRecordedFiles > 0)
    {
        // if max nb of histo reached, start from 0 (circular )
        if (currentHistoIndex >= (PREFERENCES_STAT_MAX_NB - 1))
        {
            currentHistoIndex = 0;
        }
        else
        {
            currentHistoIndex++;
        }
    }

    // store new pointer index
    preferences->begin(PREFERENCES_NAMESPACE_HISTO, false);
    preferences->putUInt(PREFERENCES_STAT_INDEX_KEY, currentHistoIndex);
    uint8_t nb = preferences->getUInt(PREFERENCES_STAT_NB_KEY, 0);
    if (nb < PREFERENCES_STAT_MAX_NB)
    {
        nb++;
        nbRecordedFiles = nb;
        // store the number of recorded flights
        preferences->putUInt(PREFERENCES_STAT_NB_KEY, nb);
    }
    preferences->end();

    // switch to new namespace
    strcpy(currentNamespace, getNamespaceIndexBased(currentHistoIndex));

    delete varioHistoFlight;
    varioHistoFlight = NULL;
    varioHistoFlight = new VarioHistoFlight(preferences, currentNamespace);
    // clear old data from previous flight at current index
    varioHistoFlight->beginHisto();
}

char *VarioHisto::getFormatedStartDate()
{
    return varioHistoFlight->getFormatedStartDate();
}

char *VarioHisto::getFormatedStartTime()
{
    return varioHistoFlight->getFormatedStartTime();
}

char *VarioHisto::getFormatedFlightDuration()
{
    return varioHistoFlight->getFormatedFlightDuration();
}

int16_t VarioHisto::getAltiMax()
{
    return varioHistoFlight->getAltiMax();
}

float VarioHisto::getVarioMin()
{
    return varioHistoFlight->getVarioMin();
}

float VarioHisto::getVarioMax()
{
    return varioHistoFlight->getVarioMax();
}

int16_t VarioHisto::getSpeedMax()
{
    return varioHistoFlight->getSpeedMax();
}

void VarioHisto::onSignalReceived(uint8_t _val)
{
    // Serial.println("VarioHisto::onSignalReceived");
    // Serial.println(_val);
    switch (_val)
    {
    case FLIGHT_START:
        newHisto();
        break;
    case GPS_NEW_TIME:
        if (isFlightStarted)
        {
            varioHistoFlight->setFlightDuration();
        }
        break;
    case VARIO_NEW_ALTI:
        if (isFlightStarted)
        {
            varioHistoFlight->setAltiMax();
        }
        break;
    case VARIO_NEW_VELOCITY:
        if (isFlightStarted)
        {
            varioHistoFlight->setVarioMax();
            varioHistoFlight->setVarioMin();
        }
        break;
    case GPS_NEW_SPEED:
        if (isFlightStarted)
        {
            varioHistoFlight->setSpeedMax();
        }
        break;
    default:
        break;
    }
}
void VarioHisto::setValuesToFc()
{
    char buffer[50];

    // the title
    sprintf(buffer, "%8s %d/%d", title, currentHistoIndex, nbRecordedFiles);
    fc.setText1(true, buffer);

    sprintf(buffer, "%s %s", "Date:", getFormatedStartDate());
    fc.setText2(true, buffer);

    sprintf(buffer, "%s %s", "Heure:", getFormatedStartTime());
    fc.setText3(true, buffer);

    sprintf(buffer, "%s %s", "Duree:", getFormatedFlightDuration());
    fc.setText4(true, buffer);

    sprintf(buffer, "%s %d", "Alti max:", getAltiMax());
    fc.setText5(true, buffer);

    sprintf(buffer, "%s %d.%01d", "Vario min:", (int16_t)getVarioMin(), abs((int16_t)(getVarioMin() * 10) % 10));
    fc.setText6(true, buffer);

    sprintf(buffer, "%s %d.%01d", "Vario max:", (int16_t)getVarioMax(), abs((int16_t)(getVarioMax() * 10) % 10));
    fc.setText7(true, buffer);

    sprintf(buffer, "%s %d", "Vitesse max:", getSpeedMax());
    fc.setText8(true, buffer);
}
char *VarioHisto::getNamespaceIndexBased(uint8_t index)
{
    char *namespaceIndexBased = (char *)malloc(strlen(PREFERENCES_NAMESPACE_HISTO) + 2);
    char _indexStr[1];
    itoa(index, _indexStr, 10);

    strcpy(namespaceIndexBased, PREFERENCES_NAMESPACE_HISTO);
    strcat(namespaceIndexBased, _indexStr);

    return namespaceIndexBased;
}