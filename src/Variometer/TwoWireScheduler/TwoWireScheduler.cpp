/* TwoWireScheduler -- Interrupt driven Two Wire devices scheduler
 *
 * Copyright 2016-2019 Baptiste PELLEGRIN
 *
 * This file is part of GNUVario.
 *
 * GNUVario is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNUVario is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "TwoWireScheduler.h"

#include <Arduino.h>

#include "Variometer/IntTW/IntTW.h"
#include <esp32-hal-timer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "Variometer/ms5611TW/ms5611TW.h"
#include "varioversion.h"

#include "Variometer/LightInvensense/LightInvensense.h"
#include "Variometer/vertaccel/vertaccel.h"

#define TEMP_READ 0
#define PRESS_READ 1
#define HAVE_PRESSURE 2
#define HAVE_ACCEL 3
#define HAVE_MAG 4
#define HAVE_NEWACCEL 5
#define HAVE_GYRO 6
#define MPU_FIFO_EMPTIED 7

#define bset(bit) status |= (1 << bit)
#define bunset(bit) status &= ~(1 << bit)
#define bisset(bit) (status & (1 << bit))

TWScheduler twScheduler;

Ms5611 TWScheduler::ms5611;
Vertaccel TWScheduler::vertaccel;

/*********************/
/* static class data */
/*********************/
uint16_t volatile TWScheduler::status = 0; // no problem to not release at start as there is no values

int8_t volatile TWScheduler::ms5611Step = 0;
uint8_t volatile TWScheduler::ms5611Output[3 * 3]; // three ms5611 output measures
uint8_t volatile TWScheduler::ms5611Count = TWO_WIRE_SCHEDULER_MS5611_SHIFT;
SemaphoreHandle_t TWScheduler::ms5611Mutex;
double TWScheduler::ms5611SavePressure;

uint8_t volatile TWScheduler::checkOutput[2];
uint8_t volatile TWScheduler::imuOutput[LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH]; // imu dmp fifo output

uint8_t volatile TWScheduler::imuIntCount = 0;
SemaphoreHandle_t TWScheduler::imuIntCountMutex;

uint8_t volatile TWScheduler::imuCount = TWO_WIRE_SCHEDULER_IMU_SHIFT;
SemaphoreHandle_t TWScheduler::imuMutex;

uint8_t volatile TWScheduler::magOutput[8]; // magnetometer output
uint8_t volatile TWScheduler::magCount = TWO_WIRE_SCHEDULER_MAG_SHIFT;
SemaphoreHandle_t TWScheduler::magMutex;

TaskHandle_t TWScheduler::schedulerTaskHandler;
hw_timer_t *TWScheduler::timer;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
bool IntEnable = true;

/***************/
/* ms5611 part */
/***************/

const uint8_t ms5611Step1[] PROGMEM = {INTTW_ACTION(MS5611_STATIC_ADDRESS, INTTW_WRITE),
                                       INTTW_DEST(1, INTTW_IN_CMD),
                                       MS5611_CMD_ADC_READ,
                                       INTTW_ACTION(MS5611_STATIC_ADDRESS, INTTW_READ),
                                       INTTW_DEST(3, INTTW_AT_POINTER),
                                       INTTW_ACTION(MS5611_STATIC_ADDRESS, INTTW_WRITE),
                                       INTTW_DEST(1, INTTW_IN_CMD),
                                       MS5611_CMD_CONV_D2};

const uint8_t ms5611Step2[] PROGMEM = {INTTW_ACTION(MS5611_STATIC_ADDRESS, INTTW_WRITE),
                                       INTTW_DEST(1, INTTW_IN_CMD),
                                       MS5611_CMD_ADC_READ,
                                       INTTW_ACTION(MS5611_STATIC_ADDRESS, INTTW_READ),
                                       INTTW_DEST(3, INTTW_AT_POINTER),
                                       INTTW_ACTION(MS5611_STATIC_ADDRESS, INTTW_WRITE),
                                       INTTW_DEST(1, INTTW_IN_CMD),
                                       MS5611_CMD_CONV_D1};

