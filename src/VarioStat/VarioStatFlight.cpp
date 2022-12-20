#include "VarioStatFlight.h"
#include <Arduino.h>

VarioStatFlight::VarioStatFlight(Preferences *_preferences, char *_currentNamespace)
{
    preferences = _preferences;
    currentNamespace = _currentNamespace;
    loadHisto();
}

void VarioStatFlight::clear()
{
    preferences->begin(currentNamespace, false);
    preferences->clear();
    preferences->end();
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    durationHour = 0;
    durationMinute = 0;
    altiMax = 0;
    varioMin = 0.0;
    varioMax = 0.0;
    speedMax = 0;
}

void VarioStatFlight::beginHisto()
{
    clear();
    setStartDateAndTime();
}

void VarioStatFlight::loadHisto()
{

    preferences->begin(currentNamespace, false);
    year = preferences->getUInt(START_DATE_YEAR_KEY, 0);
    month = preferences->getUInt(START_DATE_MONTH_KEY, 0);
    day = preferences->getUInt(START_DATE_DAY_KEY, 0);
    hour = preferences->getUInt(START_TIME_HOUR_KEY, 0.0);
    minute = preferences->getUInt(START_TIME_MINUTE_KEY, 0.0);
    durationHour = preferences->getUInt(FLIGHT_DURATION_HOUR_KEY, 0);
    durationMinute = preferences->getUInt(FLIGHT_DURATION_MINUTE_KEY, 0);
    altiMax = preferences->getInt(ALTI_MAX_KEY, 0);
    varioMin = preferences->getFloat(VARIO_MIN_KEY, 0.0);
    varioMax = preferences->getFloat(VARIO_MAX_KEY, 0.0);
    speedMax = preferences->getInt(SPEED_MAX_KEY, 0);
    preferences->end();
}

void VarioStatFlight::setStartDateAndTime()
{
    year = fc.getGpsDateYear();
    month = fc.getGpsDateMonth();
    day = fc.getGpsDateDay();
    hour = fc.getGpsTimeHour();
    minute = fc.getGpsTimeMinute();

    preferences->begin(currentNamespace, false);
    preferences->putUInt(START_DATE_YEAR_KEY, fc.getGpsDateYear());
    preferences->putUInt(START_DATE_MONTH_KEY, fc.getGpsDateMonth());
    preferences->putUInt(START_DATE_DAY_KEY, fc.getGpsDateDay());

    preferences->putUInt(START_TIME_HOUR_KEY, fc.getGpsTimeHour());
    preferences->putUInt(START_TIME_MINUTE_KEY, fc.getGpsTimeMinute());

    preferences->end();
}

char *VarioStatFlight::getFormatedStartDate()
{
    const char *format = "%02d/%02d/%02d";
    char *buffer = (char *)malloc(11);
    sprintf(buffer, format, day, month, year);

    return buffer;
}

char *VarioStatFlight::getFormatedStartTime()
{
    const char *format = "%02d:%02d";
    char *buffer = (char *)malloc(6);
    sprintf(buffer, format, hour, minute);

    return buffer;
}

void VarioStatFlight::setFlightDuration()
{
    uint8_t _durationHour = fc.getFlightTimeDurationHour();
    uint8_t _durationMinute = fc.getFlightTimeDurationMinute();

    if (_durationHour != durationHour || _durationMinute != durationMinute)
    {
        durationHour = _durationHour;
        durationMinute = _durationMinute;
        preferences->begin(currentNamespace, false);
        preferences->putUInt(FLIGHT_DURATION_HOUR_KEY, durationHour);
        preferences->putUInt(FLIGHT_DURATION_MINUTE_KEY, durationMinute);
        preferences->end();
    }
}

char *VarioStatFlight::getFormatedFlightDuration()
{
    const char *format = "%02d:%02d";
    char *buffer = (char *)malloc(6);
    sprintf(buffer, format, durationHour, durationMinute);

    return buffer;
}

void VarioStatFlight::setAltiMax()
{
    int16_t alti = (int16_t)(fc.getGpsAltiMeters() / 10) * 10;
    if (alti > altiMax)
    {
        altiMax = alti;
        preferences->begin(currentNamespace, false);
        preferences->putInt(ALTI_MAX_KEY, altiMax);
        preferences->end();
    }
}

int16_t VarioStatFlight::getAltiMax()
{
    return altiMax;
}

void VarioStatFlight::setVarioMin()
{
    if (fc.getVarioVelocity() < varioMin)
    {
        varioMin = fc.getVarioVelocity();
        preferences->begin(currentNamespace, false);
        preferences->putFloat(VARIO_MIN_KEY, varioMin);
        preferences->end();
    }
}

float VarioStatFlight::getVarioMin()
{
    return varioMin;
}

void VarioStatFlight::setVarioMax()
{
    if (fc.getVarioVelocity() > varioMax)
    {
        varioMax = fc.getVarioVelocity();
        preferences->begin(currentNamespace, false);
        preferences->putFloat(VARIO_MAX_KEY, varioMax);
        preferences->end();
    }
}

float VarioStatFlight::getVarioMax()
{
    return varioMax;
}

void VarioStatFlight::setSpeedMax()
{
    if (fc.getGpsKmph() > speedMax)
    {
        speedMax = fc.getGpsKmph();
        preferences->begin(currentNamespace, false);
        preferences->putInt(SPEED_MAX_KEY, speedMax);
        preferences->end();
    }
}

int16_t VarioStatFlight::getSpeedMax()
{
    return speedMax;
}
