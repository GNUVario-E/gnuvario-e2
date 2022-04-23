#ifndef VARIO_WEBHANDLER_H
#define VARIO_WEBHANDLER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "esp32FOTA2/src/esp32fota2.h"
#include <ArduinoJson.h>

#include "varioIgcParser/igcdata.h"
#include "VarioIgcParser/VarioIgcParser.h"
#include "VarioSqlFlight/VarioSqlFlight.h"
#include "VarioSqlFlight/VarioSqlFlightHelper.h"

class VarioWebHandler
{
public:
    VarioWebHandler();
    // Récupération de la liste des fichiers du répertoire vols au format json
    AsyncResponseStream *handleListFlights(AsyncWebServerRequest *request);

    //récupération de la liste du contenu de la carte SD
    AsyncResponseStream *handlePrintDirectory(AsyncWebServerRequest *request);

    // Récupération du contenu du fichier parameters.jso
    AsyncWebServerResponse *handleParams(AsyncWebServerRequest *request);

    //telechargement d'un fichier dont le nom complet avec chemin se trouve en param
    AsyncWebServerResponse *handleFileDownload(AsyncWebServerRequest *request);

    //suppression d'un fichier dont le nom complet avec chemin se trouve en param
    AsyncWebServerResponse *handleFileDelete(AsyncWebServerRequest *request);

    // sauvegarde des parametres
    static void handleSaveParams(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

    // mise à jour OTA
    static void handleOtaUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

    // upload d'un fichier, le chemin de destination se trouve dans le nom du fichier posté
    static void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

    //creation d'un fichier ou d'un repertoire
    static void handleFileCreate(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

    //récupération du contenu du fichier wifi
    AsyncWebServerResponse *handleWifi(AsyncWebServerRequest *request);

    // sauvegarde des parametres
    static void handleSaveWifi(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

    //récupération du contenu du fichier preference
    AsyncWebServerResponse *handleWebConfig(AsyncWebServerRequest *request);

    // sauvegarde du contenu du fichier preference
    static void handleSaveWebConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

    // parsage d'un fichier IGC
    AsyncWebServerResponse *handleParseIgc(AsyncWebServerRequest *request);

    // téléchargements de "mes vols" en BDD
    AsyncWebServerResponse *handleGetFlights(AsyncWebServerRequest *request);

    // enregistrement un vol en BDD
    static void handleSetFlight(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

    // suppression d'un vol en BDD
    AsyncWebServerResponse *handleDelFlight(AsyncWebServerRequest *request);

    //recuperation des versions de firmware
    AsyncWebServerResponse *handleFirmwareVersion(AsyncWebServerRequest *request);

    //Mise à jour via internet
    AsyncWebServerResponse *handleUpgradeWeb(AsyncWebServerRequest *request);

    // récupération de la liste des sites
    AsyncWebServerResponse *handleGetSites(AsyncWebServerRequest *request);

    // sauvegarde d'un site
    static void handleSetSite(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

    // suppression d'un site
    AsyncWebServerResponse *handleDelSite(AsyncWebServerRequest *request);

    // récupération du resumé des vols
    AsyncWebServerResponse *getFlightsShort(AsyncWebServerRequest *request);

private:
    static void _doParseIgcAndInsert(void *parameter);
    String getFileSizeStringFromBytes(int bytes);
    void printDirectoryRecurse(AsyncResponseStream *response, String path, boolean isRecursive);
    static void backupFile(String pathOrig, String pathBack);
    int readFlightsData(uint8_t *buffer, size_t maxLength);
    static igcdata jsonToIgcdata(String data);
};

extern esp32FOTA2 esp32FOTA;
extern QueueHandle_t xQueueParse;

#endif //VARIO_WEBHANDLER_H