#include "VarioCalibration.h"
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "VarioSettings/VarioSettings.h"
#include "HardwareConfig/HardwareConfig.h"
#include "VarioDebug/VarioDebug.h"

#include "VarioBeeper/VarioBeeper.h"

#include "IntTW/IntTW.h"
#include "TwoWireScheduler/TwoWireScheduler.h"

Ms5611 TWScheduler::ms5611;
Vertaccel TWScheduler::vertaccel;

/* need beeps */
#define MAKE_BEEP
#define BEEP_DURATION 300
#define BEEP_VOLUME 10
#define BEEP_START_FREQ 500
#define BEEP_RECORD_FREQ 1000

/* to compute the standard deviation of the accelerometer */
#define ACCEL_SD_WAIT_DURATION 2000
#define ACCEL_SD_MEASURE_DURATION 8000

/* to make averaged measure */
#define MEASURE_DURATION 500
#define STABILIZE_DURATION 500

/* movement detection */
#define PREDICTION_INTERVAL_COEFFICIENT 1.96

/* orientation change detection */
#define NEW_MEASURE_MINIMAL_DEVIATION_COEFF 20.0

VarioCalibration Calibration;

#if defined(SDCARD_OUTPUT)

void VarioCalibration::writeNumber(int16_t number)
{
	valueDigit.begin((long)number);
	while (valueDigit.available())
	{
		file.write(valueDigit.get());
	}
}
#endif //SDCARD_OUTPUT

void VarioCalibration::startMeasure(void)
{
	/* stabilize */
	// just wait
	unsigned long stabilizeTimestamp = millis();
	while (millis() - stabilizeTimestamp < STABILIZE_DURATION)
	{
		//makeMeasureStep();

		vTaskDelay(delayT100);
	}

	VARIO_CAL_DEBUG_PRINTLN("init Var");

	/* init vars */
	measureTimestamp = millis();
	accelCount = 0;
#ifdef AK89xx_SECONDARY
	magCount = 0;
#endif

	for (int i = 0; i < 3; i++)
	{
		accelMean[i] = 0.0;
		accelSD[i] = 0.0;
#ifdef AK89xx_SECONDARY
		magMean[i] = 0.0;
		magSD[i] = 0.0;
#endif
	}
}

uint8_t VarioCalibration::readRawAccel(int16_t *accel, int32_t *quat)
{
	VARIO_CAL_DEBUG_PRINTLN("readRawAccel");
	if (twScheduler.haveAccel())
	{
		VARIO_CAL_DEBUG_PRINTLN("haveAccel");
		twScheduler.getRawAccel(accel, quat);
		return 1;
	}

	return 0;
}

#ifdef AK89xx_SECONDARY
uint8_t VarioCalibration::readRawMag(int16_t *mag)
{
	VARIO_CAL_DEBUG_PRINTLN("readRawMag");

	if (twScheduler.haveMag())
	{
		VARIO_CAL_DEBUG_PRINTLN("haveMag");
		twScheduler.getRawMag(mag);
		return 1;
	}

	return 0;
}

#endif //AK89xx_SECONDARY

void VarioCalibration::makeMeasureStep(void)
{
	/* accel */
	int16_t accel[3];
	int32_t quat[4];
	if (readRawAccel(accel, quat))
	{

		VARIO_CAL_DEBUG_PRINTLN("makeMeasureStep : readRawAccel OK");

		accelCount++;
		for (int i = 0; i < 3; i++)
		{

			accelMean[i] += (double)accel[i];
			accelSD[i] += ((double)accel[i]) * ((double)accel[i]);

			VARIO_CAL_DEBUG_PRINT("accelMean : ");
			VARIO_CAL_DEBUG_PRINTLN(accelMean[i]);
			VARIO_CAL_DEBUG_PRINTLN("accelSD : ");
			VARIO_CAL_DEBUG_PRINTLN(accelSD[i]);
		}
	}

#ifdef AK89xx_SECONDARY
	/* mag */
	int16_t mag[3];
	if (readRawMag(mag))
	{
		VARIO_CAL_DEBUG_PRINTLN("makeMeasureStep : readRawMag OK");

		magCount++;
		for (int i = 0; i < 3; i++)
		{
			magMean[i] += (double)mag[i];
			magSD[i] += ((double)mag[i]) * ((double)mag[i]);

			VARIO_CAL_DEBUG_PRINT("magMean : ");
			VARIO_CAL_DEBUG_PRINTLN(magMean[i]);
			VARIO_CAL_DEBUG_PRINT("magSD : ");
			VARIO_CAL_DEBUG_PRINTLN(magSD[i]);
		}
	}
#endif //AK89xx_SECONDARY
}

