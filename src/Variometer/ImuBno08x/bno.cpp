#include "Variometer/ImuBno08x/bno.h"
#include "SparkFun_BNO080_Arduino_Library.h"


BNO080 myIMU;

void BNO::initBNO() {
    
    if (myIMU.begin(0x4A, Wire) == false)
    {
        Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
        while (1);
    }

    Wire.setClock(400000); //Increase I2C data rate to 400kHz

    myIMU.enableGravity(50); //Send data update every 50ms
    myIMU.enableLinearAccelerometer(50);

    Serial.println(F("Rotation vector enabled"));

}

double BNO::getAccel() {
    if (myIMU.dataAvailable() == true)
    {


        float gx = myIMU.getGravityX();
        float gy = myIMU.getGravityY();
        float gz = myIMU.getGravityZ();

        float absg = sqrt((gx * gx) + (gy * gy) + (gz * gz));

        float  ax = myIMU.getLinAccelX();
        float  ay = myIMU.getLinAccelY();
        float  az = myIMU.getLinAccelZ();

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
        return acc;
    }
    return 0;
}