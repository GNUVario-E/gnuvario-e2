#pragma once

#include <Preferences.h>
#include "VarioStatFlight.h"
#include "event.h"
#include "Observer/Observer.h"

#define PREFERENCES_NAMESPACE_HISTO "gvh"

#define PREFERENCES_STAT_INDEX_KEY "statIndex"
#define PREFERENCES_STAT_NB_KEY "statNb"
#define PREFERENCES_STAT_MAX_NB 10

class VarioStat : public Observer
{
private:
    Preferences *preferences;
    VarioStatFlight *varioStatFlight;
    void loadLastHisto();

    uint8_t currentHistoIndex = 0;
    char *currentNamespace = (char *)malloc(15);
    uint8_t nbHistos = 0;
    bool isFlightStarted = false;
    char *getNamespaceIndexBased(uint8_t index);
    char *title;
    uint8_t nbRecordedFiles = 0;

public:
    VarioStat(const char *_title);
    uint8_t getNbHistos();

    void newHisto();

    char *getFormatedStartDate();
    char *getFormatedStartTime();

    char *getFormatedFlightDuration();

    int16_t getAltiMax();

    float getVarioMin();

    float getVarioMax();

    int16_t getSpeedMax();

    void setValuesToFc();

    void onSignalReceived(uint8_t _val);
};