void TWScheduler::ms5611Interrupt(void)
{

  if (ms5611Step == 0)
  {

    /* if PRESS_READ is not set the last I2C operation failed */
    /* so the CONV_D1 operation was not launched, relaunch */
    if (!bisset(PRESS_READ))
    {
      errorRelaunch();
      return;
    }

    bunset(TEMP_READ);
    intTW.setRxBuffer((uint8_t *)ms5611Output);
    intTW.start((uint8_t *)ms5611Step1, sizeof(ms5611Step1), INTTW_USE_PROGMEM, ms5611TempCallback);
    ms5611Step = 1;
  }
  else
  {

    /* if can't get temp, don't go further */
    if (!bisset(TEMP_READ))
    {
      errorRelaunch();
      return;
    }

    /* copy the first value to get it at any time from main loop */
    /* we need to lock */
    xSemaphoreTake(ms5611Mutex, portMAX_DELAY);
    for (int i = 0; i < 3; i++)
    {
      ms5611Output[i + 3] = ms5611Output[i];
    }

    /* get the next value */
    intTW.setRxBuffer((uint8_t *)(&ms5611Output[6]));
    intTW.start((uint8_t *)ms5611Step2, sizeof(ms5611Step2), INTTW_USE_PROGMEM, ms5611OutputCallback);
    xSemaphoreGive(ms5611Mutex);
    ms5611Step = 0;
  }
}

void TWScheduler::ms5611TempCallback(void)
{
  bset(TEMP_READ);
}

void TWScheduler::errorRelaunch(void)
{

  /* relaunch conv D1 */
  /* use PRESS_READ as success flag */
  bunset(PRESS_READ);
  intTW.start((uint8_t *)(&(ms5611Step2[5])), 3, INTTW_USE_PROGMEM, errorRelaunchCallback);
  ms5611Step = 0;
}

void TWScheduler::errorRelaunchCallback(void)
{
  /* conv D1 success */
  bset(PRESS_READ);
}

void TWScheduler::ms5611OutputCallback(void)
{
  /* done ! */
  status |= (1 << PRESS_READ) | (1 << HAVE_PRESSURE);
}

bool TWScheduler::havePressure(void)
{
  return bisset(HAVE_PRESSURE);
}

double TWScheduler::getAlti(void)
{
  /* copy needed values */
  uint8_t ms5611Values[6];

  xSemaphoreTake(ms5611Mutex, portMAX_DELAY);
  for (int i = 0; i < 6; i++)
  {
    ms5611Values[i] = ms5611Output[i + 3];
  }
  bunset(HAVE_PRESSURE);
  xSemaphoreGive(ms5611Mutex);

  /* compute pressure and temp */
  double temperature, pressure;
  ms5611.computeMeasures(&ms5611Values[0], &ms5611Values[3], temperature, pressure);

  ms5611SavePressure = pressure;

  /* get corresponding alti */
  double alti = ms5611.computeAltitude(pressure);

  //	if (alti < 0) alti = ms5611.computeAltitude2(pressure);

  if (alti < 0)
  {
    ms5611.SetBaseSeaPressure(1030);
    alti = 0;
  }

  return alti;
}

void TWScheduler::getTempAlti(double &temp, double &alti)
{
  /* copy needed values */
  uint8_t ms5611Values[6];

  xSemaphoreTake(ms5611Mutex, portMAX_DELAY);
  for (int i = 0; i < 6; i++)
  {
    ms5611Values[i] = ms5611Output[i + 3];
  }
  bunset(HAVE_PRESSURE);
  xSemaphoreGive(ms5611Mutex);

  /* compute pressure and temp */
  double temperature, pressure;
  ms5611.computeMeasures(&ms5611Values[0], &ms5611Values[3], temperature, pressure);

  ms5611SavePressure = pressure;

  /* get corresponding alti */
  alti = ms5611.computeAltitude(pressure);

  /*	if (alti < 0) alti = ms5611.computeAltitude2(pressure);

  if (alti < 0) alti = 0;*/
  temp = temperature;
}

/************/
/* IMU part */
/************/

