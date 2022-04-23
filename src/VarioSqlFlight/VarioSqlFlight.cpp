#include <Arduino.h>

#include <ArduinoJson.h>
#include "varioSqlFlight/VarioSqlFlight.h"
#include "VarioSettings.h"
#include "VarioDebug/VarioDebug.h"
#include <SD.h>
#include "VarioTool/VarioTool.h"

VarioSqlFlight::VarioSqlFlight()
{
    VARIO_SQL_DEBUG_PRINTLN("VarioSqlFlight constructeur");

    dbPath = "/sd/db/vol.db";
    isOpened = false;

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_initialize");

    sqlite3_initialize();

    haveNextFlight = false;
}

VarioSqlFlight::~VarioSqlFlight()
{
    closeDb();
    sqlite3_shutdown();
}

int VarioSqlFlight::callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    Serial.printf("%s: ", (const char *)data);
    for (i = 0; i < argc; i++)
    {
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    Serial.printf("\n");
    return 0;
}

int VarioSqlFlight::openDb(const char *filename, sqlite3 **db)
{
    int rc = sqlite3_open(filename, db);
    if (rc)
    {
        VARIO_SQL_DEBUG_PRINT("Can't open database:");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(*db));

        return rc;
    }
    else
    {
        VARIO_SQL_DEBUG_PRINTLN("Opened database successfully");
    }
    isOpened = true;
    return rc;
}

void VarioSqlFlight::closeDb()
{
    if (isOpened)
    {
        VARIO_SQL_DEBUG_PRINTLN("close db");
        sqlite3_close(myDb);
        isOpened = false;
    }
}

int VarioSqlFlight::db_exec(sqlite3 *db, const char *sql)
{

    VARIO_SQL_DEBUG_PRINTLN(sql);

    int rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("SQL error:");
        VARIO_SQL_DEBUG_PRINTLN(zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        VARIO_SQL_DEBUG_PRINTLN("Operation done successfully");
    }
    return rc;
}

