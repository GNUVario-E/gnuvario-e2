
#include <Arduino.h>
#include <VarioData.h>
#include "version.h"
//#include "VarioTool/VarioTool.h"
#include "VarioDebug/VarioDebug.h"

VarioData::VarioData()
{
}

VarioDataType *VarioData::getParam(const char *paramName)
{
  for (int i = 0; i < sizeof(Params) / sizeof(VarioDataType); i++)
  {
    if (strcmp(paramName, Params[i].getName()) == 0)
    {
      return &Params[i];
    }
  }

  VARIO_SDCARD_DEBUG_PRINTLN("Parameter not found");

  return NULL;
}

boolean VarioData::init()
{
  return true;
}

/* Apply the value to the parameter by searching for the parameter name */
boolean VarioData::applySettingParam(const char *settingName, const char *settingValue)
{
  VarioDataType *vparam;
  vparam = getParam(settingName);

  if (vparam != NULL)
  {
    return vparam->setValue(settingValue);
  }

  VARIO_SDCARD_DEBUG_PRINTLN("Param introuvable :");
  VARIO_SDCARD_DEBUG_DUMP(settingName);
  //Serial.print("Param introuvable :");
  //Serial.println(settingName);

  return false;
}

VarioDataType *VarioData::getData(const char *dataName)
{
  for (int i = 0; i < sizeof(Datas) / sizeof(VarioDataType); i++)
  {
    if (strcmp(dataName, Datas[i].getName()) == 0)
    {
      return &Datas[i];
    }
  }

  VARIO_SDCARD_DEBUG_PRINTLN("Parameter not found");

  return NULL;
}

boolean VarioData::setData(const char *dataName, const char *dataValue)
{
  VarioDataType *vdata;
  vdata = getData(dataName);

  if (vdata != NULL)
  {
    return vdata->setValue(dataValue);
  }

//  Serial.print("Param introuvable :");
//  Serial.println(dataName);
  VARIO_SDCARD_DEBUG_PRINTLN("Param introuvable :");
  VARIO_SDCARD_DEBUG_DUMP(dataName);

  return false;
}

VarioData varioData;