const uint8_t imuReadFifoCount[] PROGMEM = {INTTW_ACTION(INV_HW_ADDR, INTTW_WRITE),
                                            INTTW_DEST(1, INTTW_IN_CMD),
                                            INV_REG_FIFO_COUNT_H,
                                            INTTW_ACTION(INV_HW_ADDR, INTTW_READ),
                                            INTTW_DEST(2, INTTW_AT_POINTER)};

const uint8_t imuReadFifo[] PROGMEM = {INTTW_ACTION(INV_HW_ADDR, INTTW_WRITE),
                                       INTTW_DEST(1, INTTW_IN_CMD),
                                       INV_REG_FIFO_R_W,
                                       INTTW_ACTION(INV_HW_ADDR, INTTW_READ),
                                       INTTW_DEST(LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH, INTTW_AT_POINTER)};

void TWScheduler::imuInterrupt(void)
{
  /* once the FiFo is emptied, we use interrupts */
  if (bisset(MPU_FIFO_EMPTIED))
  {
    if (imuIntCount > 0)
    {
      /* we can read the fifo directly */
      imuReadFifoData();
    }
  }
  else
  {
    /* FiFo still not emptied */
    /* check for measures     */
    intTW.setRxBuffer((uint8_t *)checkOutput);
    intTW.start((uint8_t *)imuReadFifoCount, sizeof(imuReadFifoCount), INTTW_USE_PROGMEM | INTTW_KEEP_BUS, imuCheckFifoCountCallBack);
  }
}

void TWScheduler::imuCheckFifoCountCallBack(void)
{

  /* we have FiFo count */
  uint16_t fifoCount = (((uint16_t)checkOutput[0]) << 8) | checkOutput[1];

  /* launch FiFo read if OK */
  int8_t fifoState = fastMPUHaveFIFOPaquet(fifoCount);

  /* check for empty fifo */
  if (fifoState == 0)
  {
    xSemaphoreTake(imuIntCountMutex, portMAX_DELAY);
    bset(MPU_FIFO_EMPTIED);
    imuIntCount = 0; // start using the interrupt
    xSemaphoreGive(imuIntCountMutex);
  }

  if (fifoState > 0)
  {
    imuReadFifoData();
  }
  else
  {
    /* else stop TW communication */
    intTW.stop();
  }
}

void TWScheduler::imuReadFifoData(void)
{
  /* we need to lock */
  intTW.setRxBuffer((uint8_t *)imuOutput);

  xSemaphoreTake(imuMutex, portMAX_DELAY);
  intTW.start((uint8_t *)imuReadFifo, sizeof(imuReadFifo), INTTW_USE_PROGMEM, imuHaveFifoDataCallback);
  xSemaphoreGive(imuMutex);
}

void TWScheduler::imuHaveFifoDataCallback(void)
{
  /* done ! */
  status |= (1 << HAVE_ACCEL);
  status |= (1 << HAVE_NEWACCEL);

  status |= (1 << HAVE_GYRO);

  /* decrease FiFo counter */
  xSemaphoreTake(imuIntCountMutex, portMAX_DELAY);
  imuIntCount--;
  xSemaphoreGive(imuIntCountMutex);
}

void IRAM_ATTR TWScheduler::imuIntPinInterrupt(void)
{
  BaseType_t xHigherPriorityTaskWokenT = 0;
  xSemaphoreTakeFromISR(imuIntCountMutex, &xHigherPriorityTaskWokenT);

  imuIntCount++;

  BaseType_t xHigherPriorityTaskWokenG = 0;
  xSemaphoreGiveFromISR(imuIntCountMutex, &xHigherPriorityTaskWokenG);

  if (xHigherPriorityTaskWokenT == pdTRUE || xHigherPriorityTaskWokenG == pdTRUE)
    portYIELD_FROM_ISR();
}

bool TWScheduler::haveAccel(void)
{
  return bisset(HAVE_ACCEL);
}

bool TWScheduler::haveNewAccel(void)
{
  return bisset(HAVE_NEWACCEL);
}

bool TWScheduler::resetNewAccel(void)
{
  return bunset(HAVE_NEWACCEL);
}