/* return standard deviation */
double VarioCalibration::getAccelMeasure(int16_t *accelMeasure)
{
	double accelMeasureSD = 0.0;

	VARIO_CAL_DEBUG_PRINTLN("GetAccelMeasure");

	for (int i = 0; i < 3; i++)
	{
		accelMeasureSD += accelSD[i] / (double)accelCount;
		accelMeasureSD -= (accelMean[i] / (double)accelCount) * (accelMean[i] / (double)accelCount);

		accelMeasure[i] = (int16_t)(accelMean[i] / (double)accelCount);

		VARIO_CAL_DEBUG_PRINT("accelMeasure : ");
		VARIO_CAL_DEBUG_PRINTLN(accelMeasure[i]);
	}

	return sqrt(accelMeasureSD);
}

#ifdef AK89xx_SECONDARY
/* return standard deviation */
double VarioCalibration::getMagMeasure(int16_t *magMeasure)
{
	VARIO_CAL_DEBUG_PRINTLN("GetMagMeasure");

	double magMeasureSD = 0.0;

	for (int i = 0; i < 3; i++)
	{
		magMeasureSD += magSD[i] / (double)magCount;
		magMeasureSD -= (magMean[i] / (double)magCount) * (magMean[i] / (double)magCount);

		magMeasure[i] = (int16_t)(magMean[i] / (double)magCount);

		VARIO_CAL_DEBUG_PRINT("magMeasure : ");
		VARIO_CAL_DEBUG_PRINTLN(magMeasure[i]);
	}

	return sqrt(magMeasureSD);
}
#endif //AK89xx_SECONDARY

