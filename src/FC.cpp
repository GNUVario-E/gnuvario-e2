#include <FC.h>
#include "event.h"

#define TIMEOUT_DATA 1200
#define MIN_DURATION_TAKEOFF 2000 // time during parameters conditions must be true to start flight

void FC::setTzn(int8_t _tzn)
{
    tzn = _tzn;
}

void FC::checkFlightStart(bool forceManualStart)
{
    bool doStart = false;
    if (!fcdata.flight.isFlightStart)
    {
        uint32_t currentMillis = millis();
        if (fcdata.gps.isFixed && fcdata.gps.locTimestamp > (currentMillis - TIMEOUT_DATA) && fcdata.gps.kmphTimestamp > (currentMillis - TIMEOUT_DATA))
        {
            if (forceManualStart)
            {
                doStart = true;
            }
            else if (fcdata.gps.kmph >= params->P_FLIGHT_START_MIN_SPEED->getValue() && ((fcdata.vario.velocity >= params->P_FLIGHT_START_VARIO_HIGH_THRESHOLD->getValue()) || (fcdata.vario.velocity <= params->P_FLIGHT_START_VARIO_LOW_THRESHOLD->getValue())))
            {
                if (fcdata.flight.flightStartFistTimestamp == (uint32_t)ULONG_MAX)
                {
                    fcdata.flight.flightStartFistTimestamp = millis();
                }
                else if (fcdata.flight.flightStartFistTimestamp < (currentMillis - MIN_DURATION_TAKEOFF))
                {
                    doStart = true;
                }
            }
            if (doStart)
            {
                fcdata.flight.isFlightStart = true;
                fcdata.flight.flightStartTime[0] = fcdata.gps.timeHour;
                fcdata.flight.flightStartTime[1] = fcdata.gps.timeMinute;
                fcdata.flight.flightStartTime[2] = fcdata.gps.timeSecond;
                _notifyObserver(FLIGHT_START);
            }
        }
        else
        {
            fcdata.flight.flightStartFistTimestamp = (uint32_t)ULONG_MAX;
        }
    }
}

FC::FC()
{
}

// POWER
void FC::setPowerCapacite(int capacite, uint32_t capaciteTimestamp)
{
    fcdata.power.capacite = capacite;
    fcdata.power.capaciteTimestamp = capaciteTimestamp;
}

int FC::getPowerCapacite()
{
    return fcdata.power.capacite;
}

uint32_t FC::getPowerCapaciteTimestamp()
{
    return fcdata.power.capaciteTimestamp;
}

void FC::setPowerTension(float tension, uint32_t tensionTimestamp)
{
    fcdata.power.tension = tension;
    fcdata.power.tensionTimestamp = tensionTimestamp;
}

float FC::getPowerTension()
{
    return fcdata.power.tension;
}

uint32_t FC::getPowerTensionTimestamp()
{
    return fcdata.power.tensionTimestamp;
}

// SOUND
void FC::setSoundIsMute(bool isMute)
{
    fcdata.sound.isMute = isMute;
}

void FC::setSoundVolume(float volume)
{
    fcdata.sound.volume = volume;
}

bool FC::getSoundIsMute()
{
    return fcdata.sound.isMute;
}

float FC::getSoundVolume()
{
    return fcdata.sound.volume;
}

// VARIO
void FC::setVarioAlti(int16_t alti, uint32_t altiTimestamp)
{
    fcdata.vario.alti = alti;
    fcdata.vario.altiTimestamp = altiTimestamp;
    if (fcdata.vario.altiTimestamp > (millis() - TIMEOUT_DATA))
    {
        _notifyObserver(VARIO_NEW_ALTI);
    }
}

int16_t FC::getVarioAlti()
{
    return fcdata.vario.alti;
}

uint32_t FC::getVarioAltiTimestamp()
{
    return fcdata.vario.altiTimestamp;
}

void FC::setVarioVelocity(float velocity, uint32_t velocityTimestamp)
{
    fcdata.vario.velocity = velocity;
    fcdata.vario.velocityTimestamp = velocityTimestamp;
    checkFlightStart(false);
    _notifyObserver(VARIO_NEW_VELOCITY);
}

float FC::getVarioVelocity()
{
    return fcdata.vario.velocity;
}

uint32_t FC::getVarioVelocityTimestamp()
{
    return fcdata.vario.velocityTimestamp;
}

void FC::setVarioBearing(int16_t bearing, char bearingTxt[3], uint32_t bearingTimestamp)
{
    fcdata.vario.bearing = bearing;
    strcpy(fcdata.vario.bearingTxt, bearingTxt);
    fcdata.vario.bearingTimestamp = bearingTimestamp;
}

int16_t FC::getVarioBearing()
{
    return fcdata.vario.bearing;
}

char *FC::getVarioBearingTxt()
{
    return fcdata.vario.bearingTxt;
}