bool VarioSqlFlight::insertFlight(igcdata myIgcData)
{
    VARIO_MEMORY_DEBUG_PRINT("Free heap debut insertFlight");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    VARIO_MEMORY_DEBUG_PRINT("Free heap insertFlight fin opendb");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    String sql;

    if (myIgcData.filename != "" && myIgcData.md5 != "")
    {
        sql = "DELETE FROM flight WHERE filename = ? AND md5 = ?";

        rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
        if (rc != SQLITE_OK)
        {
            VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
            VARIO_SQL_DEBUG_PRINT(sqlite3_errmsg(myDb));
            closeDb();
            return false;
        }

        VARIO_SQL_DEBUG_PRINTLN("Début binding");

        sqlite3_bind_text(res, 1, (char *)myIgcData.filename.c_str(), myIgcData.filename.length(), SQLITE_STATIC);
        sqlite3_bind_text(res, 2, (char *)myIgcData.md5.c_str(), myIgcData.md5.length(), SQLITE_STATIC);

        if (sqlite3_step(res) != SQLITE_DONE)
        {
            closeDb();
            return false;
        }

        sqlite3_clear_bindings(res);

        rc = sqlite3_reset(res);
        if (rc != SQLITE_OK)
        {
            closeDb();
            return false;
        }

        rc = sqlite3_finalize(res);
        if (rc != SQLITE_OK)
        {
            VARIO_SQL_DEBUG_PRINTLN("cannot finalize");
            closeDb();
            return false;
        }
    }

    VARIO_MEMORY_DEBUG_PRINT("Free heap fin delete  insertFlight");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    sql = "INSERT INTO flight (";
    sql = sql + "pilot, wing, flight_date, start_flight_time, end_flight_time, start_height, end_height, min_height, max_height, start_lat, start_lon, end_lat, end_lon";

    if (myIgcData.filename != "" && myIgcData.md5 != "")
    {
        sql = sql + ", filename, md5";
    }
    else
    {
        sql = sql + ", site_id, comment";
    }

    sql = sql + ") VALUES (?, ?, ? ,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_text(res, 1, (char *)myIgcData.pilot.c_str(), myIgcData.pilot.length(), SQLITE_STATIC);
    sqlite3_bind_text(res, 2, (char *)myIgcData.wing.c_str(), myIgcData.wing.length(), SQLITE_STATIC);
    sqlite3_bind_text(res, 3, (char *)myIgcData.flightDate.c_str(), myIgcData.flightDate.length(), SQLITE_STATIC);
    sqlite3_bind_text(res, 4, (char *)myIgcData.startFlightTime.c_str(), myIgcData.startFlightTime.length(), SQLITE_STATIC);
    sqlite3_bind_text(res, 5, (char *)myIgcData.endFlightTime.c_str(), myIgcData.endFlightTime.length(), SQLITE_STATIC);
    sqlite3_bind_int(res, 6, myIgcData.startHeight);
    sqlite3_bind_int(res, 7, myIgcData.endHeight);
    sqlite3_bind_int(res, 8, myIgcData.minHeight);
    sqlite3_bind_int(res, 9, myIgcData.maxHeight);
    sqlite3_bind_double(res, 10, myIgcData.startLat);
    sqlite3_bind_double(res, 11, myIgcData.startLon);
    sqlite3_bind_double(res, 12, myIgcData.endLat);
    sqlite3_bind_double(res, 13, myIgcData.endLon);
    if (myIgcData.filename != "" && myIgcData.md5 != "")
    {
        sqlite3_bind_text(res, 14, (char *)myIgcData.filename.c_str(), myIgcData.filename.length(), SQLITE_STATIC);
        sqlite3_bind_text(res, 15, (char *)myIgcData.md5.c_str(), myIgcData.md5.length(), SQLITE_STATIC);
    }
    else
    {
        sqlite3_bind_int(res, 14, myIgcData.site_id);
        sqlite3_bind_text(res, 15, (char *)myIgcData.comment.c_str(), myIgcData.comment.length(), SQLITE_STATIC);
    }

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {

        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);

    return true;
}