void VarioCalibration::begin(VarioBeeper *_varioBeeper)
{
	intTW.begin();
	twScheduler.init();

	varioBeeper = _varioBeeper;
#if defined(SDCARD_OUTPUT)
	sdcardFound = true;
#endif

	varioBeeper->setVolume(10);
	varioBeeper->generateTone(2000, 300);

	vTaskDelay(delayT100 * 30);

	/* reset variables */
	VARIO_CAL_DEBUG_PRINTLN("Begin calibration");

	startMeasure();
	accelSDRecordTimestamp = millis();

	VARIO_CAL_DEBUG_PRINTLN("Loop");

	while (1)
	{
		vTaskDelay(delayT100);

		/*------------------------------------------------------------*/
		/*  wait, record standard deviation, and get gyro calibration */
		/*------------------------------------------------------------*/

		/* first  wait */
		if (recordInitState == RECORD_STATE_INITIAL)
		{

			// VARIO_CAL_DEBUG_PRINTLN("First Wait");

			/* wait finished ? */
			if (millis() - accelSDRecordTimestamp > ACCEL_SD_WAIT_DURATION)
			{

				/* start measure */

				VARIO_CAL_DEBUG_PRINTLN("START MESURE");
				VARIO_CAL_DEBUG_PRINTLN("BEEP");

				varioBeeper->generateTone(BEEP_START_FREQ, BEEP_DURATION, BEEP_VOLUME);

				//	#endif //MAKE_BEEP

				startMeasure();

				/* next step */
				recordInitState = RECORD_STATE_WAIT_DONE;
			}
		}

		/* next record accel SD */
		else if (recordInitState == RECORD_STATE_WAIT_DONE)
		{

			/* accel SD recording finished ? */
			if (millis() - measureTimestamp > ACCEL_SD_MEASURE_DURATION)
			{

				/* get result */
				int16_t measure[3];
				rawAccelSD = getAccelMeasure(measure);

				/* next step */
				recordInitState = RECORD_STATE_ACCEL_SD_RECORDED;
			}

			/* else measure */
			else
			{
				vTaskDelay(delayT100 * 2);
				makeMeasureStep();
			}
		}

		/* next get gyro calibration */
		else if (recordInitState == RECORD_STATE_ACCEL_SD_RECORDED)
		{

			/* check if gyro calibrated */
			unsigned char gyroCal[12];

			fastMPUReadGyroBias(gyroCal);

			bool gyroCalibrated = false;
			for (int i = 0; i < 12; i++)
			{
				if (gyroCal[i] != 0)
				{
					gyroCalibrated = true;
					break;
				}
			}

			if (!gyroCalibrated)
			{
				vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION));
			}
			else
			{
				/* output gyro calibration start measures */

#if defined(SDCARD_OUTPUT)
				if (sdcardFound)
				{
					file = SD.open(filename, FILE_WRITE);
				}
				VARIO_CAL_DEBUG_PRINTLN("WRITE GyroCal");
				writeNumber(gyroCal[0]);
				for (int i = 1; i < 12; i++)
				{
					file.write(',');
					file.write(' ');
					writeNumber(gyroCal[i]);
				}
				file.write('\n');
#endif //SDCARD_OUTPUT

#ifdef SERIAL_OUTPUT
				Serial.print(gyroCal[0], DEC);
				for (int i = 1; i < 12; i++)
				{
					Serial.print(", ");
					Serial.print(gyroCal[i], DEC);
				}
				Serial.print("\n");
#endif //SERIAL_OUTPUT

				/* start recording */
				recordInitState = RECORD_STATE_GYRO_CALIBRATED;
				startMeasure();
			}
		}

		/*-----------------*/
		/* record measures */
		/*-----------------*/
		else
		{

			/****************/
			/* make measure */
			/****************/
			if (millis() - measureTimestamp < MEASURE_DURATION)
			{
				makeMeasureStep();
				vTaskDelay(pdMS_TO_TICKS(10));
			}

			/******************/
			/* output measure */
			/******************/
			else
			{

				int16_t accelMeasure[3];
				double accelMeasureSD = getAccelMeasure(accelMeasure);

#ifdef AK89xx_SECONDARY
				int16_t magMeasure[3];
				double magMeasureSD = getMagMeasure(magMeasure);
#endif //AK89xx_SECONDARY

				/**************************/
				/* check measure validity */
				/**************************/

				/* check measure stability */
				if (accelMeasureSD < PREDICTION_INTERVAL_COEFFICIENT * rawAccelSD)
				{

					/* check deviation with last measure */
					double measureDeviation = 0.0;
					for (int i = 0; i < 3; i++)
					{
						double s = (double)accelMeasure[i] - (double)lastAccelMeasure[i];
						measureDeviation += s * s;
					}
					measureDeviation = sqrt(measureDeviation);

					if (measureDeviation > NEW_MEASURE_MINIMAL_DEVIATION_COEFF * PREDICTION_INTERVAL_COEFFICIENT * rawAccelSD / sqrt((double)accelCount))
					{

						/* save measure */
						for (int i = 0; i < 3; i++)
						{
							lastAccelMeasure[i] = accelMeasure[i];
						}

#ifdef SERIAL_OUTPUT
						/*****************/
						/* serial output */
						/*****************/
						Serial.print(accelMeasure[0], DEC);
						Serial.print(", ");
						Serial.print(accelMeasure[1], DEC);
						Serial.print(", ");
						Serial.print(accelMeasure[2], DEC);
#ifdef AK89xx_SECONDARY
						Serial.print(", ");
						Serial.print(magMeasure[0], DEC);
						Serial.print(", ");
						Serial.print(magMeasure[1], DEC);
						Serial.print(", ");
						Serial.print(magMeasure[2], DEC);
#endif //AK89xx_SECONDARY
						Serial.print("\n");
#endif //SERIAL_OUTPUT

#if defined(SDCARD_OUTPUT)
						/*****************/
						/* SDcard output */
						/*****************/
						writeNumber(accelMeasure[0]);
						file.write(',');
						file.write(' ');
						writeNumber(accelMeasure[1]);
						file.write(',');
						file.write(' ');
						writeNumber(accelMeasure[2]);
#ifdef AK89xx_SECONDARY
						file.write(',');
						file.write(' ');
						writeNumber(magMeasure[0]);
						file.write(',');
						file.write(' ');
						writeNumber(magMeasure[1]);
						file.write(',');
						file.write(' ');
						writeNumber(magMeasure[2]);
#endif //AK89xx_SECONDARY
						file.write('\n');

						file.flush();
#endif //SDCARD_OUTPUT

						VARIO_CAL_DEBUG_PRINTLN("WRITE DATA");

						//	#ifdef MAKE_BEEP
						VARIO_CAL_DEBUG_PRINTLN("BEEP");
						varioBeeper->generateTone(BEEP_RECORD_FREQ, BEEP_DURATION, BEEP_VOLUME);
						//	#endif //MAKE_BEEP
					}
				}

				/* next */
				vTaskDelay(delayT100);
				startMeasure();
			}
		}
	}
}
