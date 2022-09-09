#include "VarioWifi.h"

#include <ESPmDNS.h>
#include <WiFiMulti.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "VarioDebug/VarioDebug.h"

#include "VarioData/VarioData.h"

#include "VarioManager.h"
//#include "VarioSettings/VarioSettings.h"

#define SERVER_NAME "GNUVARIO-E"

WiFiMulti wifiMulti;
AsyncWebServer server(80);
VarioWebHandler varioWebHandler;

VarioWifi::VarioWifi()
{
}

void VarioWifi::startTask()
{
    // task creation
    VARIO_PROG_DEBUG_PRINTLN("TaskVarioWifi started");
    xTaskCreatePinnedToCore(this->startTaskImpl, "TaskVarioWifi", 10000, this, 2, &_taskVarioWifiHandle, 0);
}

void VarioWifi::startTaskImpl(void *parm)
{
    // wrapper for task
    static_cast<VarioWifi *>(parm)->task();
}

void VarioWifi::task()
{
    begin();
    while (1)
    {
        // give time to other tasks
        vTaskDelay(delayT50);
    }
}

bool VarioWifi::begin()
{

    if (!isIndexFileExist())
    {
        return false;
    }

    VARIO_MEMORY_DEBUG_PRINTLN("Free heap connectToWifi");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    if (!connectToWifi())
    {
        return false;
    }

    VARIO_MEMORY_DEBUG_PRINTLN("Free heap setClock");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    setClock();

    VARIO_MEMORY_DEBUG_PRINTLN("Free heap setMDNS");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    setMDNS();

    checkDbVersion();

    esp32FOTA.checkURL = varioData.getParam(PARAM_URL_UPDATE)->getValueChar(); //"http://gnuvario-e.yj.fr/update/firmware.json";

    VARIO_MEMORY_DEBUG_PRINTLN("Free heap startWebServer");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    startWebServer();

    return true;
}

bool VarioWifi::connectToWifi()
{
    const char *ssid_1;
    const char *ssid_2;
    const char *ssid_3;
    const char *ssid_4;
    const char *password_1;
    const char *password_2;
    const char *password_3;
    const char *password_4;

    ssid_1 = varioData.getParam(PARAM_SSID_1)->getValueChar();
    ssid_2 = varioData.getParam(PARAM_SSID_2)->getValueChar();
    ssid_3 = varioData.getParam(PARAM_SSID_3)->getValueChar();
    ssid_4 = varioData.getParam(PARAM_SSID_4)->getValueChar();
    password_1 = varioData.getParam(PARAM_PASSWORD_1)->getValueChar();
    password_2 = varioData.getParam(PARAM_PASSWORD_2)->getValueChar();
    password_3 = varioData.getParam(PARAM_PASSWORD_3)->getValueChar();
    password_4 = varioData.getParam(PARAM_PASSWORD_4)->getValueChar();

    VARIO_WIFI_DEBUG_PRINT("ssid_1 : ");
    VARIO_WIFI_DEBUG_PRINTLN(ssid_1);
    VARIO_WIFI_DEBUG_PRINT("password_1 : ");
    VARIO_WIFI_DEBUG_PRINTLN(password_1);

    VARIO_WIFI_DEBUG_PRINT("ssid_2 : ");
    VARIO_WIFI_DEBUG_PRINTLN(ssid_2);
    VARIO_WIFI_DEBUG_PRINT("password_2 : ");
    VARIO_WIFI_DEBUG_PRINTLN(password_2);

    VARIO_WIFI_DEBUG_PRINT("ssid_3 : ");
    VARIO_WIFI_DEBUG_PRINTLN(ssid_3);
    VARIO_WIFI_DEBUG_PRINT("password_3 : ");
    VARIO_WIFI_DEBUG_PRINTLN(password_3);

    VARIO_WIFI_DEBUG_PRINT("ssid_4 : ");
    VARIO_WIFI_DEBUG_PRINTLN(ssid_4);
    VARIO_WIFI_DEBUG_PRINT("password_4 : ");
    VARIO_WIFI_DEBUG_PRINTLN(password_4);

    wifiMulti.addAP("gnuvario", "gnuvario");

    if (ssid_1[0] != '\0' && password_1[0] != '\0')
    {
        wifiMulti.addAP(ssid_1, password_1); // add Wi-Fi networks you want to connect to, it connects strongest to weakest
    }
    if (ssid_2[0] != '\0' && password_2[0] != '\0')
    {
        wifiMulti.addAP(ssid_2, password_2);
    }
    if (ssid_3[0] != '\0' && password_3[0] != '\0')
    {
        wifiMulti.addAP(ssid_3, password_3);
    }
    if (ssid_4[0] != '\0' && password_4[0] != '\0')
    {
        wifiMulti.addAP(ssid_4, password_4);
    }

    VARIO_WIFI_DEBUG_PRINTLN("Connecting ...");

    int counter = 0;
    while (wifiMulti.run() != WL_CONNECTED)
    {
        // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        vTaskDelay(delayT50 * 50);
        VARIO_WIFI_DEBUG_PRINT('.');
        counter++;
        if (counter > 30)
        {
            VARIO_WIFI_DEBUG_PRINTLN("Error Wifi not found!");
            ESP.restart();
        }
    }
    VARIO_WIFI_DEBUG_PRINT("Connected to :");
    VARIO_WIFI_DEBUG_PRINTLN(WiFi.SSID());
    VARIO_WIFI_DEBUG_PRINT("Use IP address :");
    VARIO_WIFI_DEBUG_PRINTLN(WiFi.localIP().toString());
    fc.setText1(true, (char *)WiFi.SSID().c_str());
    fc.setText3(true, (char *)WiFi.localIP().toString().c_str());

    return true;
}

