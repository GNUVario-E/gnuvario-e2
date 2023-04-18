#include "CircleWidget.h"
#include <Arduino.h>
#include "VarioWind/_vector.h"
#define MPS 3.6

bool CircleWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    return true;

    return TextWidget::isRefreshNeeded(lastDisplayTime);
}

void CircleWidget::addToBuffer(GxEPD2_GFX &_display)
{
    Vector *vectors;
    _display.fillRect(topx, topy, width, height, GxEPD_WHITE);

    int rMax = 64;    // rayon max en pt
    int maxValue = 0; // max value in km/h

    int16_t w2 = width / 2;
    int16_t h2 = height / 2;

    uint16_t nb = fc.getWindVectorsCount();

    // axis
    _display.drawLine(topx + w2, topy, topx + w2, topy + height, GxEPD_BLACK);
    _display.drawLine(topx, topy + h2, topx + width, topy + h2, GxEPD_BLACK);

    if (nb < 1)
    {
        return;
    }

    vectors = fc.getWindVectors();

    // first loop to get max value
    for (int i = 0; i < nb; i++)
    {
        maxValue = max(maxValue, (int)(abs(vectors[i].x) * MPS));
        maxValue = max(maxValue, (int)(abs(vectors[i].y) * MPS));
    }
    if (maxValue < 10)
    {
        maxValue = 11;
    }

    float coef = (float)rMax / maxValue;

    for (int i = 0; i < nb; i++)
    {
        int x = vectors[i].x * MPS * coef;
        int y = vectors[i].y * MPS * coef;

        // Serial.print("coeff=");
        // Serial.print(coef);
        // Serial.print(" x=");
        // Serial.print(vectors[i].x);
        // Serial.print(" y=");
        // Serial.println(vectors[i].y);
        _display.drawCircle(topx + w2 + x, topy + h2 - y, 2, GxEPD_BLACK);
    }

    if (fc.getWindSpeed() && fc.getWindHeading())
    {
        uint16_t angle = (270 - fc.getWindHeading()) % 360;
        Serial.print("angle=");
        Serial.println(angle);
        int x2 = rMax - fc.getWindSpeed() * cos(radians(angle)) * coef;
        int y2 = rMax + fc.getWindSpeed() * sin(radians(angle) * coef);
        // Serial.print("x2=");
        // Serial.print(x2);
        // Serial.print(" y2=");
        // Serial.println(y2);

        drawThickLine(_display, topx + rMax, topy + rMax, topx + x2, topy + y2, 2, GxEPD_BLACK);
    }

    // origine
    _display.drawCircle(topx + w2, topy + h2, 1, GxEPD_BLACK);

    // cercle 10km/h
    _display.drawCircle(topx + w2, topy + h2, (int16_t)10 * coef, GxEPD_BLACK);

    if (maxValue > 20)
    {
        // cercle 20km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)20 * coef, GxEPD_BLACK);
    }

    if (maxValue > 30)
    {
        // cercle 30km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)30 * coef, GxEPD_BLACK);
    }

    if (maxValue > 40)
    {
        // cercle 40km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)40 * coef, GxEPD_BLACK);
    }
    if (maxValue > 50)
    {
        // cercle 50km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)50 * coef, GxEPD_BLACK);
    }

    if (maxValue > 60)
    {
        // cercle 60km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)60 * coef, GxEPD_BLACK);
    }

    if (maxValue > 70)
    {
        // cercle 70km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)70 * coef, GxEPD_BLACK);
    }

    if (maxValue > 80)
    {
        // cercle 80km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)80 * coef, GxEPD_BLACK);
    }

    if (maxValue > 90)
    {
        // cercle 90km/h
        _display.drawCircle(topx + w2, topy + h2, (int16_t)90 * coef, GxEPD_BLACK);
    }
}

void CircleWidget::drawThickLine(GxEPD2_GFX &_display, int x1, int y1, int x2, int y2, int thickness, uint16_t color)
{
    for (int i = -thickness / 2; i <= thickness / 2; i++)
    {
        for (int j = -thickness / 2; j <= thickness / 2; j++)
        {
            _display.drawLine(x1 + i, y1 + j, x2 + i, y2 + j, color);
        }
    }
}