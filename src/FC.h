#pragma once

#include <Arduino.h>
#include "Observer/Subject.h"
struct power_data
{
    int capacite = 0;
    uint32_t capaciteTimestamp = 0;
    float tension = .0;
    uint32_t tensionTimestamp = 0;
};

struct sound_data
{
    bool isMute;
    float volume;
};

struct vario_data
{
    int16_t alti;
    uint32_t altiTimestamp = 0;
    float velocity;
    uint32_t velocityTimestamp = 0;
    int16_t bearing;
    char bearingTxt[3];
    uint32_t bearingTimestamp = 0;
};

struct wifi_data
{
    bool connected = false;
    char ssid[32];
    char ip[16];
};

struct text_data
{
    bool isText1 = false;
    char text1[50];
    bool isText2 = false;
    char text2[50];
    bool isText3 = false;
    char text3[50];
    bool isText4 = false;
    char text4[50];
    bool isText5 = false;
    char text5[50];
    bool isText6 = false;
    char text6[50];
    bool isText7 = false;
    char text7[50];
    bool isText8 = false;
    char text8[50];
};

struct gps_data
{
    bool isFixed = false;
    uint32_t isFixedTimestamp = 0;

    double locLat = -999;
    double locLon = -999;
    uint32_t locTimestamp = 0;

    uint8_t dateDay;
    uint8_t dateMonth;
    uint8_t dateYear;
    uint32_t dateTimestamp = 0;

    uint8_t timeHour;
    uint8_t timeMinute;
    uint8_t timeSecond;
    uint32_t timeTimestamp = 0;

    double kmph;
    uint32_t kmphTimestamp = 0;

    int32_t headingDeg;
    char headingTxt[3];
    uint32_t headingDegTimestamp = 0;

    double altiMeters;
    uint32_t altiMetersTimestamp = 0;

    double hdop;
    uint32_t hdopTimestamp = 0;

    uint32_t satellitesCount;
    uint32_t satellitesTimestamp = 0;
};

struct agl_data
{
    int aglAlt = -1;
    uint32_t aglAltTimestamp = 0;
    int groundLvl = 0;
    uint32_t aglGroundLvlTimestamp = 0;
};

struct flight_data
{
    bool isFlightStart = false;
    uint32_t flightStartFistTimestamp;
    int8_t flightStartTime[3] = {0, 0, 0};
};

struct wifi_data
{
    bool connected = false;
    char ssid[32];
    char ip[16];
};

struct text_data
{
    bool isText1 = false;
    char text1[50];
    bool isText2 = false;
    char text2[50];
    bool isText3 = false;
    char text3[50];
    bool isText4 = false;
    char text4[50];
    bool isText5 = false;
    char text5[50];
    bool isText6 = false;
    char text6[50];
    bool isText7 = false;
    char text7[50];
    bool isText8 = false;
    char text8[50];
};

struct fcdata_t
{
    vario_data vario;
    power_data power;
    sound_data sound;
    wifi_data wifi;
    text_data text;
    gps_data gps;
    agl_data agl;
    flight_data flight;
};

class FC : public Subject
{
private:
    fcdata_t fcdata;
    int8_t tzn;

public:
    FC();

    void setTzn(int8_t tzn);

    void checkFlightStart(bool forceManualStart);

    // POWER
    void setPowerCapacite(int capacite, uint32_t capaciteTimestamp);
    int getPowerCapacite();
    uint32_t getPowerCapaciteTimestamp();

    void setPowerTension(float tension, uint32_t tensionTimestamp);
    float getPowerTension();
    uint32_t getPowerTensionTimestamp();

    // SOUND
    void setSoundIsMute(bool isMute);
    void setSoundVolume(float volume);
    bool getSoundIsMute();
    float getSoundVolume();

    // VARIO
    void setVarioAlti(int16_t alti, uint32_t altiTimestamp);
    int16_t getVarioAlti();
    uint32_t getVarioAltiTimestamp();

    void setVarioVelocity(float velocity, uint32_t velocityTimestamp);
    float getVarioVelocity();
    uint32_t getVarioVelocityTimestamp();

    void setVarioBearing(int16_t bearing, char bearingTxt[3], uint32_t bearingTimestamp);
    int16_t getVarioBearing();
    char *getVarioBearingTxt();
    uint32_t getVarioBearingTimestamp();

