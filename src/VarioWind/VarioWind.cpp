#include "VarioWind.h"

#define IS_FROM_TEST
#if defined(IS_FROM_TEST)
#include "windTestData.h"
static uint16_t indTest = 0;
static uint16_t nbPtsTest = sizeof(igcPoints) / sizeof(IGCPoint);
#endif

VarioWind::VarioWind()
{
    circleFit = new SimpleCircleFit();
}

void VarioWind::init()
{
    TimerHandle_t timerHndl10Sec;
    timerHndl10Sec = xTimerCreate(
        "timer10Sec",         /* name */
        pdMS_TO_TICKS(10000), /* period/time */
        pdTRUE,               /* auto reload */
        (void *)this,         /* timer ID */
        startTimers10sImpl);  /* callback */

    if (xTimerStart(timerHndl10Sec, 0) != pdPASS)
    {
        for (;;)
            ; /* failure!?! */
    }
}

void VarioWind::startTimers10sImpl(TimerHandle_t timerHndlWind)
{
    // wrapper for timer
    VarioWind *obj;
    obj = (VarioWind *)pvTimerGetTimerID(timerHndlWind);
    // pvTimerGetTimerID(timerHndl10Sec);
    obj->timerWind();
}

void VarioWind::timerWind()
{
    Circle circle = circleFit->fitCircleFromVectors();

    float windSpeed;
    uint16_t heading;
    float meanSpeed;
    windSpeed = (float)sqrt(circle.a * circle.a + circle.b * circle.b) * 3.6;
    heading = (int)(270 + degrees(-atan2(circle.b, circle.a))) % 360;
    meanSpeed = (float)circle.r * 3.6;
    // // heading = (int)(270 - degrees(atan2(sin(-circle.a) * cos(0), cos(circle.b) * sin(0) - sin(circle.b) * cos(0) * cos(-circle.a))) + 360) % 360;

    if (circle.s == 99.0)
    {
        // no data
        return;
    }
    fc.setWind(windSpeed, heading, meanSpeed, millis());

    Serial.print("windSpeed:");
    Serial.print(windSpeed);
    Serial.print("km/h, heading:");
    Serial.print(heading);
    Serial.print("°, meanSpeed:");
    Serial.print(meanSpeed);
    Serial.println("km/h");
}

void VarioWind::addPoint(double lat, double lon, uint32_t timestamp)
{

#if defined IS_FROM_TEST

    lat = VarioTool::convertIGCDDMMmmmToDecimal(igcPoints[indTest].latitude);
    lon = VarioTool::convertIGCDDMMmmmToDecimal(igcPoints[indTest].longitude);
    timestamp = igcPoints[indTest].timestamp * 1000;
    indTest = (indTest + 1) % nbPtsTest;
    // Serial.print("latIGC:");
    // Serial.print(lat);
    // Serial.print(", lonIGC:");
    // Serial.print(lon);
    // Serial.print(", timestamp:");
    // Serial.println(timestamp);
#endif

    circleFit->addPoint(lat, lon, timestamp);
}

VarioWind::~VarioWind()
{
}

void VarioWind::calculateWind(IGCPoint *points, int numPoints, float &windSpeed, int &heading, float &meanSpeed)
{
    // Circle circle = circleFit->fitCircle(points, numPoints);

    // // Serial.println("");
    // // Serial.println("");
    // Serial.print("x:");
    // Serial.print(circle.a, 8);
    // Serial.print(", y:");
    // Serial.print(circle.b, 8);
    // Serial.print(", r:");
    // Serial.print(circle.r, 8);
    // // Serial.print(", v (km/h):");
    // // Serial.print(sqrt(circle.a * circle.a + circle.b * circle.b) * 3.6, 2);
    // // Serial.print(", angle:");
    // // Serial.print(degrees(atan2(sin(-circle.a) * cos(0), cos(circle.b) * sin(0) - sin(circle.b) * cos(0) * cos(-circle.a))));

    // windSpeed = (float)sqrt(circle.a * circle.a + circle.b * circle.b) * 3.6;
    // heading = (int)(270 + degrees(-atan2(circle.b, circle.a))) % 360;
    // meanSpeed = (float)circle.r * 3.6;
    // // heading = (int)(270 - degrees(atan2(sin(-circle.a) * cos(0), cos(circle.b) * sin(0) - sin(circle.b) * cos(0) * cos(-circle.a))) + 360) % 360;
}