uint32_t FC::getVarioBearingTimestamp()
{
    return fcdata.vario.bearingTimestamp;
}

// WIFI
void FC::setWifiConnected(bool connected)
{
    fcdata.wifi.connected = connected;
}

void FC::setWifiSsid(char ssid[32])
{
    strcpy(fcdata.wifi.ssid, ssid);
}

void FC::setWifiIp(char ip[16])
{
    strcpy(fcdata.wifi.ip, ip);
}

bool FC::getWifiConnected()
{
    return fcdata.wifi.connected;
}

char *FC::getWifiSsid()
{
    return fcdata.wifi.ssid;
}

char *FC::getWifiIp()
{
    return fcdata.wifi.ip;
}

// TEXT
void FC::setText1(bool isText, const char *text)
{
    fcdata.text.isText1 = isText;
    strcpy(fcdata.text.text1, text);
}

bool FC::getIsText1()
{
    return fcdata.text.isText1;
}

char *FC::getText1()
{
    return fcdata.text.text1;
}

void FC::setText2(bool isText, const char *text)
{
    fcdata.text.isText2 = isText;
    strcpy(fcdata.text.text2, text);
}

bool FC::getIsText2()
{
    return fcdata.text.isText2;
}

char *FC::getText2()
{
    return fcdata.text.text2;
}

void FC::setText3(bool isText, const char *text)
{
    fcdata.text.isText3 = isText;
    strcpy(fcdata.text.text3, text);
}

bool FC::getIsText3()
{
    return fcdata.text.isText3;
}

char *FC::getText3()
{
    return fcdata.text.text3;
}

void FC::setText4(bool isText, const char *text)
{
    fcdata.text.isText4 = isText;
    strcpy(fcdata.text.text4, text);
}

bool FC::getIsText4()
{
    return fcdata.text.isText4;
}

char *FC::getText4()
{
    return fcdata.text.text4;
}

void FC::setText5(bool isText, const char *text)
{
    fcdata.text.isText5 = isText;
    strcpy(fcdata.text.text5, text);
}

bool FC::getIsText5()
{
    return fcdata.text.isText5;
}

char *FC::getText5()
{
    return fcdata.text.text5;
}

void FC::setText6(bool isText, const char *text)
{
    fcdata.text.isText6 = isText;
    strcpy(fcdata.text.text6, text);
}

bool FC::getIsText6()
{
    return fcdata.text.isText6;
}

char *FC::getText6()
{
    return fcdata.text.text6;
}

void FC::setText7(bool isText, const char *text)
{
    fcdata.text.isText7 = isText;
    strcpy(fcdata.text.text7, text);
}

bool FC::getIsText7()
{
    return fcdata.text.isText7;
}

char *FC::getText7()
{
    return fcdata.text.text7;
}

void FC::setText8(bool isText, const char *text)
{
    fcdata.text.isText8 = isText;
    strcpy(fcdata.text.text8, text);
}

bool FC::getIsText8()
{
    return fcdata.text.isText8;
}

char *FC::getText8()
{
    return fcdata.text.text8;
}

// GPS
void FC::setGpsIsFixed(bool isFixed, uint32_t isFixedTimestamp)
{
    if (!fcdata.gps.isFixed && isFixed)
    {
        _notifyObserver(GPS_FIXED);
    }
    else if (fcdata.gps.isFixed && !isFixed)
    {
        _notifyObserver(GPS_LOST_FIXED);
    }

    fcdata.gps.isFixed = isFixed;
    fcdata.gps.isFixedTimestamp = isFixedTimestamp;
}

bool FC::getGpsIsFixed()
{
    return fcdata.gps.isFixed;
}

uint32_t FC::getGpsIsFixedTimestamp()
{
    return fcdata.gps.isFixedTimestamp;
}

void FC::setGpsLocation(double locLat, double locLon, uint32_t locTimestamp)
{
    fcdata.gps.locLat = locLat;
    fcdata.gps.locLon = locLon;
    fcdata.gps.locTimestamp = locTimestamp;

    if (fcdata.gps.locTimestamp > (millis() - TIMEOUT_DATA))
    {
        _notifyObserver(GPS_NEW_POSITION);
        setGpsIsFixed(true, locTimestamp);
    }
    else
    {
        _notifyObserver(GPS_NO_POSITION);
        setGpsIsFixed(false, 0);
    }
}

void FC::setGpsLocTimestamp(uint32_t locTimestamp)
{
    fcdata.gps.locTimestamp = locTimestamp;
    if (fcdata.gps.locTimestamp > (millis() - TIMEOUT_DATA))
    {
        setGpsIsFixed(true, locTimestamp);
    }
    else
    {
        setGpsIsFixed(false, 0);
    }
}

double FC::getGpsLat()
{
    return fcdata.gps.locLat;
}