bool VarioWifi::checkDbVersion()
{
    VARIO_WIFI_DEBUG_PRINTLN("checkDbVersion");

    if (!SD.exists("/db"))
    {
        SD.mkdir("/db");
        File myOrigFile;
        File myDestFile;

        // copie du fichier de bdd initial si inexistant
        if (SD.exists("/www/db/vol.db") && !SD.exists("/db/vol.db"))
        {
            myDestFile = SD.open("/db/vol.db", FILE_WRITE);
            myOrigFile = SD.open("/www/db/vol.db", FILE_READ);
            if (myDestFile && myOrigFile)
            {
                int data;
                while ((data = myOrigFile.read()) >= 0)
                {
                    myDestFile.write(data);
                }
                myDestFile.close();
                myOrigFile.close();
            }
            else
            {
                return false;
            }
        }
    }

    char path[] = "/www/sql";
    File dir;

    dir = SD.open(path, FILE_READ);

    if (!dir.isDirectory())
    {
        dir.close();
        return false;
    }
    dir.rewindDirectory();

    VarioSqlFlight varioSqlFlight;

    for (int cnt = 0; true; ++cnt)
    {
        File entry;
        entry = dir.openNextFile(FILE_READ);
        if (!entry)
        {
            break;
        }

        String tmpFullName = entry.name();
        String version = tmpFullName.substring(tmpFullName.lastIndexOf("/") + 1);
        version = version.substring(0, version.lastIndexOf("."));

        VARIO_MEMORY_DEBUG_PRINTLN("avant migration");
        VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

        varioSqlFlight.executeMigration(version, entry.readString());

        VARIO_MEMORY_DEBUG_PRINTLN("apres migration");
        VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

        VARIO_WIFI_DEBUG_PRINTLN(version);
    }
    dir.close();

    return true;
}

bool VarioWifi::isIndexFileExist()
{
    File dataFile;

    // test présence fichier index
    if (SD.exists("/www/index.htm"))
    {
        VARIO_WIFI_DEBUG_PRINTLN("SD Card: initialized, index.htm found");

        return true;
    }
    else
    {
        VARIO_WIFI_DEBUG_PRINTLN("SD Card: failed to read index.htm.");

        return false;
    }
}

/***********************************/
void VarioWifi::setClock()
{
    /***********************************/
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    VARIO_WIFI_DEBUG_PRINT("Waiting for NTP time sync: ");

    time_t nowSecs = time(nullptr);
    while (nowSecs < 8 * 3600 * 2)
    {
        vTaskDelay(delayT50 * 10);

        VARIO_WIFI_DEBUG_PRINT(".");

        nowSecs = time(nullptr);
    }

    VARIO_WIFI_DEBUG_PRINTLN("");

    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);

    VARIO_WIFI_DEBUG_PRINT("Current time: ");
    VARIO_WIFI_DEBUG_PRINTLN(asctime(&timeinfo));
}

void VarioWifi::setMDNS()
{
    if (MDNS.begin(SERVER_NAME))
    {
        MDNS.addService("http", "tcp", 80);

        VARIO_WIFI_DEBUG_PRINTLN("MDNS responder started");
        VARIO_WIFI_DEBUG_PRINT("You can now connect to http://");
        VARIO_WIFI_DEBUG_PRINT(SERVER_NAME);
        VARIO_WIFI_DEBUG_PRINTLN(".local");
    }
    else
    {
        VARIO_WIFI_DEBUG_PRINTLN("Error setting up MDNS responder!");
        ESP.restart();
    }
}

