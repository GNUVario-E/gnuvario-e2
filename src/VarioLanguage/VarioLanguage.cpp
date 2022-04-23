#include "VarioLanguage.h"
#include <SD.h>
#include "VarioDebug/VarioDebug.h"
#include "VarioTool/VarioTool.h"

#define PARAMS_VERSION_LANGUE "1.0"

#define DEFAULT_TITRE_TIME "Heure"
#define DEFAULT_TITRE_TDV "Temps de vol"
#define DEFAULT_TITRE_ALTI "Alti"
#define DEFAULT_TITRE_AGL "Alti Sol"
#define DEFAULT_TITRE_VARIO "Vario"
#define DEFAULT_TITRE_SPEED "Vitesse"
#define DEFAULT_TITRE_FINESSE "Finesse"
#define DEFAULT_TITRE_TCHUTE "T Chute"
#define DEFAULT_TITRE_CAP "Cap"
#define DEFAULT_TITRE_LAT "Lat"
#define DEFAULT_TITRE_LONG "Long"
#define DEFAULT_TITRE_COMPAS "Compas"
#define DEFAULT_TITRE_STAT "Statistique"
#define DEFAULT_TITRE_DATE "Date"
#define DEFAULT_TITRE_HEURE "Heure"
#define DEFAULT_TITRE_DUREE "Duree"
#define DEFAULT_TITRE_VITESSE "Vitesse"
#define DEFAULT_TITRE_CONNECT "Connexion"
#define DEFAULT_TITRE_CONNECTA "Connexion a "
#define DEFAULT_TITRE_DEMAR "Demarrage"
#define DEFAULT_TITRE_REDEMAR "Redemarrage"
#define DEFAULT_TITRE_ENCOURS "en cours"
#define DEFAULT_TITRE_CALIBR "Calibration"
#define DEFAULT_TITRE_VEILLE "En veille"
#define DEFAULT_TITRE_CHARGE "en charge"
#define DEFAULT_TITRE_CHARGER "chargee"
#define DEFAULT_TITRE_BATTERIE "Batterie"
#define DEFAULT_TITRE_STAT_DUREE "Duree"
#define DEFAULT_TITRE_STAT_SPEED "Vitesse"

void VarioLanguage::init(uint8_t language)
{
	char tmpchar[16] = "/gnuvario";

	switch (language)
	{
	case 0:
		strcat(tmpchar, "FR");
		break;
	case 1:
		strcat(tmpchar, "EN");
		break;
	default:
		strcat(tmpchar, "FR");
		break;
	}
	strcat(tmpchar, ".jso");
	loadConfigurationLangue(tmpchar);
}

