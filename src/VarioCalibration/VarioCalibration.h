
#pragma once

#include "FC.h"
#include <Arduino.h>
#include "VarioDebug/VarioDebug.h"
#include "VarioSettings/VarioSettings.h"
#include "VarioBeeper/VarioBeeper.h"
#include "SparkFun_BNO080_Arduino_Library.h"

#include "digit/digit.h"

/* where output ? */
#define SERIAL_OUTPUT
#define SDCARD_OUTPUT

#define RECORD_STATE_INITIAL 0
#define RECORD_STATE_WAIT_DONE 1
#define RECORD_STATE_ACCEL_SD_RECORDED 2
#define RECORD_STATE_GYRO_CALIBRATED 3

#define SDA_PIN 27
#define SCL_PIN 32
#define POWER_PIN 12
#define POWER_PIN_STATE HIGH 

class VarioCalibration: public Observer
{

private:
    BNO080 myIMU;
	const TickType_t delayT50 = pdMS_TO_TICKS(50);
	TaskHandle_t _taskCalibHandle = NULL;
	static void startTaskImpl(void *);
    char* printAccuracyLevel(byte accuracyNumber);
	void task();

	const TickType_t delayT100 = pdMS_TO_TICKS(100);
	VarioBeeper *varioBeeper = NULL;
	void saveCalibate();

public:
	void begin(VarioBeeper *_varioBeeper);
	void startTask();
	void onSignalReceived(uint8_t _val);
};
extern FC fc;