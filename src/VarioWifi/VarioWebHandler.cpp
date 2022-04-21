#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include "VarioWebHandler.h"
#include "VarioTool/VarioTool.h"
#include "VarioDebug/VarioDebug.h"

File uploadFile;
File varioParamFile;
File wifiParamFile;
File webParamFile;
VarioSqlFlightHelper varioSqlFlightHelper;

VarioWebHandler::VarioWebHandler()
{
}

AsyncResponseStream *VarioWebHandler::handleListFlights(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    String path = "/vols";

    File dir;
    dir = SD.open((char *)path.c_str(), FILE_READ);

    if (!dir.isDirectory())
    {
        dir.close();
    }
    dir.rewindDirectory();

    response->print("[");

    bool first = true;
    File entry;
    while (entry = dir.openNextFile(FILE_READ))
    {
        String output;
        String fsize = "";

        if (!entry.isDirectory())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                output = ',';
            }
            int bytes = entry.size();
            fsize = getFileSizeStringFromBytes(bytes);

            output += "{\"type\":\"";
            output += (entry.isDirectory()) ? "dir" : "file";
            output += "\",\"name\":\"";
            output += entry.name();
            output += "\",\"size\":\"";
            output += fsize;
            output += "\"";
            output += "}";

            response->print(output);
        }

        entry.close();
    }

    response->print("]");

    //correction bug chunk transfer webserver
    // server.sendContent("");
    dir.close();

    return response;
}

AsyncResponseStream *VarioWebHandler::handlePrintDirectory(AsyncWebServerRequest *request)
{
    /***********************************/
    VARIO_WIFI_DEBUG_PRINTLN("handlePrintDirectory");

    AsyncResponseStream *response = request->beginResponseStream("application/json");

    String path;
    boolean isRecursive = true;

    if (request->hasParam("dir"))
    {
        AsyncWebParameter *p = request->getParam("dir");
        path = p->value().c_str();
    }
    else
    {
        path = "/";
    }

    if (request->hasParam("dir"))
    {
        isRecursive = false;
    }

    VARIO_WIFI_DEBUG_PRINT("dir : ");
    VARIO_WIFI_DEBUG_PRINTLN((char *)path.c_str());

    File dir;
    dir = SD.open((char *)path.c_str(), FILE_READ);
    dir.rewindDirectory();

    if (!dir.isDirectory())
    {
        VARIO_WIFI_DEBUG_PRINTLN("Not directory");

        dir.close();
        //response->setCode(404);
        request->send(500, "text/plain", "NOT DIR");

        return response;
    }

    VARIO_WIFI_DEBUG_PRINTLN("[");

    response->print("[");

    String output;
    output += "{\"type\":\"";
    output += "dir";
    output += "\",\"name\":\"";
    output += path;
    output += "\",\"size\":\"";
    output += "na";
    output += "\"";

    output += ",\"contents\" :[";

    response->print(output);
    printDirectoryRecurse(response, path, isRecursive);
    output = "]";

    output += "}";
    response->print(output);
    // printDirectoryRecurse(path);

    response->print("]");

    //correction bug chunk transfer webserver
    // server.sendContent("");
    // server.client().stop();
    VARIO_WIFI_DEBUG_PRINTLN("]");

    dir.close();

    return response;
}

/***********************************/
void VarioWebHandler::printDirectoryRecurse(AsyncResponseStream *response, String path, boolean isRecursive)
/***********************************/
{

    File dir;
    dir = SD.open((char *)path.c_str(), FILE_READ);
    dir.rewindDirectory();

    int tmpcnt = 0;

    for (int cnt = 0; true; ++cnt)
    {
        File entry;
        if (!(entry = dir.openNextFile(FILE_READ)))
        {
            break;
        }
        String tmpName = entry.name();

        if (tmpName.equalsIgnoreCase("SYSTEM~1") || tmpName.startsWith(".")) //equalsIgnoreCase(".TRASH~1"))
        {
            continue;
        }

        String output;
        if (tmpcnt > 0)
        {
            output = ',';
        }

        String fsize = "";

        if (!entry.isDirectory())
        {
            int bytes = entry.size();
            fsize = getFileSizeStringFromBytes(bytes);
        }
        else
        {
            fsize = "na";
        }

        output += "{\"type\":\"";

        output += (entry.isDirectory()) ? "dir" : "file";

        output += "\",\"name\":\"";
        String tmpFullName = entry.name();
        output += tmpFullName.substring(tmpFullName.lastIndexOf("/") + 1);

        output += "\",\"size\":\"";
        output += fsize;
        output += "\"";

        if (entry.isDirectory())
        {
            output += ",\"contents\" :[";

            response->print(output);

            if (isRecursive)
            {
                printDirectoryRecurse(response, entry.name(), isRecursive);
            }
            output = "]";
        }
        output += "}";

        response->print(output);
        entry.close();
        tmpcnt++;
    }
}

