#pragma once

// PIN DEFINITION
#define PIN_DISPLAY_DC 17
#define PIN_DISPLAY_RST 16
#define PIN_DISPLAY_BUSY 4

#define PIN_POWER 12
#define PIN_POWER_STATE HIGH
#define PIN_VOLTAGE_DIVISOR 35

#define PIN_BUTTON_A 38
#define PIN_BUTTON_B 37
#define PIN_BUTTON_C 39

#define GPIO_BUTTON_A GPIO_NUM_38
#define GPIO_BUTTON_B GPIO_NUM_37
#define GPIO_BUTTON_C GPIO_NUM_39

#define PIN_SDCARD_CS 13
#define PIN_SDCARD_MOSI 15
#define PIN_SDCARD_MISO 2
#define PIN_SDCARD_SCK 14

#define VARIO_TW_SDA_PIN 27
#define VARIO_TW_SCL_PIN 32

#define VARIO_MPU_INT_PIN 26
#define GPIO_MPU_INT_PIN GPIO_NUM_26

#define NMEA_RX_PIN 33
#define NMEA_TX_PIN -1

/* Set the freq */
#define VARIO_TW_FREQ 400000UL

#define MPU_STATIC_ADDRESS 0x68
#define MS5611_STATIC_ADDRESS 0x77
