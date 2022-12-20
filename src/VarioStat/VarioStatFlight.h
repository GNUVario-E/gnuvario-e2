#pragma once

#include <Preferences.h>
#include "FC.h"

#define START_DATE_DAY_KEY "sDD"
#define START_DATE_MONTH_KEY "sDM"
#define START_DATE_YEAR_KEY "sDY"
#define START_TIME_HOUR_KEY "sTH"
#define START_TIME_MINUTE_KEY "sTM"
#define FLIGHT_DURATION_HOUR_KEY "fDH"
#define FLIGHT_DURATION_MINUTE_KEY "fDM"
#define ALTI_MAX_KEY "aM"
#define VARIO_MIN_KEY "vMi"
#define VARIO_MAX_KEY "vMa"
#define SPEED_MAX_KEY "sMa"

class VarioStatFlight
{
private:
    Preferences *preferences;
    void loadHisto();
    void clear();

    char *currentNamespace;

    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t durationHour;
    uint8_t durationMinute;
    int16_t altiMax = 0;
    float varioMin = 0.0;
    float varioMax = 0.0;
    int16_t speedMax = 0;

public:
    VarioStatFlight(Preferences *_preferences, char *_currentNamespace);
    void beginHisto();

    void setStartDateAndTime();
    char *getFormatedStartDate();
    char *getFormatedStartTime();

    void setFlightDuration();
    char *getFormatedFlightDuration();

    void setAltiMax();
    int16_t getAltiMax();

    void setVarioMin();
    float getVarioMin();

    void setVarioMax();
    float getVarioMax();

    void setSpeedMax();
    int16_t getSpeedMax();
};

extern FC fc;