bool VarioSqlFlight::delFlight(uint8_t id)
{

    VARIO_SQL_DEBUG_PRINTLN(id);

    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    String sql = F("DELETE FROM flight WHERE id = ?");

    rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {

        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_int(res, 1, id);

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {

        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {

        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);

    return true;
}

bool VarioSqlFlight::updateFlight(uint8_t id, igcdata myIgcData)
{
    VARIO_SQL_DEBUG_PRINTLN("updateFlight");
    VARIO_SQL_DEBUG_PRINTLN(data);

    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    String sql = F("UPDATE flight SET site_id = ?, comment = ? ");

    if (myIgcData.pilot != "")
    {
        sql = sql + ", pilot = ?, wing = ?, flight_date = ?, start_flight_time = ?, end_flight_time = ?, start_height = ?, end_height = ?, min_height = ?, max_height = ?, start_lat = ?, start_lon = ?, end_lat = ?, end_lon = ? ";
    }

    sql = sql + " WHERE id = ?";

    rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {

        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_int(res, 1, myIgcData.site_id);
    sqlite3_bind_text(res, 2, (char *)myIgcData.comment.c_str(), myIgcData.comment.length(), SQLITE_STATIC);

    if (myIgcData.pilot != "")
    {
        sqlite3_bind_text(res, 3, (char *)myIgcData.pilot.c_str(), myIgcData.pilot.length(), SQLITE_STATIC);
        sqlite3_bind_text(res, 4, (char *)myIgcData.wing.c_str(), myIgcData.wing.length(), SQLITE_STATIC);
        sqlite3_bind_text(res, 5, (char *)myIgcData.flightDate.c_str(), myIgcData.flightDate.length(), SQLITE_STATIC);
        sqlite3_bind_text(res, 6, (char *)myIgcData.startFlightTime.c_str(), myIgcData.startFlightTime.length(), SQLITE_STATIC);
        sqlite3_bind_text(res, 7, (char *)myIgcData.endFlightTime.c_str(), myIgcData.endFlightTime.length(), SQLITE_STATIC);
        sqlite3_bind_int(res, 8, myIgcData.startHeight);
        sqlite3_bind_int(res, 9, myIgcData.endHeight);
        sqlite3_bind_int(res, 10, myIgcData.minHeight);
        sqlite3_bind_int(res, 11, myIgcData.maxHeight);
        sqlite3_bind_double(res, 12, myIgcData.startLat);
        sqlite3_bind_double(res, 13, myIgcData.startLon);
        sqlite3_bind_double(res, 14, myIgcData.endLat);
        sqlite3_bind_double(res, 15, myIgcData.endLon);
        sqlite3_bind_int(res, 16, id);
    }
    else
    {
        sqlite3_bind_int(res, 3, id);
    }

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {

        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);

    return true;
}

bool VarioSqlFlight::updateFlightMap(uint8_t id, String data)
{
    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    String sql = F("UPDATE flight SET minimap = ? WHERE id = ?");

    rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {

        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_blob(res, 1, (char *)data.c_str(), strlen((char *)data.c_str()), SQLITE_STATIC);
    sqlite3_bind_int(res, 2, id);

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {

        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);

    return true;
}

String VarioSqlFlight::getSites()
{
    String output = "";

    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return output;
        }
    }

    String sql = F("SELECT id, lib, comment, lat, lon FROM site");

    rc = sqlite3_prepare_v2(myDb, sql.c_str(), 1000, &res, &tail);
    if (rc != SQLITE_OK)
    {
        closeDb();
        return output;
    }

    VarioTool::jsonDoc.clear();

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        // rec_count = sqlite3_column_int(res, 0);
        // if (rec_count > 5000)
        // {
        //     sqlite3_finalize(res);
        //     return;
        // }
        //doc.add(i);
        JsonObject obj1 = VarioTool::jsonDoc.createNestedObject();
        obj1["id"] = sqlite3_column_int(res, 0);
        obj1["lib"] = String((char *)sqlite3_column_text(res, 1));
        obj1["comment"] = String((char *)sqlite3_column_text(res, 2));
        obj1["lat"] = sqlite3_column_double(res, 3);
        obj1["lon"] = sqlite3_column_double(res, 4);
    }
    serializeJson(VarioTool::jsonDoc, output);
    sqlite3_finalize(res);
    return output;
}

bool VarioSqlFlight::insertSite(String data)
{
    VARIO_SQL_DEBUG_PRINTLN(data);

    VarioTool::jsonDoc.clear();

    DeserializationError err = deserializeJson(VarioTool::jsonDoc, data);
    if (err)
    {
        VARIO_SQL_DEBUG_PRINT(F("deserializeJson() failed with code "));
        VARIO_SQL_DEBUG_PRINTLN(err.c_str());

        return false;
    }

    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    String sql = F("INSERT INTO site (lib, comment, lat, lon) VALUES (?, ?, ? ,?)");

    rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_text(res, 1, VarioTool::jsonDoc["lib"], strlen(VarioTool::jsonDoc["lib"]), SQLITE_STATIC);
    sqlite3_bind_text(res, 2, VarioTool::jsonDoc["comment"], strlen(VarioTool::jsonDoc["comment"]), SQLITE_STATIC);
    sqlite3_bind_double(res, 3, VarioTool::jsonDoc["lat"]);
    sqlite3_bind_double(res, 4, VarioTool::jsonDoc["lon"]);

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();
        return false;
    }
    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);

    return true;
}