void TWScheduler::getRawAccel(int16_t *rawAccel, int32_t *quat)
{
  /***************/
  /* check accel */
  /***************/

  /* first copy fifo packet */
  uint8_t fifoPacket[LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH];
  xSemaphoreTake(imuMutex, portMAX_DELAY);
  for (int i = 0; i < LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH; i++)
  {
    fifoPacket[i] = imuOutput[i];
  }
  bunset(HAVE_ACCEL);
  xSemaphoreGive(imuMutex);

  /* parse FiFo packet to get raw measures */
  uint8_t tap;
  fastMPUParseFIFO(fifoPacket, NULL, rawAccel, quat, tap);

  /* check tap : use callback if needed */
  fastMPUCheckTap(tap);

  /* If the the sensor z axis pointing down         */
  /* the heading algorithm don't work !             */
  /* We need to change the basis by rotating around */
  /* y axis                                         */
  /* q_corr = q * (0 + (0, -1, 0))                  */
#if ((VARIOVERSION == 354) || (VARIOVERSION == 390) || (VARIOVERSION == 391) || (VARIOVERSION == 395) || (VARIOVERSION == 396))
  rawAccel[0] = -rawAccel[0];
  rawAccel[2] = -rawAccel[2];

  int32_t tmpQuat;
  tmpQuat = quat[0];
  quat[0] = quat[2];
  quat[2] = -tmpQuat;

  tmpQuat = quat[1];
  quat[1] = quat[3];
  quat[3] = -tmpQuat;
#endif
}

double TWScheduler::getAccel(double *vertVector)
{
  /*****************/
  /* get raw accel */
  /*****************/
  int16_t rawAccel[3];
  int32_t quat[4];

  getRawAccel(rawAccel, quat);

  /* compute vertVector and vertAccel */
  double vertAccel;
  if (vertVector)
  {
    vertaccel.compute(rawAccel, quat, vertVector, vertAccel);
  }
  else
  {
    double tmpVertVector[3];
    vertaccel.compute(rawAccel, quat, tmpVertVector, vertAccel);
  }

  /* done */
  return vertAccel;
}

bool TWScheduler::haveGyro(void)
{
  return bisset(HAVE_GYRO);
}

void TWScheduler::getRawGyro(int16_t *rawGyro, int32_t *quat)
{
  /***************/
  /* check gyro */
  /***************/

  /* first copy fifo packet */
  uint8_t fifoPacket[LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH];
  xSemaphoreTake(imuMutex, portMAX_DELAY);
  for (int i = 0; i < LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH; i++)
  {
    fifoPacket[i] = imuOutput[i];
  }
  bunset(HAVE_GYRO);
  xSemaphoreGive(imuMutex);

  /* parse FiFo packet to get raw measures */
  uint8_t tap;
  fastMPUParseFIFO(fifoPacket, rawGyro, NULL, quat, tap);

  /* check tap : use callback if needed */
  fastMPUCheckTap(tap);
}

void TWScheduler::getRawAccelGyro(int16_t *rawAccel, int16_t *rawGyro, int32_t *quat)
{
  /***************/
  /* check accel */
  /***************/

  /* first copy fifo packet */
  uint8_t fifoPacket[LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH];
  xSemaphoreTake(imuMutex, portMAX_DELAY);
  for (int i = 0; i < LIGHT_INVENSENSE_COMPRESSED_DMP_PAQUET_LENGTH; i++)
  {
    fifoPacket[i] = imuOutput[i];
  }
  bunset(HAVE_ACCEL);
  xSemaphoreGive(imuMutex);

  /* parse FiFo packet to get raw measures */
  uint8_t tap;
  fastMPUParseFIFO(fifoPacket, rawGyro, rawAccel, quat, tap);

  /* check tap : use callback if needed */
  fastMPUCheckTap(tap);
}

