// #define CLEAR_NVS

#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include "VarioManager.h"
#include "VarioFSM/VarioFSM.h"
#include <FC.h>
// global screen data objects
#include "DisplayData.h"

#include "VarioParameter/Parameters.h"

#ifdef CLEAR_NVS
#include <nvs_flash.h>
#endif

const TickType_t delayT1000 = pdMS_TO_TICKS(1000);

FC fc;

VarioManager vm;
VarioFSM fsm;

Parameters *params = Parameters::getInstance();

#ifdef CLEAR_NVS
void clearNVS()
{
  Serial.println("Clear NVS");
  nvs_flash_erase();
  nvs_flash_init();
  Serial.println("NVS is empty");
}
#endif

void setup()
{
  Serial.begin(115200);

#ifdef CLEAR_NVS
  clearNVS();
  return;
#endif

  // initialisation des valeurs par defaut des parametres
  params->initDefault();

  // initialisation
  if (!vm.init())
  {
    while (1)
    {
      vTaskDelay(delayT1000);
    }
  }
}

void loop()
{
}