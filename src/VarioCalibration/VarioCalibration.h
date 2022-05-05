

#ifndef VARIOCALIBRATION_H
#define VARIOCALIBRATION_H

#include <Arduino.h>
#include "VarioDebug/VarioDebug.h"
#include <SD.h>
#include "VarioSettings/VarioSettings.h"
#include "VarioBeeper/VarioBeeper.h"
#include "Variometer/IntTW/IntTW.h"
#include "Variometer/ms5611TW/ms5611TW.h"
#include "Variometer/vertaccel/vertaccel.h"
#include "Variometer/LightInvensense/LightInvensense.h"
#include "Variometer/TwoWireScheduler/TwoWireScheduler.h"

#include "digit/digit.h"

/* where output ? */
#define SERIAL_OUTPUT
#define SDCARD_OUTPUT

#define RECORD_STATE_INITIAL 0
#define RECORD_STATE_WAIT_DONE 1
#define RECORD_STATE_ACCEL_SD_RECORDED 2
#define RECORD_STATE_GYRO_CALIBRATED 3

class VarioCalibration
{

private:
	const TickType_t delayT50 = pdMS_TO_TICKS(50);
	TaskHandle_t _taskCalibHandle = NULL;
	static void startTaskImpl(void *);

	void task();

	const TickType_t delayT100 = pdMS_TO_TICKS(100);
	VarioBeeper *varioBeeper = NULL;
	/********************************/
	/* Standard deviation recording */
	/********************************/
	unsigned long accelSDRecordTimestamp;
	double rawAccelSD;

	int recordInitState = RECORD_STATE_INITIAL;

	double referencePressure;

	/*****************/
	/* measures data */
	/*****************/
	unsigned long measureTimestamp;

	/* accel measures */
	int16_t lastAccelMeasure[3];
	long accelCount;
	double accelMean[3];
	double accelSD[3];

	/* mag measures */
	int16_t lastMagMeasure[3];
	long magCount;
	double magMean[3];
	double magSD[3];

	File file;

	boolean sdcardFound = false;
	char filename[15] = "/RECORD00.CAL";

	Digit valueDigit;

	void writeNumber(int16_t number);

	void startMeasure(void);
	uint8_t readRawAccel(int16_t *accel, int32_t *quat);

	/*******************************/
	uint8_t readRawMag(int16_t *mag);

	void makeMeasureStep(void);
	double getAccelMeasure(int16_t *accelMeasure);

	/* return standard deviation */
	/*******************************/
	double getMagMeasure(int16_t *magMeasure);

public:
	void begin(VarioBeeper *_varioBeeper);
	void startTask();
};

#endif