void VarioLanguage::loadConfigurationLangue(const char *filename)
{
	// Open file for reading
	boolean isFileLangOK = true;

	File file = SD.open(filename);
	if (!file)
	{
		VARIO_SDCARD_DEBUG_PRINTLN("Failed to read file");
		setDefaultValue();
		isFileLangOK = false;

		//création du fichier
		saveConfigurationVario(PARAMS_VERSION_LANGUE, filename);
		return;
	}

	VARIO_SDCARD_DEBUG_PRINTLN("deserialisation");

	VarioTool::jsonDoc.clear();

	// Deserialize the JSON document
	DeserializationError error = deserializeJson(VarioTool::jsonDoc, file);

	if (error)
	{
		VARIO_SDCARD_DEBUG_PRINT("deserializeJson() failed: ");
		VARIO_SDCARD_DEBUG_PRINTLN(error.c_str());

		VARIO_SDCARD_DEBUG_PRINT("Failed to read file :");
		VARIO_SDCARD_DEBUG_PRINT(filename);
		VARIO_SDCARD_DEBUG_PRINTLN(", using default configuration");

		setDefaultValue();
		isFileLangOK = false;

		return;
	}

	String tmpValueString;

	VARIO_SDCARD_DEBUG_PRINTLN("Systeme : ");

	const char *GnuvarioE_version_langue = VarioTool::jsonDoc["gnuvarioe"]["version"]; // "1.0"
	if (strcmp(GnuvarioE_version_langue, PARAMS_VERSION_LANGUE) != 0)
	{
		isFileLangOK = false;
	}
	//*****    SYSTEME *****

	VARIO_SDCARD_DEBUG_PRINTLN("****** Titre *******");

	JsonObject Titre = VarioTool::jsonDoc["titre"];

	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "TIME", TITRE_TIME, MAX_CAR_TITRE_TIME);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "TDV", TITRE_TDV, MAX_CAR_TITRE_TDV);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "ALTI", TITRE_ALTI, MAX_CAR_TITRE_ALTI);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "AGL", TITRE_AGL, MAX_CAR_TITRE_AGL);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "VARIO", TITRE_VARIO, MAX_CAR_TITRE_VARIO);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "SPEED", TITRE_SPEED, MAX_CAR_TITRE_SPEED);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "FINESSE", TITRE_FINESSE, MAX_CAR_TITRE_FINESSE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "TCHUTE", TITRE_TCHUTE, MAX_CAR_TITRE_TCHUTE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "CAP", TITRE_CAP, MAX_CAR_TITRE_CAP);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "LAT", TITRE_LAT, MAX_CAR_TITRE_LAT);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "LONG", TITRE_LONG, MAX_CAR_TITRE_LONG);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Titre, "COMPAS", TITRE_COMPAS, MAX_CAR_TITRE_COMPAS);

	//*****    MESSAGE *****

	VARIO_SDCARD_DEBUG_PRINTLN("****** Message *******");

	JsonObject Message = VarioTool::jsonDoc["message"];

	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "STAT", TITRE_STAT, MAX_CAR_TITRE_STAT);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "DATE", TITRE_DATE, MAX_CAR_TITRE_DATE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "HEURE", TITRE_HEURE, MAX_CAR_TITRE_HEURE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "DUREE", TITRE_DUREE, MAX_CAR_TITRE_DUREE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "VITESSE", TITRE_VITESSE, MAX_CAR_TITRE_VITESSE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "CONNECT", TITRE_CONNECT, MAX_CAR_TITRE_CONNECT);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "CONNECTA", TITRE_CONNECTA, MAX_CAR_TITRE_CONNECTA);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "DEMAR", TITRE_DEMAR, MAX_CAR_TITRE_DEMAR);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "REDEMAR", TITRE_REDEMAR, MAX_CAR_TITRE_REDEMAR);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "ENCOURS", TITRE_ENCOURS, MAX_CAR_TITRE_ENCOURS);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "CALIBR", TITRE_CALIBR, MAX_CAR_TITRE_CALIBR);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "VEILLE", TITRE_VEILLE, MAX_CAR_TITRE_VEILLE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "CHARGE", TITRE_CHARGE, MAX_CAR_TITRE_CHARGE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "CHARGER", TITRE_CHARGER, MAX_CAR_TITRE_CHARGER);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&Message, "BATTERIE", TITRE_BATTERIE, MAX_CAR_TITRE_BATTERIE);

	VARIO_SDCARD_DEBUG_PRINTLN("****** Stat *******");

	JsonObject State = VarioTool::jsonDoc["stat"];

	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&State, "DUREE", TITRE_STAT_DUREE, MAX_TITRE_STAT_DUREE);
	isFileLangOK = isFileLangOK && setParameterFromJsonObject(&State, "SPEED", TITRE_STAT_SPEED, MAX_TITRE_STAT_SPEED);

	// Close the file (Curiously, File's destructor doesn't close the file)
	file.close();

	//Mise à jour du fichier de langue
	if (!isFileLangOK)
	{
		VARIO_SDCARD_DEBUG_PRINTLN("Sauvegarde de nouveaux paramètres");
		saveConfigurationVario(GnuvarioE_version_langue, filename);
	}

	VarioTool::jsonDoc.clear();
}

