#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include "VarioManager.h"
#include "VarioFSM/VarioFSM.h"
#include <FC.h>
// global screen data objects
#include "DisplayData.h"

const TickType_t delayT1000 = pdMS_TO_TICKS(1000);

FC fc;

VarioManager vm;
VarioFSM fsm;

void setup()
{
  Serial.begin(115200);

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