/*
   esp32 firmware OTA
   Date: December 2018
   Author: Chris Joyce <https://chrisjoyce911/esp32FOTA>
   Purpose: Perform an OTA update from a bin located on a webserver (HTTP & HTTPS)
*/

#include "esp_log.h"

#include "Arduino.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Update.h>
#include "ArduinoJson.h"
#include <WiFiClientSecure.h>
#include "VarioTool/VarioTool.h"
#include "varioversion.h"
#include "version.h"
#include "VarioDebug/VarioDebug.h"
#include "esp32FOTA2/src/esp32fota2.h"
#include <ESP32-targz.h>

esp32FOTA2 esp32FOTA("Gnuvario" + String(VARIOVERSION), VERSION, SUB_VERSION, BETA_CODE);

esp32FOTA2::esp32FOTA2(String firwmareType, int firwmareVersion, int firwmareSubVersion, int firwmareBetaVersion)
{
    _firwmareType = firwmareType;
    _firwmareVersion = firwmareVersion;
    _firwmareSubVersion = firwmareSubVersion;
    _firwmareBetaVersion = firwmareBetaVersion;
    useDeviceID = false;
    _gzwwwfile = "";
}

// Utility to extract header value from headers
String esp32FOTA2::getHeaderValue(String header, String headerName)
{
    return header.substring(strlen(headerName.c_str()));
}

