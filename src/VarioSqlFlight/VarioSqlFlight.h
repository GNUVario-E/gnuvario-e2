#ifndef VARIO_SQL_FLIGHT_H
#define VARIO_SQL_FLIGHT_H

#include "varioSqlFlight/RingBuf.h"
#include <vario_esp32_arduino_sqlite3_lib/src/sqlite3.h>
#include <varioIgcParser/igcdata.h>

class VarioSqlFlight
{

private:
    String dbPath;
    sqlite3 *myDb;
    boolean isOpened;
    boolean haveNextFlight;
    sqlite3_stmt *nextFlightRes;
    int openDb(const char *filename, sqlite3 **db);
    void closeDb();
    char *zErrMsg = 0;
    int db_exec(sqlite3 *db, const char *sql);
    const char *data = "Callback function called";
    static int callback(void *data, int argc, char **argv, char **azColName);

public:
    VarioSqlFlight();
    ~VarioSqlFlight();
    bool insertFlight(igcdata myIgcData);
    bool updateFlight(uint8_t id, igcdata myIgcData);
    bool delFlight(uint8_t id);
    bool updateFlightMap(uint8_t id, String data);
    String getSites();
    bool insertSite(String data);
    bool updateSite(uint8_t id, String data);
    bool deleteSite(uint8_t id);
    bool initGetFlightsQuery(uint16_t limit, uint16_t offset);
    bool initGetFlightsQuery(String parcel);
    //String getNextFlight();
    bool getNextFlight(bool &firstline, RingBuf<char, 1024> &buffer);
    void executeMigration(String version, String sql);
    String escapeJson(const String &s);
    String getFlightsShort(String mode, String parcel);
};
#endif