AsyncWebServerResponse *VarioWebHandler::handleParams(AsyncWebServerRequest *request)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleParams");

    AsyncWebServerResponse *response;
    String path = "/params.jso";
    if (!SD.exists(path))
    {
        response = request->beginResponse(404);
    }
    else
    {
        response = request->beginResponse(SD, "/params.jso", "application/json");
    }

    return response;
}

void VarioWebHandler::handleSaveParams(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleSaveParams");

    String path = "/params.jso";
    String pathBak = "/params.bak";

    if (!index)
    {
        backupFile(path, pathBak);
        if (!(varioParamFile = SD.open(path.c_str(), FILE_WRITE)))
        {
            request->send(500, "text/plain", "NO FILE");
            return;
        }
    }

    varioParamFile.write(data, len);

    if (index + len == total)
    {
        varioParamFile.close();
        request->send(200);
    }

    return;
}

void VarioWebHandler::handleOtaUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{

    if (!index)
    {
        Serial.printf("UploadStart: %s\n", filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
        { //start with max available size
            Update.printError(Serial);
            request->send(500, "text/plain", "UPDATE FAIL");
            return;
        }
    }
    if (len)
    {
        if (Update.write(data, len) != len)
        {
            Update.printError(Serial);
            request->send(500, "text/plain", "UPDATE FAIL");
            return;
        }
    }

    if (final)
    {
        Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);

        if (Update.end(true))
        { //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", index + len);
            request->send(200);
            Serial.println("RESTART ESP32");
            Serial.flush();

            ESP_LOGI("GnuVario-E", "RESTART ESP32");
#ifdef ENABLE_DISPLAY_WEBSERVER
            screen.ScreenViewReboot();
#endif
            ESP.restart();
        }
        else
        {
            Update.printError(Serial);
            request->send(500, "text/plain", "UPDATE FAIL");
            return;
        }
    }
}

//telechargement d'un fichier dont le nom complet avec chemin se trouve en param
AsyncWebServerResponse *VarioWebHandler::handleFileDownload(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response;
    String path;
    if (request->hasParam("path"))
    {
        AsyncWebParameter *p = request->getParam("path");
        path = p->value().c_str();
    }
    else
    {
        response = request->beginResponse(500, "text/plain", "BAD ARGS");
        return response;
    }

    if (!SD.exists((char *)path.c_str()))
    {
        response = request->beginResponse(500, "text/plain", "NO FILE");
        return response;
    }
    response = request->beginResponse(SD, path, "application/octet-stream", true);

    return response;
}

//suppression d'un fichier dont le nom complet avec chemin se trouve en param
AsyncWebServerResponse *VarioWebHandler::handleFileDelete(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response;
    String path;
    if (request->hasParam("path"))
    {
        AsyncWebParameter *p = request->getParam("path");
        path = p->value().c_str();
    }
    else
    {
        response = request->beginResponse(500, "text/plain", "BAD ARGS");
        return response;
    }

    if (!SD.exists((char *)path.c_str()))
    {
        response = request->beginResponse(500, "text/plain", "NO FILE");
        return response;
    }

    VarioTool::deleteRecursive(path.c_str());

    response = request->beginResponse(200, "text/plain", "OK");
    return response;
}

