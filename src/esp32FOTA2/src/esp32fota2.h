/*
   esp32 firmware OTA
   Date: December 2018
   Author: Chris Joyce <https://chrisjoyce911/esp32FOTA>
   Purpose: Perform an OTA update from a bin located on a webserver (HTTP & HTTPS)
*/

#ifndef esp32fota2_h
#define esp32fota2_h

#define DEST_FS_USES_SD

#include "Arduino.h"

#define MAJ_ERROR -1
#define MAJ_AVAILABLE 1
#define MAJ_NOTAVAILABLE 0

class esp32FOTA2
{
public:
  esp32FOTA2(String firwmareType, int firwmareVersion, int firwmareSubVersion, int firwmareBetaVersion);
  bool forceUpdate(String firwmareHost, int firwmarePort, String firwmarePath);
  void execOTA();
  bool execHTTPexist();
  uint8_t execHTTPcheck(bool betaVersion = false);
  bool useDeviceID;
  String checkURL;
  int UpdateVersion;
  int UpdateSubVersion;
  int UpdateBetaVersion;
  String getHTTPVersion();
  int8_t UpdateWwwDirectoryFromGz();

private:
  String getHeaderValue(String header, String headerName);
  String getDeviceID();
  String _firwmareType;
  int _firwmareVersion;
  int _firwmareSubVersion;
  int _firwmareBetaVersion;
  String _host;
  String _bin;
  String _gzwwwfile;
  int _port;
  void downloadWwwFiles();
  void downloadGzwwwFile();
};

#endif