void TWScheduler::getAccelGyro(double *vertVector, double *gyroVector)
{
  /*****************/
  /* get raw accel */
  /*****************/
  int16_t rawAccel[3];
  int32_t quat[4];

  getRawAccel(rawAccel, quat);

  /* compute vertVector and vertAccel */
  double vertAccel;
  if (vertVector)
  {
    vertaccel.compute(rawAccel, quat, vertVector, vertAccel);
  }
  else
  {
    double tmpVertVector[3];
    vertaccel.compute(rawAccel, quat, tmpVertVector, vertAccel);
  }

  int16_t rawGyro[3];
  getRawGyro(rawGyro, quat);

  /* compute vertVector and vertAccel */
  double vertGyro;
  if (gyroVector)
  {
    vertaccel.computeGyro(rawGyro, quat, gyroVector, vertGyro);
  }
  else
  {
    double tmpGyroVector[3];
    vertaccel.computeGyro(rawGyro, quat, tmpGyroVector, vertGyro);
  }
}

/************/
/* Mag part */
/************/

const uint8_t magReadStatus[] PROGMEM = {INTTW_ACTION(INV_HW_ADDR, INTTW_WRITE),
                                         INTTW_DEST(1, INTTW_IN_CMD),
                                         INV_REG_I2C_MST_STATUS,
                                         INTTW_ACTION(INV_HW_ADDR, INTTW_READ),
                                         INTTW_DEST(1, INTTW_AT_POINTER)};

const uint8_t magReadData[] PROGMEM = {INTTW_ACTION(INV_HW_ADDR, INTTW_WRITE),
                                       INTTW_DEST(1, INTTW_IN_CMD),
                                       INV_REG_RAW_COMPASS,
                                       INTTW_ACTION(INV_HW_ADDR, INTTW_READ),
                                       INTTW_DEST(8, INTTW_AT_POINTER)};

void TWScheduler::magInterrupt(void)
{
  /* check for available measures */
  intTW.setRxBuffer((uint8_t *)checkOutput);
  intTW.start((uint8_t *)magReadStatus, sizeof(magReadStatus), INTTW_USE_PROGMEM | INTTW_KEEP_BUS, magCheckStatusCallback);
}

void TWScheduler::magCheckStatusCallback(void)
{
  /* check if new measure */
  if (checkOutput[0] & 0x40)
  {

    /* read measure */
    /* we need to lock */
    intTW.setRxBuffer((uint8_t *)magOutput);
    xSemaphoreTake(magMutex, portMAX_DELAY);
    intTW.start((uint8_t *)magReadData, sizeof(magReadData), INTTW_USE_PROGMEM, magHaveDataCallback);
    xSemaphoreGive(magMutex);
  }
  else
  {
    /* stop TW communication */
    intTW.stop();
  }
}

void TWScheduler::magHaveDataCallback(void)
{
  /* done ! */
  status |= (1 << HAVE_MAG);
}

bool TWScheduler::haveMag(void)
{
  return bisset(HAVE_MAG);
}