// OTA Logic
void esp32FOTA2::execOTA()
{
    // screen.ScreenViewReboot("Upgrade");

    WiFiClient client;
    int contentLength = 0;
    bool isValidContentType = false;

    VARIO_WIFI_DEBUG_PRINTLN("Connecting to: " + String(_host));

    if (_gzwwwfile != "")
    {
        downloadGzwwwFile();
    }

    if (client.connect(_host.c_str(), _port))
    {
        // Connection Succeed.
        // Fecthing the bin
        VARIO_WIFI_DEBUG_PRINTLN("Fetching Bin: " + String(_bin));

        // Get the contents of the bin file
        client.print(String("GET ") + _bin + " HTTP/1.1\r\n" +
                     "Host: " + _host + "\r\n" +
                     "Cache-Control: no-cache\r\n" +
                     "Connection: close\r\n\r\n");

        unsigned long timeout = millis();
        while (client.available() == 0)
        {
            if (millis() - timeout > 5000)
            {
                VARIO_WIFI_DEBUG_PRINTLN("Client Timeout !");
                client.stop();
                return;
            }
        }

        while (client.available())
        {
            // read line till /n
            String line = client.readStringUntil('\n');
            // remove space, to check if the line is end of headers
            line.trim();

            if (!line.length())
            {
                //headers ended
                break; // and get the OTA started
            }

            // Check if the HTTP Response is 200
            // else break and Exit Update
            if (line.startsWith("HTTP/1.1"))
            {
                VARIO_WIFI_DEBUG_PRINTLN("HTTP LINE");
                VARIO_WIFI_DEBUG_PRINTLN(line);
                if (line.indexOf("200") < 0)
                {

                    VARIO_WIFI_DEBUG_PRINTLN("Got a non 200 status code from server. Exiting OTA Update.");
                    break;
                }
            }

            // extract headers here
            // Start with content length
            if (line.startsWith("Content-Length: "))
            {
                contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
                VARIO_WIFI_DEBUG_PRINTLN("Got " + String(contentLength) + " bytes from server");
            }

            // Next, the content type
            if (line.startsWith("Content-Type: "))
            {
                String contentType = getHeaderValue(line, "Content-Type: ");
                VARIO_WIFI_DEBUG_PRINTLN("Got " + contentType + " payload.");
                if (contentType == "application/octet-stream")
                {
                    isValidContentType = true;
                }
            }
        }
    }
    else
    {
        // Connect to webserver failed
        // May be try?
        // Probably a choppy network?
        VARIO_WIFI_DEBUG_PRINTLN("Connection to " + String(_host) + " failed. Please check your setup");
        // retry??
        // execOTA();
    }

    // Check what is the contentLength and if content type is `application/octet-stream`
    VARIO_WIFI_DEBUG_PRINTLN("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

    // check contentLength and content type
    if (contentLength && isValidContentType)
    {
        // Check if there is enough to OTA Update
        bool canBegin = Update.begin(contentLength);

        // If yes, begin
        if (canBegin)
        {
            VARIO_WIFI_DEBUG_PRINTLN("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");

            // No activity would appear on the Serial monitor
            // So be patient. This may take 2 - 5mins to complete
            size_t written = Update.writeStream(client);

            if (written == contentLength)
            {
                VARIO_WIFI_DEBUG_PRINTLN("Written : " + String(written) + " successfully");
            }
            else
            {
                VARIO_WIFI_DEBUG_PRINTLN("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?");

                // retry??
                // execOTA();
            }

            if (Update.end())
            {
                VARIO_WIFI_DEBUG_PRINTLN("OTA done!");
                if (Update.isFinished())
                {
                    VARIO_WIFI_DEBUG_PRINTLN("Update successfully completed. Rebooting.");

                    // screen.ScreenViewReboot();
                    // ESP.restart();
                }
                else
                {
                    VARIO_WIFI_DEBUG_PRINTLN("Update not finished? Something went wrong!");
                }
            }
            else
            {
                VARIO_WIFI_DEBUG_PRINTLN("Error Occurred. Error #: " + String(Update.getError()));
            }
        }
        else
        {
            // not enough space to begin OTA
            // Understand the partitions and
            // space availability
            VARIO_WIFI_DEBUG_PRINTLN("Not enough space to begin OTA");

            client.flush();
        }
    }
    else
    {
        VARIO_WIFI_DEBUG_PRINTLN("There was no content in the response");

        client.flush();
    }
}

//************************************
bool esp32FOTA2::execHTTPexist()
//************************************
{

    String useURL;

    useURL = checkURL + "/" + getDeviceID();

    WiFiClient client;
    _port = 80;

    VARIO_WIFI_DEBUG_PRINTLN("Getting HTTP");
    VARIO_WIFI_DEBUG_PRINTLN(useURL);
    VARIO_WIFI_DEBUG_PRINTLN("------");

    if ((WiFi.status() == WL_CONNECTED))
    { //Check the current connection status

        HTTPClient http;

        http.begin(useURL);        //Specify the URL
        int httpCode = http.GET(); //Make the request

        if (httpCode > 0)
        {
            //Check is a file was returned
            VARIO_WIFI_DEBUG_PRINT("[HTTP] GET... code :");
            VARIO_WIFI_DEBUG_PRINTLN(httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = http.getString();
                VARIO_WIFI_DEBUG_PRINTLN(payload);
            }

            http.end(); //Free the resources
            return true;
        }
        else
        {
            VARIO_WIFI_DEBUG_PRINTLN("Error on HTTP request");
            VARIO_WIFI_DEBUG_PRINT("[HTTP] GET... failed, error: ");
            VARIO_WIFI_DEBUG_PRINTLN(http.errorToString(httpCode).c_str());

            http.end(); //Free the resources
            return false;
        }
    }
    return false;
}

uint8_t esp32FOTA2::execHTTPcheck(bool betaVersion)
{
    String useURL;

    useURL = checkURL + "/" + getDeviceID();

    WiFiClient client;
    _port = 80;

    VARIO_WIFI_DEBUG_PRINTLN("Getting HTTP");
    VARIO_WIFI_DEBUG_PRINTLN(useURL);
    VARIO_WIFI_DEBUG_PRINTLN("------");

    if ((WiFi.status() == WL_CONNECTED))
    { //Check the current connection status

        HTTPClient http;

        http.begin(useURL);        //Specify the URL
        int httpCode = http.GET(); //Make the request

        if (httpCode == 200)
        {
            //Check is a file was returned

            String payload = http.getString();

            int str_len = payload.length() + 1;
            char JSONMessage[str_len];
            payload.toCharArray(JSONMessage, str_len);

            VARIO_WIFI_DEBUG_PRINT("Parsing size : ");
            VARIO_WIFI_DEBUG_PRINTLN(str_len);
            //VARIO_WIFI_DEBUG_PRINTLN(JSONMessage);

            StaticJsonDocument<1300> JSONDocument; //Memory pool
#define JSONDOC JSONDocument

            DeserializationError err = deserializeJson(JSONDOC, JSONMessage);

            if (err)
            {
                //Check for errors in parsing
                VARIO_WIFI_DEBUG_PRINTLN("Parsing failed");

                delay(5000);
                return MAJ_ERROR;
            }

            String tmp;
            tmp = _firwmareType;
            if (betaVersion)
                tmp = tmp + "b";

            VARIO_WIFI_DEBUG_PRINT("Update Firmware : ");
            VARIO_WIFI_DEBUG_PRINTLN(tmp);

            if (JSONDOC.containsKey(tmp))
            {
                JsonObject JSONDOCUpdate = JSONDOC[tmp];

                //const char *pltype = JSONDOC["type"];
                int plversion = JSONDOCUpdate["version"];
                UpdateVersion = plversion;

                int plsubversion = JSONDOCUpdate["subversion"];
                UpdateSubVersion = plsubversion;

                int plbetaversion = JSONDOCUpdate["betaversion"];
                UpdateBetaVersion = plbetaversion;

                const char *plhost = JSONDOCUpdate["host"];
                _port = JSONDOCUpdate["port"];

                if (JSONDOCUpdate.containsKey("gzwww"))
                {

                    VARIO_WIFI_DEBUG_PRINTLN("la section du fichier json contient la clé www");

                    const char *gzwww = JSONDOCUpdate["gzwww"];
                    _gzwwwfile = String(gzwww);
                }

                VARIO_WIFI_DEBUG_PRINT("Version : ");
                VARIO_WIFI_DEBUG_PRINTLN(plversion);

                VARIO_WIFI_DEBUG_PRINT("Sub Version : ");
                VARIO_WIFI_DEBUG_PRINTLN(plsubversion);

                VARIO_WIFI_DEBUG_PRINT("Beta Version : ");
                VARIO_WIFI_DEBUG_PRINTLN(plbetaversion);

                VARIO_WIFI_DEBUG_PRINT("Port : ");
                VARIO_WIFI_DEBUG_PRINTLN(_port);

                const char *plbin = JSONDOCUpdate["bin"];

                String jshost(plhost);
                String jsbin(plbin);

                _host = jshost;
                _bin = jsbin;

                VARIO_WIFI_DEBUG_PRINT("Host : ");
                VARIO_WIFI_DEBUG_PRINTLN(_host);

                VARIO_WIFI_DEBUG_PRINT("Bin : ");
                VARIO_WIFI_DEBUG_PRINTLN(_bin);

                VARIO_WIFI_DEBUG_PRINT("Firmware Version : ");
                VARIO_WIFI_DEBUG_PRINTLN(_firwmareVersion);

                VARIO_WIFI_DEBUG_PRINT("Firmware Sub Version : ");
                VARIO_WIFI_DEBUG_PRINTLN(_firwmareSubVersion);

                VARIO_WIFI_DEBUG_PRINT("Firmware Beta Version : ");
                VARIO_WIFI_DEBUG_PRINTLN(_firwmareBetaVersion);

                if (plversion > _firwmareVersion)
                {
                    http.end(); //Free the resources
                    return MAJ_AVAILABLE;
                }
                else if ((plversion == _firwmareVersion) && (plsubversion > _firwmareSubVersion))
                {
                    http.end(); //Free the resources
                    return MAJ_AVAILABLE;
                }
                else if ((plversion == _firwmareVersion) && (plsubversion == _firwmareSubVersion) && (plbetaversion > _firwmareBetaVersion))
                {
                    //ce cas ne match que pour les beta
                    http.end(); //Free the resources
                    return MAJ_AVAILABLE;
                }

                else
                {
                    VARIO_WIFI_DEBUG_PRINTLN("MAJ MAJ_NOTAVAILABLE");
                    http.end(); //Free the resources
                    return MAJ_NOTAVAILABLE;
                }
            }
            else
            {

                if (betaVersion)
                {
                    VARIO_WIFI_DEBUG_PRINTLN("Erreur info firmware non trouvé dans la beta");
                }
                else
                {
                    VARIO_WIFI_DEBUG_PRINTLN("Erreur info firmware non trouvé dans la non beta");
                }

                http.end(); //Free the resources
                return MAJ_ERROR;
            }
        }

        else
        {
            VARIO_WIFI_DEBUG_PRINTLN("Error on HTTP request");
            http.end(); //Free the resources
            return MAJ_ERROR;
        }

        http.end(); //Free the resources
        return MAJ_ERROR;
    }
    return MAJ_ERROR;
}

String esp32FOTA2::getDeviceID()
{
    char deviceid[21];
    uint64_t chipid;
    chipid = ESP.getEfuseMac();
    sprintf(deviceid, "%" PRIu64, chipid);
    String thisID(deviceid);
    return thisID;
}

// Force a firmware update regartless on current version
bool esp32FOTA2::forceUpdate(String firwmareHost, int firwmarePort, String firwmarePath)
{
    _host = firwmareHost;
    _bin = firwmarePath;
    _port = firwmarePort;
    execOTA();

    return true;
}

String esp32FOTA2::getHTTPVersion()
{
    String output;

    output = "{\n";

    output += "\"Firmware\":{\n";
    output += "\"type\": " + String(VARIOVERSION) + ",\n";
    output += "\"version\": " + String(_firwmareVersion) + ",\n";
    output += "\"subversion\": " + String(_firwmareSubVersion) + ",\n";
    output += "\"betaversion\": " + String(_firwmareBetaVersion) + "\n}";

    int8_t updatedNeeded = execHTTPcheck(false);

    if (updatedNeeded > 0)
    {
        VARIO_WIFI_DEBUG_PRINTLN("************** Version Stable : Mise à jour disponible *****************");
        VARIO_WIFI_DEBUG_PRINTLN(updatedNeeded);
        VARIO_WIFI_DEBUG_PRINT("Version : ");
        VARIO_WIFI_DEBUG_PRINT(UpdateVersion);
        VARIO_WIFI_DEBUG_PRINT(".");
        VARIO_WIFI_DEBUG_PRINTLN(UpdateSubVersion);

        output += ",\n\"" + _firwmareType + "\":{\n";
        output += "\"type\": " + String(VARIOVERSION) + ",\n";
        output += "\"version\": " + String(UpdateVersion) + ",\n";
        output += "\"subversion\": " + String(UpdateSubVersion) + ",\n";
        output += "\"betaversion\": 0\n}";
    }

    updatedNeeded = execHTTPcheck(true);
    if (updatedNeeded > 0)
    {
        VARIO_WIFI_DEBUG_PRINTLN("***************** Version Beta : Mise à jour disponible *********************");
        VARIO_WIFI_DEBUG_PRINT("Version : ");
        VARIO_WIFI_DEBUG_PRINT(UpdateVersion);
        VARIO_WIFI_DEBUG_PRINT(".");
        VARIO_WIFI_DEBUG_PRINT(UpdateSubVersion);
        VARIO_WIFI_DEBUG_PRINT("b");
        VARIO_WIFI_DEBUG_PRINTLN(UpdateBetaVersion);

        output += ",\n\"" + _firwmareType + "b\":{\n";
        output += "\"type\": " + String(VARIOVERSION) + ",\n";
        output += "\"version\": " + String(UpdateVersion) + ",\n";
        output += "\"subversion\": " + String(UpdateSubVersion) + ",\n";
        output += "\"betaversion\": " + String(UpdateBetaVersion) + "\n}";
    }

    output += "\n}";

    return output;
}

/**
 * Methode qui telecharge l'archive www.gz depuis le site de mise à jour
 */
void esp32FOTA2::downloadGzwwwFile()
{
    VARIO_WIFI_DEBUG_PRINTLN("[HTTP] Debut méthode downloadGzwwwFile");

    HTTPClient http;
    const char *archFilename = "/www.gz";

    http.begin(_host.c_str(), _port, _gzwwwfile);

    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        VARIO_WIFI_DEBUG_PRINT("[HTTP] GET... code: ");
        VARIO_WIFI_DEBUG_PRINTLN(httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            File archFile;
            archFile = SD.open(archFilename, FILE_WRITE);

            if (!archFile)
            {
                VARIO_WIFI_DEBUG_PRINT("Impossible de créer le fichier : ");
                VARIO_WIFI_DEBUG_PRINTLN(archFilename);

                return;
            }

            VARIO_WIFI_DEBUG_PRINT("[HTTP] Début écriture");
            VARIO_WIFI_DEBUG_PRINTLN(archFilename);

            // get lenght of document (is -1 when Server sends no Content-Length header)
            int len = http.getSize();

            // create buffer for read
            uint8_t buff[128] = {0};

            // get tcp stream
            WiFiClient *stream = http.getStreamPtr();

            // read all data from server
            while (http.connected() && (len > 0 || len == -1))
            {
                // get available data size
                size_t size = stream->available();

                if (size)
                {
                    // read up to 128 byte
                    int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                    //l'ecriture dans le fichier
                    archFile.write(buff, c);
                    archFile.flush();

                    if (len > 0)
                    {
                        len -= c;
                    }
                }
                vTaskDelay(pdMS_TO_TICKS(1));
            }

            archFile.close();

            VARIO_WIFI_DEBUG_PRINT("[HTTP] Fin écriture");
            VARIO_WIFI_DEBUG_PRINTLN(archFilename);
            VARIO_WIFI_DEBUG_PRINTLN("[HTTP] connection closed or file end.");
        }
    }
    else
    {
        VARIO_WIFI_DEBUG_PRINTLN("[HTTP] GET... failed, error:" + String(http.errorToString(httpCode).c_str()));
    }

    http.end();
}