bool VarioSqlFlight::updateSite(uint8_t id, String data)
{
    VARIO_SQL_DEBUG_PRINTLN(data);

    VarioTool::jsonDoc.clear();

    DeserializationError err = deserializeJson(VarioTool::jsonDoc, data);
    if (err)
    {
        VARIO_SQL_DEBUG_PRINT("deserializeJson() failed with code ");
        VARIO_SQL_DEBUG_PRINTLN(err.c_str());

        return false;
    }

    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    String sql = F("UPDATE site SET lib = ?, comment = ?, lat = ?, lon = ? WHERE id = ?");

    rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_text(res, 1, VarioTool::jsonDoc["lib"], strlen(VarioTool::jsonDoc["lib"]), SQLITE_STATIC);
    sqlite3_bind_text(res, 2, VarioTool::jsonDoc["comment"], strlen(VarioTool::jsonDoc["comment"]), SQLITE_STATIC);
    sqlite3_bind_double(res, 3, VarioTool::jsonDoc["lat"]);
    sqlite3_bind_double(res, 4, VarioTool::jsonDoc["lon"]);
    sqlite3_bind_int(res, 5, id);

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();
        return false;
    }
    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);

    VarioTool::jsonDoc.clear();

    return true;
}

bool VarioSqlFlight::deleteSite(uint8_t id)
{
    VARIO_SQL_DEBUG_PRINTLN(id);

    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    String sql = F("DELETE FROM site WHERE id = ?");

    rc = sqlite3_prepare_v2(myDb, (char *)sql.c_str(), sql.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_int(res, 1, id);

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();
        return false;
    }
    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);

    return true;
}

