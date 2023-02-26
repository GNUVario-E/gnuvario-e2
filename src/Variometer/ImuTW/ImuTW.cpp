#include "Variometer/ImuTW/ImuTW.h"
#include "SparkFun_BNO080_Arduino_Library.h"
#include <MS5611.h>

MS5611 MS5611(0x77);
BNO080 myIMU;

void IMUTW::initBNO() {
    
    Wire.setClock(400000);
    delay(100);
    Wire.flush();
    Wire.begin (SDA_PIN, SCL_PIN);

    if (MS5611.begin(&Wire) == true)
    {
        Serial.println("MS5611 found.");
    }
    else
    {
        Serial.println("MS5611 not found. halt.");
        while (1);
    }

    if (myIMU.begin(0x4A, Wire) == false)
    {
        Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
        while (1);
    }
    
    Serial.println(F("Rotation vector enabled"));

    MS5611.setOversampling(OSR_ULTRA_HIGH);
    //Wire.setClock(400000);
    myIMU.enableGravity(50); //Send data update every 50ms
    myIMU.enableLinearAccelerometer(50);
    
    Serial.println("Initialized MS5611 Sensor");
}

void IMUTW::getpressure() {
     MS5611.read();           // note no error checking => "optimistic".
        Serial.print("T:\t");
        Serial.print(MS5611.getTemperature(), 2);
        Serial.print("\tP:\t");
        Serial.print(MS5611.getPressure(), 2);
        Serial.println();
        Serial.println();
}

double IMUTW::getAlti(){
    return computeAlti(MS5611.getPressure() * 100);
}

double IMUTW::getAccel() {
    
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

double IMUTW::computeAlti(double pressure, double seaLevelPressure)
{
    Serial.println("Pressure");
    Serial.println(pressure);
    return (44330.0f * (1.0f - pow((double)pressure / (double)seaLevelPressure, 0.1902949f)));
}