/**
 * Méthode qui:
 * - vérifie la présence d'un fichier www.gz à la racine
 * - si présent, sauvegarde le rep www dans wwwold
 * - extrait l'archive
 * - supprime l'archive
 * 
 */
int8_t esp32FOTA2::UpdateWwwDirectoryFromGz()
{
    VARIO_WIFI_DEBUG_PRINTLN("[HTTP] Debut méthode UpdateWwwDirectoryFromGz");

    TarGzUnpacker *TARGZUnpacker = new TarGzUnpacker();

    TARGZUnpacker->haltOnError(true);  // stop on fail (manual restart/reset required)
    TARGZUnpacker->setTarVerify(true); // true = enables health checks but slows down the overall process

    TARGZUnpacker->setTarStatusProgressCallback(BaseUnpacker::defaultTarStatusProgressCallback); // print the filenames as they're expanded
                                                                                                 //  TARGZUnpacker->setupFSCallbacks( targzTotalBytesFn, targzFreeBytesFn ); // prevent the partition from exploding, recommended
    TARGZUnpacker->setGzProgressCallback(BaseUnpacker::defaultProgressCallback);                 // targzNullProgressCallback or defaultProgressCallback
    TARGZUnpacker->setLoggerCallback(BaseUnpacker::targzPrintLoggerCallback);                    // gz log verbosity
    TARGZUnpacker->setTarProgressCallback(BaseUnpacker::defaultProgressCallback);                // prints the untarring progress for each individual file
                                                                                                 //  TARGZUnpacker->setTarMessageCallback( myTarMessageCallback/*BaseUnpacker::targzPrintLoggerCallback*/ ); // tar log verbosity

    const char *myfilename = "/www.gz";
    const char *origRep = "/www";
    const char *oldRep = "/wwwold";

    if (!SD.exists(myfilename))
    {
        //rien à faire
        VARIO_WIFI_DEBUG_PRINTLN("[HTTP] Fichier www.gz introuvable");

        return 0;
    }

    /** @TODO, gérer l'affichage du message */
    // screen.ScreenViewMessage("MAJ www", 0);

    //backup de l'ancien répertoire www
    const char *tmpPath = "/wwwold";

    if (SD.exists(tmpPath))
    {
        VARIO_WIFI_DEBUG_PRINTLN("[HTTP] le dossier wwwold existe");

        //suppression de wwwold si existe
        VarioTool::deleteRecursive(tmpPath);
    }

    if (SD.exists(origRep))
    {
        // rename "www" into "wwwold"
        if (!SD.rename(origRep, oldRep))
        {
            VARIO_WIFI_DEBUG_PRINTLN("[HTTP] le dossier www ne peut être renommé en wwwold");

            return -1; //Pas de mise à jour
        }
    }

    //decompression de l'archive dans /www
    if (TARGZUnpacker->tarGzExpander(SD, myfilename, SD, "/"))
    {
        VARIO_WIFI_DEBUG_PRINTLN("Decompression dand /www");
    }
    else
    {
        VARIO_WIFI_DEBUG_PRINTLN("Echec decompression dand /www");

        return -1;
    }

    //suppression de l'archive
    if (!SD.remove(myfilename))
    {
        VARIO_WIFI_DEBUG_PRINTLN("[HTTP] le fichier n'a pas pu être supprimé");

        return -1;
    }

    return 1;
}

/*{
  "Gnuvario154":{
    "version": 0,
		"subversion": 7,
		"betaversion": 0,
    "host": "gnuvario-e.yj.fr",
    "port": 80,
    "bin": "/update/Gnuvario154.bin"
	},
  "Gnuvario154b":{
    "version": 0,
		"subversion": 7,
		"betaversion": 8,
    "host": "gnuvario-e.yj.fr",
    "port": 80,
    "bin": "/update/Gnuvario154b.bin"
	},*/
