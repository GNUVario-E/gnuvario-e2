#ifndef VARIO_IGC_PARSER_H
#define VARIO_IGC_PARSER_H

#include "VarioIgcParser/igcdata.h"
#include "VarioSettings/VarioSettings.h"
#include "VarioParameter/Parameters.h"

class VarioIgcParser
{

private:
    Parameters *params = Parameters::getInstance();

    boolean isParsed = false;
    String filePath;
    struct igcdata myIgcData;
    void correctTimeZone(String &hms);

public:
    VarioIgcParser(String path);
    ~VarioIgcParser();
    boolean parseFile();
    String getFilename();
    igcdata getIgcdata();
};

#endif