// Saves the configuration to a file
void VarioLanguage::saveConfigurationVario(const char *version, const char *filename)
{
	VarioTool::jsonDoc.clear();
	// Delete existing file, otherwise the configuration is appended to the file
	SD.remove(filename);

	// Open file for writing
	File file = SD.open(filename, FILE_WRITE);
	if (!file)
	{
		VARIO_SDCARD_DEBUG_PRINTLN("Failed to create file");
		return;
	}

	VARIO_SDCARD_DEBUG_PRINT("****** SAUVEGARDE ");
	VARIO_SDCARD_DEBUG_PRINT(filename);
	VARIO_SDCARD_DEBUG_PRINTLN(" *******");

	// Set the values in the document
	JsonObject GnuvarioE = VarioTool::jsonDoc.createNestedObject("gnuvarioe");
	GnuvarioE["version"] = version;

	//*****    SYSTEME *****

	VARIO_SDCARD_DEBUG_PRINTLN("****** Titre *******");

	JsonObject Titre = VarioTool::jsonDoc.createNestedObject("titre");

	Titre["TIME"] = TITRE_TAB[TITRE_TIME];
	Titre["TDV"] = TITRE_TAB[TITRE_TDV];
	Titre["ALTI"] = TITRE_TAB[TITRE_ALTI];
	Titre["AGL"] = TITRE_TAB[TITRE_AGL];
	Titre["VARIO"] = TITRE_TAB[TITRE_VARIO];
	Titre["SPEED"] = TITRE_TAB[TITRE_SPEED];
	Titre["FINESSE"] = TITRE_TAB[TITRE_FINESSE];
	Titre["TCHUTE"] = TITRE_TAB[TITRE_TCHUTE];
	Titre["CAP"] = TITRE_TAB[TITRE_CAP];
	Titre["LAT"] = TITRE_TAB[TITRE_LAT];
	Titre["LONG"] = TITRE_TAB[TITRE_LONG];
	Titre["COMPAS"] = TITRE_TAB[TITRE_COMPAS];

	//*****    Message *****

//	Serial.println("****** Message *******");
	VARIO_SDCARD_DEBUG_PRINTLN("****** Message *******");

	JsonObject Message = VarioTool::jsonDoc.createNestedObject("message");
	Message["STAT"] = TITRE_TAB[TITRE_STAT];
	Message["DATE"] = TITRE_TAB[TITRE_DATE];
	Message["HEURE"] = TITRE_TAB[TITRE_HEURE];
	Message["DUREE"] = TITRE_TAB[TITRE_DUREE];
	Message["VITESSE"] = TITRE_TAB[TITRE_VITESSE];
	Message["CONNECT"] = TITRE_TAB[TITRE_CONNECT];
	Message["CONNECTA"] = TITRE_TAB[TITRE_CONNECTA];
	Message["DEMAR"] = TITRE_TAB[TITRE_DEMAR];
	Message["REDEMAR"] = TITRE_TAB[TITRE_REDEMAR];
	Message["ENCOURS"] = TITRE_TAB[TITRE_ENCOURS];
	Message["CALIBR"] = TITRE_TAB[TITRE_CALIBR];
	Message["VEILLE"] = TITRE_TAB[TITRE_VEILLE];
	Message["CHARGE"] = TITRE_TAB[TITRE_CHARGE];
	Message["CHARGER"] = TITRE_TAB[TITRE_CHARGER];
	Message["BATTERIE"] = TITRE_TAB[TITRE_BATTERIE];
	//*****    Stat *****

//	Serial.println("****** Stat *******");
	VARIO_SDCARD_DEBUG_PRINTLN("****** Stat *******");

	JsonObject State = VarioTool::jsonDoc.createNestedObject("stat");
	State["DUREE"] = TITRE_TAB[TITRE_DUREE];
	State["SPEED"] = TITRE_TAB[TITRE_SPEED];

	// Serialize JSON to file
	if (serializeJson(VarioTool::jsonDoc, file) == 0)
	{
//		Serial.println(F("Failed to write to file"));
		VARIO_SDCARD_DEBUG_PRINTLN("Failed to write to file");
	}

	// Close the file
	file.close();
	// Clearing Buffer
	VarioTool::jsonDoc.clear();
}

//**********************************************************
String VarioLanguage::getText(uint8_t value)
{
	//**********************************************************
	return TITRE_TAB[value];
}

