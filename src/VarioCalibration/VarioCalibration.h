

#ifndef VARIOCALIBRATION_H
#define VARIOCALIBRATION_H

#include <Arduino.h>
#include "VarioDebug/VarioDebug.h"
#include <SD.h>
#include "VarioSettings/VarioSettings.h"
#include "VarioBeeper/VarioBeeper.h"
#include "IntTW/IntTW.h"
#include "ms5611TW/ms5611TW.h"
#include "vertaccel/vertaccel.h"
#include "LightInvensense/LightInvensense.h"
#include "TwoWireScheduler/TwoWireScheduler.h"

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

public:
	void begin(VarioBeeper *_varioBeeper);

private:
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

#ifdef AK89xx_SECONDARY
	/* mag measures */
	int16_t lastMagMeasure[3];
	long magCount;
	double magMean[3];
	double magSD[3];
#endif //AK89xx_SECONDARY

	File file;

	boolean sdcardFound = false;
	char filename[15] = "/RECORD00.CAL";

	Digit valueDigit;

	void writeNumber(int16_t number);

	void startMeasure(void);
	uint8_t readRawAccel(int16_t *accel, int32_t *quat);

#ifdef AK89xx_SECONDARY
	/*******************************/
	uint8_t readRawMag(int16_t *mag);
#endif //AK89xx_SECONDARY

	void makeMeasureStep(void);
	double getAccelMeasure(int16_t *accelMeasure);

#ifdef AK89xx_SECONDARY
	/* return standard deviation */
	/*******************************/
	double getMagMeasure(int16_t *magMeasure);
#endif //AK89xx_SECONDARY
};

#endif