bool VarioSqlFlight::initGetFlightsQuery(uint16_t limit, uint16_t offset)
{
    VARIO_MEMORY_DEBUG_PRINTLN("Free heap initGetFlightsQuery");
    VARIO_MEMORY_DEBUG_PRINT(ESP.getFreeHeap());

    haveNextFlight = false;

    int rc;
    // sqlite3_stmt *res;
    const char *tail;

    if (!isOpened)
    {
        VARIO_SQL_DEBUG_PRINTLN("openDb");

        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    VARIO_MEMORY_DEBUG_PRINTLN("Free heap initGetFlightsQuery after opendb");
    VARIO_MEMORY_DEBUG_PRINT(ESP.getFreeHeap());

    //String sql = F("SELECT f.id, f.site_id, f.filename, f.md5, f.pilot, f.wing, f.flight_date, f.start_flight_time, f.end_flight_time, f.start_height, f.end_height, f.min_height, f.max_height, f.start_lat, f.start_lon, f.end_lat, f.end_lon, f.comment, f.minimap, s.lib FROM flight f LEFT JOIN site s ON(s.id = f.site_id) ORDER BY f.flight_date DESC, f.start_flight_time ASC LIMIT ?  OFFSET ?");
    //String sql = F("SELECT f.id, f.site_id, f.filename, f.md5, f.pilot, f.wing, f.flight_date, f.start_flight_time, f.end_flight_time, f.start_height, f.end_height, f.min_height, f.max_height, f.start_lat, f.start_lon, f.end_lat, f.end_lon, f.comment, f.minimap, 'toto' AS lib FROM flight f");
    String sql = "SELECT f.id, f.site_id, f.filename, f.pilot, f.wing, f.flight_date, f.start_flight_time, f.end_flight_time, f.start_height, f.end_height, f.min_height, f.max_height, f.start_lat, f.start_lon, f.end_lat, f.end_lon, f.comment, s.lib FROM flight f LEFT JOIN site s ON(s.id = f.site_id) LIMIT ?  OFFSET ?";

    rc = sqlite3_prepare_v2(myDb, sql.c_str(), sql.length(), &nextFlightRes, &tail);

    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    sqlite3_bind_int(nextFlightRes, 1, limit);
    sqlite3_bind_int(nextFlightRes, 2, offset);

    haveNextFlight = true;

    VARIO_MEMORY_DEBUG_PRINT("Free heap FIN initGetFlightsQuery");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    return true;
}

bool VarioSqlFlight::initGetFlightsQuery(String parcel)
{
    VARIO_MEMORY_DEBUG_PRINT("Free heap initGetFlightsQuery");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    haveNextFlight = false;

    int rc;
    // sqlite3_stmt *res;
    const char *tail;

    if (!isOpened)
    {
        VARIO_SQL_DEBUG_PRINTLN("openDb");

        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return false;
        }
    }

    VARIO_MEMORY_DEBUG_PRINT("Free heap initGetFlightsQuery after opendb");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    VARIO_SQL_DEBUG_PRINT("Parcel:");
    VARIO_SQL_DEBUG_PRINTLN(parcel);

    //String sql = F("SELECT f.id, f.site_id, f.filename, f.md5, f.pilot, f.wing, f.flight_date, f.start_flight_time, f.end_flight_time, f.start_height, f.end_height, f.min_height, f.max_height, f.start_lat, f.start_lon, f.end_lat, f.end_lon, f.comment, f.minimap, s.lib FROM flight f LEFT JOIN site s ON(s.id = f.site_id) ORDER BY f.flight_date DESC, f.start_flight_time ASC LIMIT ?  OFFSET ?");
    //String sql = F("SELECT f.id, f.site_id, f.filename, f.md5, f.pilot, f.wing, f.flight_date, f.start_flight_time, f.end_flight_time, f.start_height, f.end_height, f.min_height, f.max_height, f.start_lat, f.start_lon, f.end_lat, f.end_lon, f.comment, f.minimap, 'toto' AS lib FROM flight f");
    String sql = "SELECT f.id, f.site_id, f.filename, f.pilot, f.wing, f.flight_date, f.start_flight_time, f.end_flight_time, f.start_height, f.end_height, f.min_height, f.max_height, f.start_lat, f.start_lon, f.end_lat, f.end_lon, f.comment, s.lib FROM flight f LEFT JOIN site s ON(s.id = f.site_id) WHERE strftime('%Y%m', f.flight_date) = ?";

    rc = sqlite3_prepare_v2(myDb, sql.c_str(), sql.length(), &nextFlightRes, &tail);

    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    }

    // parcel = "202005";
    rc = sqlite3_bind_text(nextFlightRes, 1, (char *)parcel.c_str(), parcel.length(), SQLITE_TRANSIENT);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR binding sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();
        return false;
    };

    haveNextFlight = true;

    VARIO_MEMORY_DEBUG_PRINT("Free heap FIN initGetFlightsQuery");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

    return true;
}

