/*
  Using the BNO080 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the i/j/k/real parts of the rotation vector.
  https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO080 breakout to avoid polling.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>
#include "MerweScaledSigmaPoints.hpp"
#include "Ukf.hpp"

#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO080 myIMU;
#define SDA_PIN 27
#define SCL_PIN 32
#define POWER_PIN 12
#define POWER_PIN_STATE HIGH 

void setup()
{

 
  pinMode(POWER_PIN, OUTPUT); 
  digitalWrite(POWER_PIN, POWER_PIN_STATE);     // turn on POWER

  
  Serial.begin(115200);
  delay(5000);
  Serial.println();
  Serial.println("BNO080 Read Example");
Wire.setClock(400000);
delay(100);
  Wire.flush();
  Wire.begin (SDA_PIN, SCL_PIN);
  //=================================

  if (myIMU.begin(0x4A, Wire) == false)
  {
    Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  myIMU.enableGravity(50); //Send data update every 50ms
  myIMU.enableAccelerometer(50);

  Serial.println(F("Rotation vector enabled"));
  Serial.println(F("Output in form i, j, k, real, accuracy"));
}

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {


  float gx = myIMU.getGravityX();
  float gy = myIMU.getGravityY();
  float gz = myIMU.getGravityZ();

  float absg = sqrt((gx * gx) + (gy * gy) + (gz * gz));

  float  ax = myIMU.getAccelX();
  float  ay = myIMU.getAccelY();
  float  az = myIMU.getAccelZ();

  double acc = 0.0;

  if ((9.0 < absg) && (absg < 11.0)) {
    float skag = (gx * ax) + (gy * ay) + (gz * az);
    acc = skag / absg;
  } else {
    // Sensor error, the gravity vector is reported to be invalid
    acc = 0;
  }

    Serial.print(acc, 2);
    
    Serial.println();
  }
}