void VarioLanguage::setDefaultValue()
{
	strcpy(TITRE_TAB[TITRE_TIME], DEFAULT_TITRE_TIME);
	strcpy(TITRE_TAB[TITRE_TDV], DEFAULT_TITRE_TDV);
	strcpy(TITRE_TAB[TITRE_ALTI], DEFAULT_TITRE_ALTI);
	strcpy(TITRE_TAB[TITRE_AGL], DEFAULT_TITRE_AGL);
	strcpy(TITRE_TAB[TITRE_VARIO], DEFAULT_TITRE_VARIO);
	strcpy(TITRE_TAB[TITRE_SPEED], DEFAULT_TITRE_SPEED);
	strcpy(TITRE_TAB[TITRE_FINESSE], DEFAULT_TITRE_FINESSE);
	strcpy(TITRE_TAB[TITRE_TCHUTE], DEFAULT_TITRE_TCHUTE);
	strcpy(TITRE_TAB[TITRE_CAP], DEFAULT_TITRE_CAP);
	strcpy(TITRE_TAB[TITRE_LAT], DEFAULT_TITRE_LAT);
	strcpy(TITRE_TAB[TITRE_LONG], DEFAULT_TITRE_LONG);
	strcpy(TITRE_TAB[TITRE_COMPAS], DEFAULT_TITRE_COMPAS);
	strcpy(TITRE_TAB[TITRE_STAT], DEFAULT_TITRE_STAT);
	strcpy(TITRE_TAB[TITRE_DATE], DEFAULT_TITRE_DATE);
	strcpy(TITRE_TAB[TITRE_HEURE], DEFAULT_TITRE_HEURE);
	strcpy(TITRE_TAB[TITRE_DUREE], DEFAULT_TITRE_DUREE);
	strcpy(TITRE_TAB[TITRE_VITESSE], DEFAULT_TITRE_VITESSE);
	strcpy(TITRE_TAB[TITRE_CONNECT], DEFAULT_TITRE_CONNECT);
	strcpy(TITRE_TAB[TITRE_CONNECTA], DEFAULT_TITRE_CONNECTA);
	strcpy(TITRE_TAB[TITRE_DEMAR], DEFAULT_TITRE_DEMAR);
	strcpy(TITRE_TAB[TITRE_REDEMAR], DEFAULT_TITRE_REDEMAR);
	strcpy(TITRE_TAB[TITRE_ENCOURS], DEFAULT_TITRE_ENCOURS);
	strcpy(TITRE_TAB[TITRE_CALIBR], DEFAULT_TITRE_CALIBR);
	strcpy(TITRE_TAB[TITRE_VEILLE], DEFAULT_TITRE_VEILLE);
	strcpy(TITRE_TAB[TITRE_CHARGE], DEFAULT_TITRE_CHARGE);
	strcpy(TITRE_TAB[TITRE_CHARGER], DEFAULT_TITRE_CHARGER);
	strcpy(TITRE_TAB[TITRE_BATTERIE], DEFAULT_TITRE_BATTERIE);
	strcpy(TITRE_TAB[TITRE_STAT_DUREE], DEFAULT_TITRE_STAT_DUREE);
	strcpy(TITRE_TAB[TITRE_STAT_SPEED], DEFAULT_TITRE_STAT_SPEED);
}

bool VarioLanguage::setParameterFromJsonObject(JsonObject *section, const char *key, uint8_t index, uint8_t maxSize)
{
	bool isFromJson = false;
	const char *tVal;
	if (section->containsKey(key))
	{
		if ((tVal = section->getMember(key).as<const char *>()) != NULL)
		{
			strncpy(TITRE_TAB[index], tVal, maxSize);
			TITRE_TAB[index][maxSize] = 0;
			isFromJson = true;
		}
	}

	if (isFromJson)
	{
		VARIO_SDCARD_DEBUG_PRINT("Json Recup - ");
		VARIO_SDCARD_DEBUG_PRINT(key);
		VARIO_SDCARD_DEBUG_PRINT(" : ");
		VARIO_SDCARD_DEBUG_PRINTLN(tVal);
	}
	else
	{
		VARIO_SDCARD_DEBUG_PRINTLN("NO Recup - ");
		VARIO_SDCARD_DEBUG_PRINTLN(key);
	}

	return isFromJson;
}
/*

{
    "gnuvarioe": {
        "version": "1.0"
    },
		"language": {
				"id": 0,
				"text": "Français"
		},
    "titre": {
				"TIME":  "Heure",
				"TDV":   "Temps de vol",
				"ALTI":  "Alti",
				"AGL":   "Alti Sol",
				"VARIO": "Vario",
				"SPEED": "Vitesse",
				"FINESSE": "Finesse",
				"TCHUTE": "T Chute",
				"CAP": "Cap",
				"LAT": "Lat",
				"LONG": "Long",
				"COMPAS": "Compas"
    },
    "message": {
				"STAT": "Statistique",
				"DATE": "Date",
				"HEURE":"Heure",
				"DUREE": "Duree",
				"VITESSE": "Vitesse",
				"CONNECT": "Connection",
				"CONNECTA": "Connection a ",
				"DEMAR": "Demarrage",
				"REDEMAR": "Redemarrage",
				"ENCOURS": "en cours",		
				"CALIBR": "Calibration",
				"VEILLE": "En veille",
				"CHARGE": "En Charge",
				"CHARGER": "Chargée",
				"BATTERIE": "Batterie"
		}
}

*/