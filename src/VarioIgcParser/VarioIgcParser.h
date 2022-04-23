#ifndef VARIO_IGC_PARSER_H
#define VARIO_IGC_PARSER_H

#include <varioIgcParser/igcdata.h>
#include "VarioSettings/VarioSettings.h"

class VarioIgcParser
{

private:
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