bool VarioSqlFlight::getNextFlight(bool &firstline, RingBuf<char, 1024> &buffer)
{
    VARIO_SQL_DEBUG_PRINTLN("getNextFlight");

    VARIO_MEMORY_DEBUG_PRINT("Free heap getNextFlight");
    VARIO_MEMORY_DEBUG_PRINTLN(ESP.getFreeHeap());

        int step_res;

    if (haveNextFlight)
    {
        step_res = sqlite3_step(nextFlightRes);

        if (step_res == SQLITE_ROW)
        {
            char unvol[2048] = "";

            String tmp;
            strcat(unvol, "{");

            tmp = "\"id\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 0)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"site_id\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 1)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"filename\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 2)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            // tmp = "\"md5\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 3)));
            // strcat(unvol, tmp.c_str());
            // strcat(unvol, ", ");

            tmp = "\"pilot\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 3)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"wing\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 4)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"flight_date\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 5)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"start_flight_time\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 6)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"end_flight_time\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 7)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"start_height\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 8)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"end_height\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 9)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"min_height\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 10)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"max_height\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 11)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"start_lat\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 12)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"start_lon\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 13)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"end_lat\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 14)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"end_lon\" : " + escapeJson(String(sqlite3_column_int(nextFlightRes, 15)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            tmp = "\"comment\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 16)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, ", ");

            // tmp = "\"minimap\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 18)));
            // strcat(unvol, tmp.c_str());
            // strcat(unvol, ", ");

            tmp = "\"site_lib\" : " + escapeJson(String((char *)sqlite3_column_text(nextFlightRes, 17)));
            strcat(unvol, tmp.c_str());
            strcat(unvol, "}");

            // serializeJson(obj1, unvol);

            VARIO_SQL_DEBUG_PRINT("unvol : ");
            VARIO_SQL_DEBUG_PRINTLN(unvol);

            if (!firstline)
            {
                buffer.push(',');
            }
            else
            {
                firstline = false;
            }

            int i = 0;
            while (unvol[i] != '\0')
            {
                buffer.push(unvol[i]);
                i++;
            }

            return true;
        }
        else if (step_res == SQLITE_ERROR)
        {
            VARIO_SQL_DEBUG_PRINTLN("ERROR ICI");

            haveNextFlight = false;
            sqlite3_clear_bindings(nextFlightRes);
            sqlite3_finalize(nextFlightRes);
            closeDb();

            return false;
        }
        else
        {
            VARIO_SQL_DEBUG_PRINT("Plus aucun vol");
            VARIO_SQL_DEBUG_PRINTLN(step_res);

            haveNextFlight = false;
            sqlite3_clear_bindings(nextFlightRes);
            sqlite3_finalize(nextFlightRes);
            closeDb();
            return false;
        }
    }
    else
    {
        sqlite3_clear_bindings(nextFlightRes);
        sqlite3_finalize(nextFlightRes);
        closeDb();
        VARIO_SQL_DEBUG_PRINTLN("Aucun vol");

        return false;
    }
}

void VarioSqlFlight::executeMigration(String version, String sql)
{
    int rc;
    sqlite3_stmt *res;
    const char *tail;

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return;
        }
    }

    String sqlSelect = "SELECT COUNT(version) AS NB FROM version WHERE version = ?";

    rc = sqlite3_prepare_v2(myDb, sqlSelect.c_str(), 1000, &res, &tail);
    if (rc != SQLITE_OK)
    {
        closeDb();

        return;
    }
    sqlite3_bind_text(res, 1, (char *)version.c_str(), version.length(), SQLITE_STATIC);

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

        if (sqlite3_column_int(res, 0) == 1)
        {
            VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

            sqlite3_clear_bindings(res);
            rc = sqlite3_reset(res);
            if (rc != SQLITE_OK)
            {
                closeDb();
            }
            sqlite3_finalize(res);

            return;
        }
    }

    sqlite3_clear_bindings(res);

    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {
        closeDb();

        return;
    }

    sqlite3_finalize(res);

    rc = sqlite3_prepare_v2(myDb, sql.c_str(), 1000, &res, &tail);
    if (rc != SQLITE_OK)
    {
        closeDb();

        return;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt:");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();

        return;
    }

    sqlite3_finalize(res);

    //insertion de l'enregistrement de version
    String sql2 = "INSERT INTO version (version) VALUES (?)";

    rc = sqlite3_prepare_v2(myDb, (char *)sql2.c_str(), sql2.length(), &res, &tail);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR preparing sql: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();

        return;
    }

    VARIO_SQL_DEBUG_PRINTLN("Début binding");

    sqlite3_bind_text(res, 1, (char *)version.c_str(), version.length(), SQLITE_STATIC);

    VARIO_SQL_DEBUG_PRINTLN("Début step");

    if (sqlite3_step(res) != SQLITE_DONE)
    {
        VARIO_SQL_DEBUG_PRINT("ERROR executing stmt: ");
        VARIO_SQL_DEBUG_PRINTLN(sqlite3_errmsg(myDb));

        closeDb();

        return;
    }

    VARIO_SQL_DEBUG_PRINTLN("sqlite3_clear_bindings");

    sqlite3_clear_bindings(res);
    rc = sqlite3_reset(res);
    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINTLN("reset failed");

        closeDb();

        return;
    }
    VARIO_SQL_DEBUG_PRINTLN("sqlite3_finalize");

    sqlite3_finalize(res);
}

