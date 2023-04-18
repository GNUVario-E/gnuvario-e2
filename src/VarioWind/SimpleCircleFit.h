#pragma once

#include <Arduino.h>
#include "VarioTool/VarioTool.h"
#include "_vector.h"
#include "FC.h"

#define NB_VECTORS 150 // nb points for wind calculation

typedef struct
{
    double a;
    double b;
    double r;
    double s;
} Circle;

/**
 * @brief Structure de données pour stocker un point GPS
 * latitude et longitude sont stockées en degrés DDMMmmm (IGC)
 */
typedef struct
{
    int32_t latitude;
    int32_t longitude;
    long timestamp;
} IGCPoint;

/**
 * @brief Structure de données pour stocker un point GPS
 * latitude et longitude sont stockées en degrés décimaux
 */
typedef struct
{
    double latitude;
    double longitude;
    long timestamp;
} GPSPoint;

class SimpleCircleFit
{
private:
    const TickType_t delayT10 = pdMS_TO_TICKS(10);

    uint8_t filter;

    Vector *vectors;
    uint16_t indexVector = 0;
    uint16_t bufSizeVector = 0;

    uint8_t nbPointsGps = 0;
    GPSPoint *pointsGps;
    uint16_t indexGps = 0;
    uint8_t bufSizeGps = 0;

    void calculateNewVector();
    bool linearSolve2x2(double matrix[4], double vector[2], double &x, double &y);
    Vector getMeanPointFromVectors(uint16_t start, uint16_t end, uint16_t nbVectors);
    Circle computeFromVectors();

public:
    SimpleCircleFit();
    void addPoint(double lat, double lon, uint32_t timestamp);

    Circle fitCircleFromVectors();
};

extern FC fc;