// upload d'un fichier, le chemin de destination se trouve dans le nom du fichier posté
void VarioWebHandler::handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{

    if (!index)
    {
        if (SD.exists(filename))
        {
            SD.remove(filename);
        }

        Serial.printf("UploadStart: %s\n", filename.c_str());
        uploadFile = SD.open(filename, FILE_WRITE);
        if (!uploadFile)
        {
            //return error
            request->send(500, "text/plain", "UPLOAD FAIL");
            return;
        }
    }

    if (len && uploadFile)
    {
        if (uploadFile.write(data, len) != len)
        {
            //return error
            Serial.printf("Upload fail");
            request->send(500, "text/plain", "UPLOAD FAIL");
            return;
        }
    }

    if (final && uploadFile)
    {
        Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
        uploadFile.close();
        request->send(200);
    }
}

void VarioWebHandler::handleFileCreate(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    String path;

    if (request->hasParam("path"))
    {
        AsyncWebParameter *p = request->getParam("path");
        path = p->value().c_str();
    }
    else
    {
        request->send(500, "text/plain", "BAD ARGS");

        return;
    }

    if (path == "/" || SD.exists((char *)path.c_str()))
    {
        request->send(500, "text/plain", "BAD PATH");
        return;
    }

    if (path.indexOf('.') > 0)
    {
        File fileSD = SD.open((char *)path.c_str(), FILE_WRITE);
        if (fileSD)
        {
            fileSD.print("\0");
            fileSD.close();
        }
    }
    else
    {
        SD.mkdir((char *)path.c_str());
    }
    request->send(200, "text/plain", "OK");
    return;
}

//récupération du contenu du fichier wifi
AsyncWebServerResponse *VarioWebHandler::handleWifi(AsyncWebServerRequest *request)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleParams - handle Wifi");

    AsyncWebServerResponse *response = request->beginResponse(SD, "/wifi.cfg", "text/plain");

    return response;
}

// sauvegarde des parametres
void VarioWebHandler::handleSaveWifi(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleSaveParams - Save Wifi");

    String path = "/wifi.cfg";
    String pathBak = "/wifi.bak";

    if (!index)
    {
        VARIO_WIFI_DEBUG_PRINTLN("handleSaveParams - Backup");

        backupFile(path, pathBak);
        if (!(wifiParamFile = SD.open(path.c_str(), FILE_WRITE)))
        {

            VARIO_WIFI_DEBUG_PRINTLN("Erreur impossible d'ouvrir wifi.cfg");

            request->send(500, "text/plain", "NO FILE");
            return;
        }
    }

    VARIO_WIFI_DEBUG_PRINTLN("Save config wifi");

    wifiParamFile.write(data, len);

    if (index + len == total)
    {
        wifiParamFile.close();
        request->send(200);
    }

    return;
}

AsyncWebServerResponse *VarioWebHandler::handleWebConfig(AsyncWebServerRequest *request)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleParams Web config");

    AsyncWebServerResponse *response;

    if (!SD.exists("/prefs.jso"))
    {
        response = request->beginResponse(404, "text/plain", "NO FILE");
    }
    else
    {
        response = request->beginResponse(SD, "/prefs.jso", "application/json");
    }

    return response;
}

// sauvegarde du contenu du fichier preference
void VarioWebHandler::handleSaveWebConfig(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleSaveParams");

    String path = "/prefs.jso";
    String pathBak = "/prefs.bak";

    if (!index)
    {
        backupFile(path, pathBak);
        if (!(webParamFile = SD.open(path.c_str(), FILE_WRITE)))
        {
            request->send(500, "text/plain", "NO FILE");
            return;
        }
    }

    webParamFile.write(data, len);

    if (index + len == total)
    {
        webParamFile.close();
        request->send(200);
    }

    return;
}