    // WIFI
    void setWifiConnected(bool connected);
    void setWifiSsid(char ssid[32]);
    void setWifiIp(char ip[16]);
    bool getWifiConnected();
    char *getWifiSsid();
    char *getWifiIp();

    // TEXT
    void setText1(bool isText, const char *text);
    bool getIsText1();
    char *getText1();

    void setText2(bool isText, const char *text);
    bool getIsText2();
    char *getText2();

    void setText3(bool isText, const char *text);
    bool getIsText3();
    char *getText3();

    void setText4(bool isText, const char *text);
    bool getIsText4();
    char *getText4();

    void setText5(bool isText, const char *text);
    bool getIsText5();
    char *getText5();

    void setText6(bool isText, const char *text);
    bool getIsText6();
    char *getText6();

    void setText7(bool isText, const char *text);
    bool getIsText7();
    char *getText7();

    void setText8(bool isText, const char *text);
    bool getIsText8();
    char *getText8();

    // GPS
    void setGpsIsFixed(bool isFixed, uint32_t isFixedTimestamp);
    bool getGpsIsFixed();
    uint32_t getGpsIsFixedTimestamp();

    void setGpsLocation(double locLat, double locLon, uint32_t locTimestamp);
    void setGpsLocTimestamp(uint32_t locTimestamp);
    double getGpsLat();
    double getGpsLon();
    uint32_t getGpsLocTimestamp();

    void setGpsDate(uint8_t dateDay, uint8_t dateMonth, uint8_t dateYear, uint32_t dateTimestamp);
    void setGpsDateTimestamp(uint32_t dateTimestamp);
    uint8_t getGpsDateDay();
    uint8_t getGpsDateMonth();
    uint8_t getGpsDateYear();
    uint32_t getDateTimestamp();

    void setGpsTimeUTC(uint8_t timeHour, uint8_t timeMinute, uint8_t timeSecond, uint32_t timeTimestamp);
    void setGpsTimeTimestamp(uint32_t timeTimestamp);
    uint8_t getGpsTimeHour();
    uint8_t getGpsTimeHourUTC();
    uint8_t getGpsTimeMinute();
    uint8_t getGpsTimeSecond();
    uint32_t getGpsTimeTimestamp();

    void setGpsKmph(double kmph, uint32_t kmphTimestamp);
    void setGpsKmphTimestamp(uint32_t kmphTimestamp);
    double getGpsKmph();
    uint32_t getGpsKmphTimestamp();

    void setGpsHeading(int32_t headingDeg, const char *headingTxt, uint32_t headingDegTimestamp);
    void setGpsHeadingTimestamp(uint32_t headingDegTimestamp);
    int32_t getGpsHeadingDeg();
    char *getGpsHeadingTxt();
    uint32_t getGpsHeadingDegTimestamp();

    void setGpsAltiMeters(double altiMeters, uint32_t altiMetersTimestamp);
    void setGpsAltiMetersTimestamp(uint32_t altiMetersTimestamp);
    double getGpsAltiMeters();
    uint32_t getGpsAltiMetersTimestamp();

    void setGpsHdop(double hdop, uint32_t hdopTimestamp);
    void setGpsHdopTimestamp(uint32_t hdopTimestamp);
    double getGpsHdop();
    uint32_t getGpsHdopTimestamp();

    void setGpsSatellitesCount(uint32_t satellitesCount, uint32_t satellitesTimestamp);
    void setGpsSatellitesCountTimestamp(uint32_t satellitesTimestamp);
    uint32_t getGpsSatellitesCount();
    uint32_t getGpsSatellitesTimestamp();

    // AGL
    void setAglAlt(int aglAlt, uint32_t aglAltTimestamp);
    int getAglAlt();
    uint32_t getAglAltTimestamp();
    void setAglGroundLvl(int groundLvl, uint32_t aglGroundLvlTimestamp);
    int getAglGroundLvl();
    uint32_t getAglGroundLvlTimestamp();

    bool getIsFlightStart();
    uint32_t getFlightDurationSecond();
    uint8_t getFlightTimeDurationHour();
    uint8_t getFlightTimeDurationMinute();
    uint8_t getFlightTimeDurationSecond();
};