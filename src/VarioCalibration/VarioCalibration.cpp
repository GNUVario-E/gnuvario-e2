#include "VarioCalibration.h"
#include <Arduino.h>
#include "VarioSettings/VarioSettings.h"
#include "HardwareConfig/HardwareConfig.h"
#include "VarioDebug/VarioDebug.h"

#include "VarioBeeper/VarioBeeper.h"
#include "event.h"


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

#define CALIBRATION_TASK_PRIORITY 9

VarioCalibration Calibration;
char *cal_mag;
char *cal_gyro;

void VarioCalibration::startTask()
{
	// task creation
	VARIO_PROG_DEBUG_PRINTLN("Task Calib started");
	xTaskCreate(this->startTaskImpl, "TaskCalib", 4000, this, CALIBRATION_TASK_PRIORITY, &_taskCalibHandle);
}

void VarioCalibration::startTaskImpl(void *parm)
{
	// wrapper for task
	static_cast<VarioCalibration *>(parm)->task();
}

void VarioCalibration::task()
{
	while (1)
	{
		vTaskDelay(delayT50);
        //Look for reports from the IMU
        if (myIMU.dataAvailable() == true)
        {
            
            byte accuracy = myIMU.getMagAccuracy();
            byte sensorAccuracy = myIMU.getQuatAccuracy();
            cal_mag = printAccuracyLevel(accuracy);
            cal_gyro = printAccuracyLevel(sensorAccuracy);

            fc.setText3(true, cal_mag);
            fc.setText5(true, cal_gyro);
        }
	}
}

void VarioCalibration::begin(VarioBeeper *_varioBeeper)
{
    Wire.setClock(400000);
    vTaskDelay(delayT100);
    Wire.flush();
    Wire.begin (SDA_PIN, SCL_PIN);

    if (myIMU.begin(0x4A, Wire) == false)
    {
        Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
        while (1);
    }
    
    myIMU.calibrateAll();
    myIMU.enableGameRotationVector(100); //Send data update every 100ms
    //Enable Magnetic Field output
    myIMU.enableMagnetometer(100); //Send data update every 100ms

	varioBeeper = _varioBeeper;
	varioBeeper->setVolume(10);
	varioBeeper->generateTone(2000, 300);

	vTaskDelay(delayT100 * 30);

	/* reset variables */
	VARIO_CAL_DEBUG_PRINTLN("Begin calibration");
}

char* VarioCalibration::printAccuracyLevel(byte accuracyNumber)
{
    if (accuracyNumber == 0) {
        return "Unreliable";
    }
    else if (accuracyNumber == 1){ 
        return "Low";
    }
    else if (accuracyNumber == 2) {
        return "Medium";
    }
    else if (accuracyNumber == 3) {
        return "High";
    }
        return "";
}

void VarioCalibration::saveCalibate(){
    fc.setText1(true, "Saving...");
    myIMU.saveCalibration(); //Saves the current dynamic calibration data (DCD) to memory
	myIMU.requestCalibrationStatus(); //Sends command to get the latest calibration status

	//Wait for calibration response, timeout if no response
	int counter = 100;
	while(1)
	{
		if(--counter == 0) break;
		if(myIMU.dataAvailable() == true)
		{
			//The IMU can report many different things. We must wait
			//for the ME Calibration Response Status byte to go to zero
			if(myIMU.calibrationComplete() == true)
			{
				Serial.println("Calibration data successfully stored");
                fc.setText1(true, "Calibration saved...");
				vTaskDelay(1000);
				break;
			}
		}

		vTaskDelay(delayT50);
	}
	if(counter == 0)
	{
		Serial.println("Calibration data failed to store. Please try again.");
	}
}


void VarioCalibration::onSignalReceived(uint8_t _val)
{
    switch (_val)
    {
        case CALIBRATION_SAVE_ASKED:
            saveCalibate();
            break;
        default:
            break;
    }
}