// parsage d'un fichier IGC
AsyncWebServerResponse *VarioWebHandler::handleParseIgc(AsyncWebServerRequest *request)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleParseIgc");

    AsyncWebServerResponse *response;
    uint32_t dataToSend = 1;
    String path;

    if (request->hasParam("path"))
    {
        AsyncWebParameter *p = request->getParam("path");
        path = p->value().c_str();
    }

    else
    {
        response = request->beginResponse(500, "text/plain", "BAD ARGS");
        return response;
    }

    if (!xQueueParse)
    {
        xQueueParse = xQueueCreate(5, sizeof(uint32_t));
        VARIO_WIFI_DEBUG_PRINTLN("xQueueParse created");

        if (!xQueueParse)
        {
            VARIO_WIFI_DEBUG_PRINTLN("xQueueParse failed creation");
        }
    }

    xQueueSend(xQueueParse, &dataToSend, 0);

    // const TickType_t delay = (100) / portTICK_PERIOD_MS;
    // vTaskDelay(delay);

    TaskHandle_t taskParse;
    xTaskCreate(
        _doParseIgcAndInsert,   /* Task function. */
        "doParseIgcAndInsert",  /* String with name of task. */
        18000,                  /* Stack size in bytes. */
        (void *)(path.c_str()), /* Parameter passed as input of the task */
        1,                      /* Priority of the task. */
        &taskParse);            /* Task handle. */

    response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
                                             {
                                                 //Write up to "maxLen" bytes into "buffer" and return the amount written.
                                                 //index equals the amount of bytes that have been already sent
                                                 //You will be asked for more data until 0 is returned
                                                 //Keep in mind that you can not delay or yield waiting for more data!

                                                 uint32_t element;
                                                 if (!(xQueueReceive(xQueueParse, &element, portMAX_DELAY) == pdTRUE))
                                                 {
                                                     VARIO_WIFI_DEBUG_PRINTLN("RIEN DANS LA QUEUE");

                                                     //on force le renvoi de la valeur  1
                                                     element = 1;
                                                 }
                                                 else
                                                 {
                                                     VARIO_WIFI_DEBUG_PRINT("VALEUR DE LA QUEUE");
                                                     VARIO_WIFI_DEBUG_PRINTLN(element);
                                                 };

                                                 if (element == 0)
                                                 {
                                                     // la queue contient l'element 0 qui signifie que le traitement est terminé
                                                     vQueueDelete(xQueueParse);
                                                     xQueueParse = NULL;
                                                     VARIO_WIFI_DEBUG_PRINTLN("handleParseIgc RETURN 0");
                                                     return 0;
                                                 }
                                                 else if (element == 2)
                                                 {
                                                     //la queue contient l'element 2 qui signifie que le traitement a échoué
                                                     // on met 0 dans la queue pour terminer la requete au prochain passage
                                                     uint32_t dataToSend = 0;
                                                     xQueueSend(xQueueParse, &dataToSend, 0);

                                                     return snprintf((char *)buffer, maxLen, "ERROR");
                                                 }
                                                 else
                                                 {
                                                     // la queue contient l'element 1 qui signifie que le traitement est en cours
                                                     return snprintf((char *)buffer, maxLen, ".");
                                                 }
                                             });

    return response;
}

AsyncWebServerResponse *VarioWebHandler::handleGetFlights(AsyncWebServerRequest *request)
{
    int16_t offset;
    int16_t limit;
    String parcel;

    if (request->hasParam("offset"))
    {
        AsyncWebParameter *p = request->getParam("offset");
        offset = p->value().toInt();
    }
    else
    {
        offset = 0;
    }

    if (request->hasParam("limit"))
    {
        AsyncWebParameter *p = request->getParam("limit");
        limit = p->value().toInt();
    }
    else
    {
        limit = 0;
    }

    if (request->hasParam("parcel"))
    {
        AsyncWebParameter *p = request->getParam("parcel");
        parcel = String(p->value());
    }
    else
    {
        parcel = "";
    }

    if (parcel == "")
    {
        varioSqlFlightHelper.init(limit, offset);
    }
    else
    {
        varioSqlFlightHelper.init(parcel);
    }

    AsyncWebServerResponse *response = request->beginChunkedResponse("application/json", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
                                                                     {
                                                                         //Write up to "maxLen" bytes into "buffer" and return the amount written.
                                                                         //index equals the amount of bytes that have been already sent
                                                                         //You will be asked for more data until 0 is returned
                                                                         //Keep in mind that you can not delay or yield waiting for more data!
                                                                         return varioSqlFlightHelper.readData(buffer, maxLen);
                                                                     });

    return response;
}

String VarioWebHandler::getFileSizeStringFromBytes(int bytes)
{
    // conversion taille
    /***********************************/
    String fsize = "";

    if (bytes < 1024)
        fsize = String(bytes) + " B";
    else if (bytes < (1024 * 1024))
        fsize = String(bytes / 1024.0, 3) + " KB";
    else if (bytes < (1024 * 1024 * 1024))
        fsize = String(bytes / 1024.0 / 1024.0, 3) + " MB";
    else
        fsize = String(bytes / 1024.0 / 1024.0 / 1024.0, 3) + " GB";

    return fsize;
}