double FC::getGpsLon()
{
    return fcdata.gps.locLon;
}

uint32_t FC::getGpsLocTimestamp()
{
    return fcdata.gps.locTimestamp;
}

void FC::setGpsDate(uint8_t dateDay, uint8_t dateMonth, uint8_t dateYear, uint32_t dateTimestamp)
{

    fcdata.gps.dateDay = dateDay;
    fcdata.gps.dateMonth = dateMonth;
    fcdata.gps.dateYear = dateYear;
    fcdata.gps.dateTimestamp = dateTimestamp;
}

void FC::setGpsDateTimestamp(uint32_t dateTimestamp)
{
    fcdata.gps.dateTimestamp = dateTimestamp;
}

uint8_t FC::getGpsDateDay()
{
    return fcdata.gps.dateDay;
}

uint8_t FC::getGpsDateMonth()
{
    return fcdata.gps.dateMonth;
}

uint8_t FC::getGpsDateYear()
{
    return fcdata.gps.dateYear;
}

uint32_t FC::getDateTimestamp()
{
    return fcdata.gps.dateTimestamp;
}

void FC::setGpsTimeUTC(uint8_t timeHour, uint8_t timeMinute, uint8_t timeSecond, uint32_t timeTimestamp)
{
    bool newTime = false;
    if (timeHour != fcdata.gps.timeHour || timeMinute != fcdata.gps.timeMinute || timeSecond != fcdata.gps.timeSecond)
    {
        newTime = true;
    }

    fcdata.gps.timeHour = timeHour;
    fcdata.gps.timeMinute = timeMinute;
    fcdata.gps.timeSecond = timeSecond;
    fcdata.gps.timeTimestamp = timeTimestamp;

    if (newTime)
    {
        _notifyObserver(GPS_NEW_TIME);
    }
}

void FC::setGpsTimeTimestamp(uint32_t timeTimestamp)
{
    fcdata.gps.timeTimestamp = timeTimestamp;
}

uint8_t FC::getGpsTimeHour()
{
    return fcdata.gps.timeHour + tzn;
}

uint8_t FC::getGpsTimeHourUTC()
{
    return fcdata.gps.timeHour;
}

uint8_t FC::getGpsTimeMinute()
{
    return fcdata.gps.timeMinute;
}

uint8_t FC::getGpsTimeSecond()
{
    return fcdata.gps.timeSecond;
}

uint32_t FC::getGpsTimeTimestamp()
{
    return fcdata.gps.timeTimestamp;
}

void FC::setGpsKmph(double kmph, uint32_t kmphTimestamp)
{
    fcdata.gps.kmph = kmph;
    fcdata.gps.kmphTimestamp = kmphTimestamp;
    checkFlightStart(false);
    _notifyObserver(GPS_NEW_SPEED);
}

void FC::setGpsKmphTimestamp(uint32_t kmphTimestamp)
{
    fcdata.gps.kmphTimestamp = kmphTimestamp;
}

double FC::getGpsKmph()
{
    return fcdata.gps.kmph;
}

uint32_t FC::getGpsKmphTimestamp()
{
    return fcdata.gps.kmphTimestamp;
}

void FC::setGpsHeading(int32_t headingDeg, const char *headingTxt, uint32_t headingDegTimestamp)
{
    fcdata.gps.headingDeg = headingDeg;
    strcpy(fcdata.gps.headingTxt, headingTxt);
    fcdata.gps.headingDegTimestamp = headingDegTimestamp;
}

void FC::setGpsHeadingTimestamp(uint32_t headingDegTimestamp)
{
    fcdata.gps.headingDegTimestamp = headingDegTimestamp;
}

int32_t FC::getGpsHeadingDeg()
{
    return fcdata.gps.headingDeg;
}

char *FC::getGpsHeadingTxt()
{
    return fcdata.gps.headingTxt;
}

uint32_t FC::getGpsHeadingDegTimestamp()
{
    return fcdata.gps.headingDegTimestamp;
}

void FC::setGpsAltiMeters(double altiMeters, uint32_t altiMetersTimestamp)
{
    fcdata.gps.altiMeters = altiMeters;
    fcdata.gps.altiMetersTimestamp = altiMetersTimestamp;

    if (fcdata.gps.altiMetersTimestamp > (millis() - TIMEOUT_DATA))
    {
        _notifyObserver(GPS_NEW_ALTI_METERS);
    }
}

void FC::setGpsAltiMetersTimestamp(uint32_t altiMetersTimestamp)
{
    fcdata.gps.altiMetersTimestamp = altiMetersTimestamp;
}

double FC::getGpsAltiMeters()
{
    return fcdata.gps.altiMeters;
}

uint32_t FC::getGpsAltiMetersTimestamp()
{
    return fcdata.gps.altiMetersTimestamp;
}

