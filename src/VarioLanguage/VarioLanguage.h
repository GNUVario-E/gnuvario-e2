#ifndef VARIO_LANGUAGE_H
#define VARIO_LANGUAGE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define FRENCH 0
#define ENGLISH 1

#define TITRE_TIME 0
#define TITRE_TDV 1
#define TITRE_ALTI 2
#define TITRE_AGL 3
#define TITRE_VARIO 4
#define TITRE_SPEED 5
#define TITRE_FINESSE 6
#define TITRE_TCHUTE 7
#define TITRE_CAP 8
#define TITRE_LAT 9
#define TITRE_LONG 10
#define TITRE_COMPAS 11
#define TITRE_STAT 12
#define TITRE_DATE 13
#define TITRE_HEURE 14
#define TITRE_DUREE 15
#define TITRE_VITESSE 16
#define TITRE_CONNECT 17
#define TITRE_CONNECTA 18
#define TITRE_DEMAR 19
#define TITRE_REDEMAR 20
#define TITRE_ENCOURS 21
#define TITRE_CALIBR 22
#define TITRE_VEILLE 23
#define TITRE_CHARGE 24
#define TITRE_CHARGER 25
#define TITRE_BATTERIE 26
#define TITRE_STAT_DUREE 27
#define TITRE_STAT_SPEED 28

#define MAX_CAR_TITRE_TIME 15
#define MAX_CAR_TITRE_TDV 15
#define MAX_CAR_TITRE_ALTI 8
#define MAX_CAR_TITRE_AGL 8
#define MAX_CAR_TITRE_VARIO 5
#define MAX_CAR_TITRE_SPEED 7
#define MAX_CAR_TITRE_FINESSE 7
#define MAX_CAR_TITRE_TCHUTE 7
#define MAX_CAR_TITRE_CAP 3
#define MAX_CAR_TITRE_LAT 3
#define MAX_CAR_TITRE_LONG 4
#define MAX_CAR_TITRE_COMPAS 6
#define MAX_CAR_TITRE_STAT 12
#define MAX_CAR_TITRE_DATE 4
#define MAX_CAR_TITRE_HEURE 5
#define MAX_CAR_TITRE_DUREE 11
#define MAX_CAR_TITRE_VITESSE 7
#define MAX_CAR_TITRE_CONNECT 10
#define MAX_CAR_TITRE_CONNECTA 13
#define MAX_CAR_TITRE_DEMAR 9
#define MAX_CAR_TITRE_REDEMAR 11
#define MAX_CAR_TITRE_ENCOURS 11
#define MAX_CAR_TITRE_CALIBR 12
#define MAX_CAR_TITRE_VEILLE 14
#define MAX_CAR_TITRE_CHARGE 9
#define MAX_CAR_TITRE_CHARGER 7
#define MAX_CAR_TITRE_BATTERIE 8
#define MAX_TITRE_STAT_DUREE 10
#define MAX_TITRE_STAT_SPEED 10

class VarioLanguage
{
public:
	void init(uint8_t language = 0);
	void loadConfigurationLangue(const char *filename);
	String getText(uint8_t value);

private:
	uint8_t langue = 0;
	char TITRE_TAB[30][30];
	void setDefaultValue();
	boolean setParameterFromJsonObject(JsonObject *section, const char *key, uint8_t index, uint8_t maxSize);
	void saveConfigurationVario(const char *version, const char *filename);
};

#endif //VARIO_LANGUAGE_H