#ifndef _VARIO_GPS_H
#define _VARIO_GPS_H

#include "Variometer/SerialNmea/SerialNmea.h"
#include "Variometer/NmeaParser/NmeaParser.h"

class Kalmanvert;

class VarioGps
{
private:
    NmeaParser nmeaParser;

public:
    void init();
    bool update(Kalmanvert *kalmanvert, bool *lastSentence);
};

extern SerialNmea serialNmea;
#endif //_VARIO_GPS_H