void FC::setGpsHdop(double hdop, uint32_t hdopTimestamp)
{
    fcdata.gps.hdop = hdop;
    fcdata.gps.hdopTimestamp = hdopTimestamp;
}

void FC::setGpsHdopTimestamp(uint32_t hdopTimestamp)
{
    fcdata.gps.hdopTimestamp = hdopTimestamp;
}
double FC::getGpsHdop()
{
    return fcdata.gps.hdop;
}

uint32_t FC::getGpsHdopTimestamp()
{
    return fcdata.gps.hdopTimestamp;
}

void FC::setGpsSatellitesCount(uint32_t satellitesCount, uint32_t satellitesTimestamp)
{
    fcdata.gps.satellitesCount = satellitesCount;
    fcdata.gps.satellitesTimestamp = satellitesTimestamp;
}

void FC::setGpsSatellitesCountTimestamp(uint32_t satellitesTimestamp)
{
    fcdata.gps.satellitesTimestamp = satellitesTimestamp;
}

uint32_t FC::getGpsSatellitesCount()
{
    return fcdata.gps.satellitesCount;
}

uint32_t FC::getGpsSatellitesTimestamp()
{
    return fcdata.gps.satellitesTimestamp;
}

void FC::setGpsSentence(const char *sentence, uint32_t gpsSentenceTimestamp)
{

    // Serial.println("Tentative ajout d'une phrase GPS");
    // Serial.println(sentence);
    if ((strncmp(sentence, "$GPGGA", 6) == 0) || (strncmp(sentence, "$GNGGA", 6) == 0) || (strncmp(sentence, "$GPRMC", 6) == 0) || (strncmp(sentence, "$GNRMC", 6) == 0))
    {
        // Serial.println("Ajout d'une phrase GPS");

        strcpy(fcdata.gps.sentence, sentence);
        fcdata.gps.sentenceTimestamp = gpsSentenceTimestamp;
        // Serial.println(fcdata.gps.sentence);
        // Serial.println(fcdata.gps.sentenceTimestamp);
    }
}

char *FC::getGpsSentence()
{
    return fcdata.gps.sentence;
}

uint32_t FC::getGpsSentenceTimestamp()
{
    return fcdata.gps.sentenceTimestamp;
}

// AGL
void FC::setAglAlt(int aglAlt, uint32_t aglTimestamp)
{
    fcdata.agl.aglAlt = aglAlt;
    fcdata.agl.aglAltTimestamp = aglTimestamp;
}

int FC::getAglAlt()
{
    return fcdata.agl.aglAlt;
}

uint32_t FC::getAglAltTimestamp()
{
    return fcdata.agl.aglAltTimestamp;
}

void FC::setAglGroundLvl(int groundLvl, uint32_t aglGroundLvlTimestamp)
{
    fcdata.agl.groundLvl = groundLvl;
    fcdata.agl.aglGroundLvlTimestamp = aglGroundLvlTimestamp;
}

int FC::getAglGroundLvl()
{
    return fcdata.agl.groundLvl;
}

uint32_t FC::getAglGroundLvlTimestamp()
{
    return fcdata.agl.aglGroundLvlTimestamp;
}

bool FC::getIsFlightStart()
{
    return fcdata.flight.isFlightStart;
}

uint32_t FC::getFlightDurationSecond()
{
    return (fcdata.gps.timeHour * 60 * 60 + fcdata.gps.timeMinute * 60 + fcdata.gps.timeSecond) - (fcdata.flight.flightStartTime[0] * 60 * 60 + fcdata.flight.flightStartTime[1] * 60 + fcdata.flight.flightStartTime[2]);
}

uint8_t FC::getFlightTimeDurationHour()
{
    return getFlightDurationSecond() / 60 / 60;
}

uint8_t FC::getFlightTimeDurationMinute()
{
    return getFlightDurationSecond() / 60 - getFlightTimeDurationHour() * 60;
}

uint8_t FC::getFlightTimeDurationSecond()
{
    return getFlightDurationSecond() - getFlightTimeDurationMinute() * 60 - getFlightTimeDurationHour() * 60 * 60;
}

void FC::setWind(int8_t _speed, uint16_t heading, int8_t _meanSpeed, uint32_t timestamp)
{
    fcdata.wind.speed = _speed;
    fcdata.wind.heading = heading;
    fcdata.wind.meanSpeed = _meanSpeed;
    fcdata.wind.windTimestamp = timestamp;
}

int8_t FC::getWindSpeed()
{
    return fcdata.wind.speed;
}

uint16_t FC::getWindHeading()
{
    return fcdata.wind.heading;
}

int8_t FC::getWindMeanSpeed()
{
    return fcdata.wind.meanSpeed;
}

uint32_t FC::getWindTimestamp()
{
    return fcdata.wind.windTimestamp;
}