String VarioSqlFlight::escapeJson(const String &s)
{
    if (s == NULL || s.length() == 0)
    {
        return "\"\"";
    }

    char c = 0;
    int i;
    int len = s.length();
    String sb;
    String t;

    sb.concat('"');
    for (i = 0; i < len; i += 1)
    {
        c = s.charAt(i);

        switch (c)
        {
        case '\\':
        case '"':
            sb.concat('\\');
            sb.concat(c);
            break;
        case '/':
            //                if (b == '<') {
            sb.concat('\\');
            //                }
            sb.concat(c);
            break;
        case '\b':
            sb.concat("\\b");
            break;
        case '\t':
            sb.concat("\\t");
            break;
        case '\n':
            sb.concat("\\n");
            break;
        case '\f':
            sb.concat("\\f");
            break;
        case '\r':
            sb.concat("\\r");
            break;
        default:
            if (c < ' ')
            {
                t = "000" + String(c, HEX); // Integer.toHexString(c);
                sb.concat("\\u" + t.substring(t.length() - 4));
            }
            else
            {
                sb.concat(c);
            }
        }
    }

    sb.concat('"');

    return sb;
}

String VarioSqlFlight::getFlightsShort(String mode, String parcel)
{

    String output = "";

    int step_res;
    int rc;
    sqlite3_stmt *res;
    const char *tail;

    VARIO_SQL_DEBUG_PRINTLN("openDb");

    if (!isOpened)
    {
        if (openDb((char *)dbPath.c_str(), &myDb))
        {
            return output;
        }
    }

    String sql;
    sql = "SELECT COUNT(*) AS nf, time(SUM(strftime('%s', end_flight_time)- strftime('%s', start_flight_time)), 'unixepoch') AS du, ";

    if (mode == "Y")
    {
        sql = sql + "strftime('%Y', flight_date) as 'gr' FROM flight GROUP BY strftime('%Y', flight_date)";
    }
    else if (mode == "M")
    {
        //dans ce cas, "parcel" est une année
        sql = sql + "strftime('%Y%m', flight_date) as 'gr' FROM flight WHERE strftime('%Y', flight_date) = ? GROUP BY strftime('%Y%m', flight_date)";
    }

    rc = sqlite3_prepare_v2(myDb, sql.c_str(), 1000, &res, &tail);

    if (mode == "Y")
    {
        //rien
    }
    else if (mode == "M")
    {
        sqlite3_bind_text(res, 1, (char *)parcel.c_str(), parcel.length(), SQLITE_STATIC);
    }

    if (rc != SQLITE_OK)
    {
        VARIO_SQL_DEBUG_PRINTLN("getFlightsShort: query error getFlightsShort");

        closeDb();
        return output;
    };

    VarioTool::jsonDoc.clear();

    while ((step_res = sqlite3_step(res)) == SQLITE_ROW)
    {
        JsonObject obj1 = VarioTool::jsonDoc.createNestedObject();
        obj1["nf"] = sqlite3_column_int(res, 0);
        // obj1["ns"] = sqlite3_column_int(res, 1);
        obj1["du"] = String((char *)sqlite3_column_text(res, 1));
        obj1["gr"] = String((char *)sqlite3_column_text(res, 2));
    }

    if (step_res == SQLITE_ERROR)
    {
        VARIO_SQL_DEBUG_PRINTLN("Error executing query ");

        sqlite3_clear_bindings(res);
        sqlite3_finalize(res);
        closeDb();

        return output;
    }
    else
    {
        VARIO_SQL_DEBUG_PRINT("Step res:");
        VARIO_SQL_DEBUG_PRINTLN(step_res);
    }

    serializeJson(VarioTool::jsonDoc, output);
    sqlite3_finalize(res);

    return output;
}