void VarioWifi::startWebServer()
{

    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", String(ESP.getFreeHeap())); });

    // téléchargements de "mes vols" = contenu du repertoire vols
    server.on("/flights", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleListFlights(request)); });

    // récupération de la liste du contenu de la carte SD
    server.on("/list", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handlePrintDirectory(request)); });

    // récupération du contenu du fichier param
    server.on("/params", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleParams(request)); });

    // le reponse est envoyé par le handler sur le body
    server.on(
        "/params", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL, varioWebHandler.handleSaveParams);

    // mise à jour OTA
    // le reponse est envoyé par le handler sur le upload
    server.on(
        "/fupdate", HTTP_POST, [](AsyncWebServerRequest *request) {},
        varioWebHandler.handleOtaUpdate);

    // telechargement d'un fichier dont le nom complet avec chemin se trouve en param
    server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleFileDownload(request)); });

    // suppression d'un fichier dont le nom complet avec chemin se trouve en param
    server.on("/file", HTTP_DELETE, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleFileDelete(request)); });

    // upload d'un fichier, le chemin de destination se trouve dans le nom du fichier posté
    // le reponse est envoyé par le handler sur le upload
    server.on(
        "/upload", HTTP_POST, [](AsyncWebServerRequest *request) {},
        varioWebHandler.handleFileUpload);

    // sauvegarde du contenu du fichier wifi
    // le reponse est envoyé par le handler sur le body
    server.on(
        "/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL, varioWebHandler.handleSaveWifi);

    // creation d'un fichier ou d'un repertoire
    //  le reponse est envoyé par le handler sur le upload
    server.on(
        "/create", HTTP_PUT, [](AsyncWebServerRequest *request) {},
        NULL, varioWebHandler.handleFileCreate);

    // récupération du contenu du fichier wifi
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleWifi(request)); });

    // récupération du contenu du fichier preference
    server.on("/webconfig", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleWebConfig(request)); });

    // sauvegarde du contenu du fichier preference
    // le reponse est envoyé par le handler sur le body
    server.on(
        "/webconfig", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL, varioWebHandler.handleSaveWebConfig);

    // parsage d'un fichier IGC
    server.on("/parseigc", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleParseIgc(request)); });

    // téléchargements de "mes vols" en BDD
    server.on("/flightsbdd", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleGetFlights(request)); });

    // enregistrement un vol en BDD
    // le reponse est envoyé par le handler sur le body
    server.on(
        "/flightsbdd", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL, varioWebHandler.handleSetFlight);

    // suppression d'un vol en BDD
    server.on("/flightsbdd", HTTP_DELETE, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleDelFlight(request)); });

    // recuperation des versions de firmware
    server.on("/firmwareversion", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleFirmwareVersion(request)); });

    // Mise à jour via internet
    server.on("/upgradeweb", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleUpgradeWeb(request)); });

    // récupération de la liste des sites
    server.on("/site", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleGetSites(request)); });

    // sauvegarde d'un site
    // le reponse est envoyé par le handler sur le body
    server.on(
        "/site", HTTP_POST, [](AsyncWebServerRequest *request) {},
        NULL, varioWebHandler.handleSetSite);

    // suppression d'un site
    server.on("/site", HTTP_DELETE, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.handleDelSite(request)); });

    // récupération du résumé des vols
    server.on("/flightsshort", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(varioWebHandler.getFlightsShort(request)); });

    // default web dir "/www"
    server.serveStatic("/", SD, "/www/").setDefaultFile("index.htm");

    server.onNotFound([](AsyncWebServerRequest *request)
                      {
                          VARIO_WIFI_DEBUG_PRINTLN("NOT_FOUND: ");
                          if (request->method() == HTTP_GET)
                              VARIO_WIFI_DEBUG_PRINTLN("GET");
                          else if (request->method() == HTTP_POST)
                              VARIO_WIFI_DEBUG_PRINTLN("POST");
                          else if (request->method() == HTTP_DELETE)
                              VARIO_WIFI_DEBUG_PRINTLN("DELETE");
                          else if (request->method() == HTTP_PUT)
                              VARIO_WIFI_DEBUG_PRINTLN("PUT");
                          else if (request->method() == HTTP_PATCH)
                              VARIO_WIFI_DEBUG_PRINTLN("PATCH");
                          else if (request->method() == HTTP_HEAD)
                              VARIO_WIFI_DEBUG_PRINTLN("HEAD");
                          else if (request->method() == HTTP_OPTIONS)
                              VARIO_WIFI_DEBUG_PRINTLN("OPTIONS");
                          else
                              VARIO_WIFI_DEBUG_PRINTLN("UNKNOWN");
                          Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

                          if (request->contentLength())
                          {
                              Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
                              Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
                          }

                          int headers = request->headers();
                          int i;
                          for (i = 0; i < headers; i++)
                          {
                              AsyncWebHeader *h = request->getHeader(i);
                              Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
                          }

                          int params = request->params();
                          for (i = 0; i < params; i++)
                          {
                              AsyncWebParameter *p = request->getParam(i);
                              if (p->isFile())
                              {
                                  Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
                              }
                              else if (p->isPost())
                              {
                                  Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
                              }
                              else
                              {
                                  Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                              }
                          }

                          if (request->method() == HTTP_OPTIONS)
                          {
                              request->send(200);
                          }
                          else
                          {
                              request->send(404);
                          } });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

    server.begin();
}