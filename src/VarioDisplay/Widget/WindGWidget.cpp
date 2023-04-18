#include "WindGWidget.h"
#include <Arduino.h>
#include "VarioWind/_vector.h"
#include "VarioTool/VarioTool.h"

#define MPS 3.6

bool WindGWidget::isRefreshNeeded(uint32_t lastDisplayTime)
{
    char bearingTxt[3];

    if (fc.getWindTimestamp() > getTimeout())
    {
        if ((fc.getWindSpeed() != oldSpeed) || (fc.getWindHeading() != oldHeading) || (fc.getWindMeanSpeed() != oldMeanSpeed))
        {
            VarioTool::bearingToOrdinal2c(bearingTxt, fc.getWindHeading());

            oldSpeed = fc.getWindSpeed();
            oldHeading = fc.getWindHeading();
            oldMeanSpeed = fc.getWindMeanSpeed();

            return true;
        }
    }

    return false;
}

void WindGWidget::addToBuffer(GxEPD2_GFX &_display)
{

    int16_t tbx, tby;
    uint16_t tbw, tbh;
    char speed[3];

    // clean zone
    _display.fillRect(topx, topy, width, height, GxEPD_WHITE);

    uint16_t angleWind = (270 + oldHeading) % 360;
    Serial.print("heading angle: ");
    Serial.println(fc.getWindHeading());
    Serial.print("Wind angle: ");
    Serial.println(angleWind);

    uint16_t centerX = width / 2 + topx;            // Get the X coordinate of the center of the widget
    uint16_t centerY = height / 2 + topy;           // Get the Y coordinate of the center of the widget
    uint16_t radius = (min(width, height) / 2) - 2; // Compute the radius of the compass (make sure it fits on the widget)

    // Draw the compass circle
    _display.drawCircle(centerX, centerY, radius, GxEPD_BLACK);

    // Draw the small triangles pointing to the center every 45 degrees with direction text
    String directions[8] = {"E", "SE", "S", "SW", "W", "NW", "N", "NE"};
    for (int i = 0; i < 8; i++)
    {
        int angle = i * 45;
        int x1 = centerX + (radius + 5) * cos(angle * PI / 180);
        int y1 = centerY + (radius + 5) * sin(angle * PI / 180);
        int x2 = centerX + (radius - 3) * cos((angle - 5) * PI / 180);
        int y2 = centerY + (radius - 3) * sin((angle - 5) * PI / 180);
        int x3 = centerX + (radius - 3) * cos((angle + 5) * PI / 180);
        int y3 = centerY + (radius - 3) * sin((angle + 5) * PI / 180);
        _display.fillTriangle(x1, y1, x2, y2, x3, y3, GxEPD_BLACK);
        int textX = centerX + (radius - 15) * cos(angle * PI / 180) - 5;
        int textY = centerY + (radius - 15) * sin(angle * PI / 180) + 5;
        _display.setFont(&NotoSans6pt7b);
        _display.setTextSize(1);
        _display.setCursor(textX, textY);
        _display.print(directions[i]);

        int smallTriX = centerX + (radius)*cos((angle + 22.5) * PI / 180);
        int smallTriY = centerY + (radius)*sin((angle + 22.5) * PI / 180);
        int smallTriSize = 2;
        _display.fillCircle(smallTriX, smallTriY, smallTriSize, GxEPD_BLACK);
    }

    // Draw arrow from circle at 128° pointing towards center
    int arrowAngle = angleWind;
    int arrowLength = radius * 1 / 3; // Length of arrow line
    int circleX = centerX + (radius)*cos(arrowAngle * PI / 180);
    int circleY = centerY + (radius)*sin(arrowAngle * PI / 180);
    int arrowTipX = centerX + arrowLength * cos(arrowAngle * PI / 180);
    int arrowTipY = centerY + arrowLength * sin(arrowAngle * PI / 180);

    int arrowTipBigLineX = centerX + (arrowLength + 5) * cos(arrowAngle * PI / 180);
    int arrowTipBigLineY = centerY + (arrowLength + 5) * sin(arrowAngle * PI / 180);

    int arrowHeadSize = 15;                                               // Size of arrow head
    int arrowBaseAngle = 25;                                              // Angle of arrow head base
    int arrowBaseLength = arrowHeadSize * sin(arrowBaseAngle * PI / 180); // Length of arrow head base

    int arrowHeadAngle1 = (arrowAngle + arrowBaseAngle + 180) % 360;
    int arrowHeadAngle2 = (arrowAngle - arrowBaseAngle + 180) % 360;

    int arrowHeadX1 = arrowTipX - arrowHeadSize * cos(arrowHeadAngle1 * PI / 180);
    int arrowHeadY1 = arrowTipY - arrowHeadSize * sin(arrowHeadAngle1 * PI / 180);

    int arrowHeadX2 = arrowTipX - arrowHeadSize * cos(arrowHeadAngle2 * PI / 180);
    int arrowHeadY2 = arrowTipY - arrowHeadSize * sin(arrowHeadAngle2 * PI / 180);

    drawThickLine(_display, circleX, circleY, arrowTipBigLineX, arrowTipBigLineY, 4, GxEPD_BLACK);
    _display.fillTriangle(arrowTipX, arrowTipY, arrowHeadX1, arrowHeadY1, arrowHeadX2, arrowHeadY2, GxEPD_BLACK);

    _display.setFont(&Gnuvarioe18pt7b);
    _display.setTextSize(1);

    sprintf(speed, "%d", oldMeanSpeed);
    _display.getTextBounds(speed, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    _display.setCursor(centerX - tbw / 2, centerY + tbh / 2);
    _display.print(speed);
}

void WindGWidget::drawThickLine(GxEPD2_GFX &_display, int x1, int y1, int x2, int y2, int thickness, uint16_t color)
{
    for (int i = -thickness / 2; i <= thickness / 2; i++)
    {
        for (int j = -thickness / 2; j <= thickness / 2; j++)
        {
            _display.drawLine(x1 + i, y1 + j, x2 + i, y2 + j, color);
        }
    }
}

/* timeout of 15s because timer that calulate wind run every 10s */
uint32_t WindGWidget::getTimeout()
{
    return millis() - 15000;
}