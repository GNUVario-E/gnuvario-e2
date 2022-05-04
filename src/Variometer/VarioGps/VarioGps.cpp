#include "VarioGps.h"
#include <Arduino.h>
#include "VarioDebug/VarioDebug.h"
#include "Variometer/SerialNmea/SerialNmea.h"
#include "Variometer/kalmanvert/kalmanvert.h"
// #include <HardwareConfig.h>

// #include <SerialNmea.h>
// #include <NmeaParser.h>
// #include <LxnavSentence.h>
// #include <LK8Sentence.h>
// #include <IGCSentence.h>
// #include <GPSSentence.h>

// *********************
void VarioGps::init()
// *********************
{
        serialNmea.begin(9600, true);

        VARIO_GPS_DEBUG_PRINTLN("SerialNmea init");
}

// *********************
bool VarioGps::update(Kalmanvert *kalmanvert, bool *lastSentence)
// *********************
{
        /* try to lock sentences */
        if (serialNmea.lockRMC())
        {
                VARIO_GPS_DEBUG_PRINTLN("mneaParser : beginRMC");

                nmeaParser.beginRMC();
        }
        else if (serialNmea.lockGGA())
        {
                VARIO_GPS_DEBUG_PRINTLN("mneaParser : beginGGA");

                nmeaParser.beginGGA();

#ifdef HAVE_BLUETOOTH
                *lastSentence = true;
#endif //HAVE_BLUETOOTH
#ifdef HAVE_SDCARD
                /* start to write IGC B frames */
                if (!GnuSettings.NO_RECORD)
                        igcSD.writePosition(kalmanvert);
#endif //HAVE_SDCARD
        }

        /* parse if needed */
        if (nmeaParser.isParsing())
        {
                VARIO_GPS_DEBUG_PRINTLN("mneaParser : isParsing");

                VARIO_SDCARD_DEBUG_PRINTLN("writeGGA : ");

                while (nmeaParser.isParsing())
                {
                        uint8_t c = serialNmea.read();

                        /* parse sentence */
                        nmeaParser.feed(c);

                        VARIO_NMEAPARSER_DEBUG_PRINT(c);

#ifdef HAVE_SDCARD
                        /* if GGA, convert to IGC and write to sdcard */
                        if (sdcardState == SDCARD_STATE_READY && nmeaParser.isParsingGGA())
                        {
                                igc.feed(c);
                                /*          while( igc.available() ) {
					fileIgc.write( igc.get() );
				}*/
                                if (!GnuSettings.NO_RECORD)
                                        igcSD.writeGGA();
                        }
#endif //HAVE_SDCARD
                }

                VARIO_NMEAPARSER_DEBUG_PRINTLN("");

                serialNmea.release();
#ifdef HAVE_SDCARD
                fileIgc.flush();
#endif //HAVE_SDCARD
                VARIO_NMEAPARSER_DEBUG_PRINTLN("");

                return true;
        }
        return false;
}