void VarioWebHandler::backupFile(String pathOrig, String pathBack)
{
    if (SD.exists((char *)pathBack.c_str()))
    {
        SD.remove((char *)pathBack.c_str());
    }
    size_t n;
    uint8_t buf[64];

    if (SD.exists((char *)pathOrig.c_str()))
    {
        File dataFile;
        dataFile = SD.open(pathOrig.c_str(), FILE_READ);
        File dataFile2;
        dataFile2 = SD.open(pathBack.c_str(), FILE_WRITE);

        while ((n = dataFile.read(buf, sizeof(buf))) > 0)
        {
            dataFile2.write(buf, n);
        }
        dataFile.close();
        dataFile2.close();

        SD.remove((char *)pathOrig.c_str());
    }
}

void VarioWebHandler::handleSetFlight(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleSetFlight");

    VarioSqlFlight varioSqlFlight;
    char content[len];
    strncpy(content, (char *)data, len);
    content[len] = '\0';
    if (request->hasParam("id", false))
    {
        AsyncWebParameter *p = request->getParam("id");
        uint8_t id = p->value().toInt();

        VARIO_WIFI_DEBUG_PRINTLN(id);
        VARIO_WIFI_DEBUG_PRINTLN(content);

        varioSqlFlight.updateFlight(id, jsonToIgcdata(content));
    }
    else
    {
        VARIO_WIFI_DEBUG_PRINTLN(content);

        varioSqlFlight.insertFlight(jsonToIgcdata(content));
    }

    request->send(200);

    return;
}

AsyncWebServerResponse *VarioWebHandler::handleDelFlight(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response;

    if (request->hasParam("filename"))
    {
        AsyncWebParameter *p = request->getParam("filename");
        String filename = p->value().c_str();

        File dataFile;
        if (dataFile = SD.open("/vols/parsed/" + filename, FILE_READ))
        {
            dataFile.close();
            SD.rename("/vols/parsed/" + filename, "/vols/" + filename);
        }
    }

    VarioSqlFlight varioSqlFlight;

    if (request->hasParam("id"))
    {
        AsyncWebParameter *p = request->getParam("id");
        uint8_t id = p->value().toInt();

        VARIO_WIFI_DEBUG_PRINTLN(id);

        varioSqlFlight.delFlight(id);
    }

    response = request->beginResponse(200, "text/plain", "OK");
    return response;
}

AsyncWebServerResponse *VarioWebHandler::handleFirmwareVersion(AsyncWebServerRequest *request)
{
    //recuperation des versions de firmware
    VARIO_WIFI_DEBUG_PRINTLN("handleFirmwareVersion");

    AsyncWebServerResponse *response = request->beginChunkedResponse("application/json", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
                                                                     {
                                                                         //Write up to "maxLen" bytes into "buffer" and return the amount written.
                                                                         //index equals the amount of bytes that have been already sent
                                                                         //You will be asked for more data until 0 is returned
                                                                         //Keep in mind that you can not delay or yield waiting for more data!
                                                                         if (!index)
                                                                         {
                                                                             String output = esp32FOTA.getHTTPVersion();
                                                                             memcpy(buffer, (char *)output.c_str(), output.length());
                                                                             return output.length();
                                                                         }
                                                                         return 0;
                                                                     });

    return response;
}

AsyncWebServerResponse *VarioWebHandler::handleUpgradeWeb(AsyncWebServerRequest *request)
{

    AsyncWebServerResponse *response;

    bool betaversion;
    if (request->hasParam("beta"))
    {
        AsyncWebParameter *p = request->getParam("beta");
        betaversion = p->value().toInt();
    }
    else
    {
        response = request->beginResponse(500, "text/plain", "BAD ARGS");
        return response;
    }

    int8_t updatedNeeded = esp32FOTA.execHTTPcheck(betaversion);
    if (updatedNeeded == 1)
    {
        VARIO_WIFI_DEBUG_PRINTLN("############Update firmware########");

        response = request->beginResponse(200);
        esp32FOTA.execOTA();
        return response;
    }
    else
    {
        response = request->beginResponse(500, "text/plain", "could not update");
        return response;
    }
}