void TWScheduler::getRawMag(int16_t *rawMag)
{
  /*************/
  /* check mag */
  /*************/
  if (bisset(HAVE_MAG))
  {

    /* copy mag data */
    uint8_t magData[8];
    xSemaphoreTake(magMutex, portMAX_DELAY);
    for (int i = 0; i < 8; i++)
    {
      magData[i] = magOutput[i];
    }
    bunset(HAVE_MAG);
    xSemaphoreGive(magMutex);

    /* parse mag data */
#ifdef VERTACCEL_USE_MAG_SENS_ADJ
    if (fastMPUParseMag(magData, rawMag) >= 0)
    {
#else
    if (fastMPUParseRawMag(magData, rawMag) >= 0)
    {
#endif
    }
  }

  /* If the the sensor z axis pointing down         */
  /* the heading algorithm don't work !             */
  /* We need to change the basis by rotating around */
  /* y axis                                         */
#if ((VARIOVERSION == 354) || (VARIOVERSION == 390) || (VARIOVERSION == 391) || (VARIOVERSION == 395) || (VARIOVERSION == 396))
  rawMag[0] = -rawMag[0];
  rawMag[2] = -rawMag[2];
#endif
}

void TWScheduler::getNorthVector(double *vertVector, double *northVector)
{
  /* get raw mag */
  int16_t rawMag[3];
  getRawMag(rawMag);

  /* compute north vector */
  vertaccel.computeNorthVector(vertVector, rawMag, northVector);

  /* apply sensor orientation */
#if ((VARIOVERSION == 254) || (VARIOVERSION == 291) || (VARIOVERSION == 293) || (VARIOVERSION == 294))
  northVector[0] = -northVector[0];
  northVector[1] = -northVector[1];
#elif ((VARIOVERSION == 290) || (VARIOVERSION == 292))
  double tmp;
  tmp = northVector[0];
  northVector[0] = northVector[1];
  northVector[1] = -tmp;
#endif
}

void TWScheduler::getNorthVector2(double *vertVector, double *gyroVector, double *northVector)
{
  /* get raw mag */
  int16_t rawMag[3];
  getRawMag(rawMag);

  /* compute north vector */
  vertaccel.computeNorthVector2(vertVector, gyroVector, rawMag, northVector);
}

/*---------------------*/
/*                     */
/*      Scheduler      */
/*                     */
/*---------------------*/
void TWScheduler::init(void)
{
  /* init the devices */
  /* and mutexes      */

  ms5611.init();
  ms5611Mutex = xSemaphoreCreateBinary();
  xSemaphoreGive(ms5611Mutex);

  vertaccel.init();
  imuMutex = xSemaphoreCreateBinary();
  xSemaphoreGive(imuMutex);

  magMutex = xSemaphoreCreateBinary();
  xSemaphoreGive(magMutex);

  /* init INT pin */
  imuIntCountMutex = xSemaphoreCreateBinary();
  xSemaphoreGive(imuIntCountMutex);
  pinMode(VARIO_MPU_INT_PIN, INPUT);
  attachInterrupt(VARIO_MPU_INT_PIN, imuIntPinInterrupt, FALLING);

  /* create scheduler task */
  VARIO_PROG_DEBUG_PRINTLN("Task two wire scheduler started");
  xTaskCreatePinnedToCore(interruptScheduler, "TWS", TWO_WIRE_SCHEDULER_STACK_SIZE, NULL, TWO_WIRE_SCHEDULER_PRIORITY, &schedulerTaskHandler, TWO_WIRE_SCHEDULER_CORE);

  /* create and launch timer */
  timer = timerBegin(TWO_WIRE_SCHEDULER_TIMER_NUM, TWO_WIRE_SCHEDULER_INTERRUPT_PRESCALE, true);
  timerAttachInterrupt(timer, timerCallback, true);
  timerAlarmWrite(timer, TWO_WIRE_SCHEDULER_INTERRUPT_COMPARE, true);
  timerAlarmEnable(timer);
}

void TWScheduler::disableAcquisition()
{
  IntEnable = false;

  vTaskSuspend(schedulerTaskHandler);
  timerAlarmDisable(timer);

  detachInterrupt(VARIO_MPU_INT_PIN);

  intTW.release();

#ifdef VARIO_TW_SDA_PIN
  detachInterrupt(VARIO_TW_SDA_PIN);
#endif

#ifdef VARIO_TW_SCL_PIN
  detachInterrupt(VARIO_TW_SCL_PIN);
#endif
}

void TWScheduler::interruptScheduler(void *param)
{
  while (true)
  {
    /* wait */
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    /* launch interrupt */
    mainInterrupt();
  }
}

void IRAM_ATTR TWScheduler::timerCallback(void)
{
  /* just wake scheduler */
  BaseType_t xHigherPriorityTaskWoken = 0;
  xTaskNotifyFromISR(schedulerTaskHandler, 0, eNoAction, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken == pdTRUE)
    portYIELD_FROM_ISR();
}

void TWScheduler::mainInterrupt(void)
{
  /* launch interrupts */
  if (ms5611Count == 0)
  {
    ms5611Interrupt();
    ms5611Count = TWO_WIRE_SCHEDULER_MS5611_PERIOD;
  }

  if (imuCount == 0)
  {
    imuInterrupt();
    imuCount = TWO_WIRE_SCHEDULER_IMU_PERIOD;
  }

  if (magCount == 0)
  {
    magInterrupt();
    magCount = TWO_WIRE_SCHEDULER_MAG_PERIOD;
  }

  /* decrease counters */
  ms5611Count--;

  imuCount--;
  magCount--;
}
