#pragma once
#include "ConfigParameter.h"
#include "ParamDefiniton.h"

class Parameters
{
private:
    static Parameters *instance;
    Parameters();

public:
    static Parameters *getInstance();

    void initDefault();

    /* systeme */
    ConfigParameter<bool> *P_BT_ENABLE;
    ConfigParameter<bool> *P_NO_RECORD;
    ConfigParameter<bool> *P_BEEP_GPSFIX;
    ConfigParameter<bool> *P_BEEP_FLYBEGIN;
    ConfigParameter<bool> *P_BEEP_VARIOBEGIN;
    ConfigParameter<bool> *P_MUTE_VARIOBEGIN;
    ConfigParameter<uint8_t> *P_SLEEP_TIMEOUT_MINUTES;
    ConfigParameter<float> *P_SLEEP_THRESHOLD_CPS;
    ConfigParameter<uint16_t> *P_MULTIDISPLAY_DURATION;
    ConfigParameter<uint8_t> *P_DISPLAY_STAT_DURATION;
    ConfigParameter<const char *> *P_URL_UPDATE;
    ConfigParameter<uint8_t> *P_LANGUAGE;

    /* Général */
    ConfigParameter<const char *> *P_PILOT_NAME;
    ConfigParameter<uint8_t> *P_GLIDER_SELECT;
    ConfigParameter<const char *> *P_GLIDER_NAME1;
    ConfigParameter<const char *> *P_GLIDER_NAME2;
    ConfigParameter<const char *> *P_GLIDER_NAME3;
    ConfigParameter<const char *> *P_GLIDER_NAME4;
    ConfigParameter<int8_t> *P_TIME_ZONE;

    /* Vario */
    ConfigParameter<float> *P_ACCELERATION_MEASURE_STANDARD_DEVIATION;

    /* Flight start */
    ConfigParameter<float> *P_FLIGHT_START_VARIO_LOW_THRESHOLD;
    ConfigParameter<float> *P_FLIGHT_START_VARIO_HIGH_THRESHOLD;
    ConfigParameter<float> *P_FLIGHT_START_MIN_SPEED;
    ConfigParameter<bool> *P_RECORD_WHEN_FLIGHT_START;

    /* wifi */
    ConfigParameter<const char *> *P_SSID_1;
    ConfigParameter<const char *> *P_PASSWORD_1;
    ConfigParameter<const char *> *P_SSID_2;
    ConfigParameter<const char *> *P_PASSWORD_2;
    ConfigParameter<const char *> *P_SSID_3;
    ConfigParameter<const char *> *P_PASSWORD_3;
    ConfigParameter<const char *> *P_SSID_4;
    ConfigParameter<const char *> *P_PASSWORD_4;

    /* variocal */
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_00;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_01;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_02;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_03;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_04;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_05;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_06;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_07;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_08;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_09;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_10;
    ConfigParameter<uint8_t> *P_VERTACCEL_GYRO_CAL_BIAS_11;
    ConfigParameter<int16_t> *P_VERTACCEL_ACCEL_CAL_BIAS_00;
    ConfigParameter<int16_t> *P_VERTACCEL_ACCEL_CAL_BIAS_01;
    ConfigParameter<int16_t> *P_VERTACCEL_ACCEL_CAL_BIAS_02;
    ConfigParameter<int16_t> *P_VERTACCEL_ACCEL_CAL_SCALE;
    ConfigParameter<int16_t> *P_VERTACCEL_MAG_CAL_BIAS_00;
    ConfigParameter<int16_t> *P_VERTACCEL_MAG_CAL_BIAS_01;
    ConfigParameter<int16_t> *P_VERTACCEL_MAG_CAL_BIAS_02;
    ConfigParameter<int16_t> *P_VERTACCEL_MAG_CAL_PROJ_SCALE;
    ConfigParameter<uint16_t> *P_VERTACCEL_ACCEL_CAL_BIAS_MULTIPLIER;
    ConfigParameter<uint16_t> *P_VERTACCEL_MAG_CAL_BIAS_MULTIPLIER;
};