AsyncWebServerResponse *VarioWebHandler::handleGetSites(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    VarioSqlFlight varioSqlFlight;

    response->print(varioSqlFlight.getSites());

    return response;
}

void VarioWebHandler::handleSetSite(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    VARIO_WIFI_DEBUG_PRINTLN("handleSetSite");

    VarioSqlFlight varioSqlFlight;
    char content[len];
    strncpy(content, (char *)data, len);
    content[len] = '\0';
    if (request->hasParam("id", false))
    {
        AsyncWebParameter *p = request->getParam("id");
        uint8_t id = p->value().toInt();

        VARIO_WIFI_DEBUG_PRINTLN("Update Site");
        VARIO_WIFI_DEBUG_PRINT("ID : ");
        VARIO_WIFI_DEBUG_PRINTLN(id);
        VARIO_WIFI_DEBUG_PRINT("CONTENT : ");
        VARIO_WIFI_DEBUG_PRINTLN(content);

        varioSqlFlight.updateSite(id, content);
    }
    else
    {
        VARIO_WIFI_DEBUG_PRINTLN("Insert Site");
        VARIO_WIFI_DEBUG_PRINT("CONTENT : ");
        VARIO_WIFI_DEBUG_PRINTLN(content);

        if (!varioSqlFlight.insertSite(content))
        {
            request->send(500);

            return;
        };
    }

    request->send(200);

    return;
}

AsyncWebServerResponse *VarioWebHandler::handleDelSite(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response;

    VarioSqlFlight varioSqlFlight;

    if (request->hasParam("id"))
    {
        AsyncWebParameter *p = request->getParam("id");
        uint8_t id = p->value().toInt();

        VARIO_WIFI_DEBUG_PRINTLN(id);

        varioSqlFlight.deleteSite(id);
    }

    response = request->beginResponse(200, "text/plain", "OK");

    return response;
}

void VarioWebHandler::_doParseIgcAndInsert(void *parameter)
{
    const String ParsedPath PROGMEM = "/vols/parsed";
    uint32_t dataToSend = 0;

    VARIO_WIFI_DEBUG_PRINTLN("_doParseIgcAndInsert");

    String path = String((char *)parameter);
    File dataFile;
    //test présence fichier
    if (dataFile = SD.open(path, FILE_READ))
    {
        String tmpFullName = dataFile.name();
        dataFile.close();

        //parsage du fichier IGC
        VarioIgcParser varioIgcParser(path);
        varioIgcParser.parseFile();

        VarioSqlFlight varioSqlFlight;
        if (varioSqlFlight.insertFlight(varioIgcParser.getIgcdata()))
        {
            String filename = tmpFullName.substring(tmpFullName.lastIndexOf("/") + 1);

            VARIO_WIFI_DEBUG_PRINT("Path origine: ");
            VARIO_WIFI_DEBUG_PRINTLN(path);

            VARIO_WIFI_DEBUG_PRINT("Filename a deplacer: ");
            VARIO_WIFI_DEBUG_PRINTLN(filename);

            if (!SD.exists(ParsedPath))
            {
                SD.mkdir(ParsedPath);
            }

            if (SD.exists(ParsedPath + "/" + filename))
            {
                SD.remove(ParsedPath + "/" + filename);
            }

            SD.rename(path, ParsedPath + "/" + filename);

            VARIO_WIFI_DEBUG_PRINTLN("Deplacement du fichier termine");

            // response = request->beginResponse(200, "text/plain", "OK");
            // return response;
        }
        else
        {
            VARIO_WIFI_DEBUG_PRINTLN("ECHEC de l insertion");

            dataToSend = 2;

            // response = request->beginResponse(500, "text/plain", "CANNOT INSERT FLIGHT");
            // return response;
        }

        varioIgcParser.~VarioIgcParser();
        varioSqlFlight.~VarioSqlFlight();
    }

    xQueueSend(xQueueParse, &dataToSend, 0);
    vTaskDelete(NULL);

    return;
}

igcdata VarioWebHandler::jsonToIgcdata(String data)
{
    igcdata myIgcData;

    VarioTool::jsonDoc.clear();

    DeserializationError err = deserializeJson(VarioTool::jsonDoc, data);
    if (err)
    {
#ifdef SQL_DEBUG
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
#endif //SQL_DEBUG

        return myIgcData;
    }

    if (VarioTool::jsonDoc.containsKey("pilot"))
    {
        myIgcData.pilot = VarioTool::jsonDoc["pilot"].as<String>();
    }
    if (VarioTool::jsonDoc.containsKey("wing"))
    {
        myIgcData.wing = VarioTool::jsonDoc["wing"].as<String>();
    }
    if (VarioTool::jsonDoc.containsKey("flightDate"))
    {
        myIgcData.flightDate = VarioTool::jsonDoc["flightDate"].as<String>();
    }
    if (VarioTool::jsonDoc.containsKey("startFlightTime"))
    {
        myIgcData.startFlightTime = VarioTool::jsonDoc["startFlightTime"].as<String>();
    }
    if (VarioTool::jsonDoc.containsKey("endFlightTime"))
    {
        myIgcData.endFlightTime = VarioTool::jsonDoc["endFlightTime"].as<String>();
    }
    if (VarioTool::jsonDoc.containsKey("startHeight"))
    {
        myIgcData.startHeight = VarioTool::jsonDoc["startHeight"];
    }
    if (VarioTool::jsonDoc.containsKey("endHeight"))
    {
        myIgcData.endHeight = VarioTool::jsonDoc["endHeight"];
    }
    if (VarioTool::jsonDoc.containsKey("minHeight"))
    {
        myIgcData.minHeight = VarioTool::jsonDoc["minHeight"];
    }
    if (VarioTool::jsonDoc.containsKey("maxHeight"))
    {
        myIgcData.maxHeight = VarioTool::jsonDoc["maxHeight"];
    }
    if (VarioTool::jsonDoc.containsKey("startLat"))
    {
        myIgcData.startLat = VarioTool::jsonDoc["startLat"];
    }
    if (VarioTool::jsonDoc.containsKey("startLon"))
    {
        myIgcData.startLon = VarioTool::jsonDoc["startLon"];
    }
    if (VarioTool::jsonDoc.containsKey("endLat"))
    {
        myIgcData.endLat = VarioTool::jsonDoc["endLat"];
    }
    if (VarioTool::jsonDoc.containsKey("endLon"))
    {
        myIgcData.endLon = VarioTool::jsonDoc["endLon"];
    }
    if (VarioTool::jsonDoc.containsKey("md5"))
    {
        myIgcData.md5 = VarioTool::jsonDoc["md5"].as<String>();
    }
    if (VarioTool::jsonDoc.containsKey("filename"))
    {
        myIgcData.filename = VarioTool::jsonDoc["filename"].as<String>();
    }
    if (VarioTool::jsonDoc.containsKey("site_id"))
    {
        myIgcData.site_id = VarioTool::jsonDoc["site_id"];
    }
    if (VarioTool::jsonDoc.containsKey("comment"))
    {
        myIgcData.comment = VarioTool::jsonDoc["comment"].as<String>();
    }

    return myIgcData;
}

AsyncWebServerResponse *VarioWebHandler::getFlightsShort(AsyncWebServerRequest *request)
{

    VarioSqlFlight varioSqlFlight;

    String mode;
    String parcel;
    bool error = false;

    if (request->hasParam("mode"))
    {
        AsyncWebParameter *p = request->getParam("mode");
        mode = p->value().c_str();
    }
    else
    {
        error = true;
    }

    if (request->hasParam("parcel"))
    {
        AsyncWebParameter *p = request->getParam("parcel");
        parcel = p->value().c_str();
    }
    else
    {
        if (mode == "Y")
        {
            parcel = "";
        }
        else
        {
            error = true;
        }
    }

    if (error)
    {
        AsyncWebServerResponse *responseError;
        responseError = request->beginResponse(500, "text/plain", "BAD ARGS");

        return responseError;
    }

    AsyncResponseStream *response;
    response = request->beginResponseStream("application/json");

    response->print(varioSqlFlight.getFlightsShort(mode, parcel));

    return response;
}
