#include "SimpleCircleFit.h"

SimpleCircleFit::SimpleCircleFit()
{
    // must be positive
    // space between points to calculate speed and heading (1 = successive points)
    filter = 1;

    // pour calculer les vecteurs, on a besoin de 2 points, espacés de "filter" points
    nbPointsGps = filter + 2;
    pointsGps = (GPSPoint *)malloc((nbPointsGps) * sizeof(GPSPoint));

    // Allocate an array of vectors
    vectors = (Vector *)malloc(NB_VECTORS * sizeof(Vector));
    if (vectors == NULL)
    {
        Serial.println("malloc failed");
    }
    else
    {
        Serial.println("malloc succeeded");
    }
}

void SimpleCircleFit::addPoint(double lat, double lon, uint32_t timestamp)
{
    // we have at least 1 point to compare with
    if (bufSizeGps > 0)
    {
        uint8_t previousIndexGps = (indexGps - 1) % nbPointsGps;
        if (pointsGps[previousIndexGps].latitude == lat && pointsGps[previousIndexGps].longitude == lon)
        {
            // same coordinate, we skip
            return;
        }
    }

    pointsGps[indexGps].latitude = lat;
    pointsGps[indexGps].longitude = lon;
    pointsGps[indexGps].timestamp = timestamp;

    if (bufSizeGps < nbPointsGps)
    {
        // not enough points to calculate a vector
        bufSizeGps++;
    }
    else
    {
        calculateNewVector();
    }

    indexGps = (indexGps + 1) % (nbPointsGps);
}

void SimpleCircleFit::calculateNewVector()
{

    uint8_t previousIndexGps = (nbPointsGps + indexGps - filter) % nbPointsGps;
    // Serial.print("nbPointsGps: ");
    // Serial.println(nbPointsGps);
    // Serial.print("indexGps: ");
    // Serial.println(indexGps);
    // Serial.print("previousIndexGps: ");
    // Serial.println(previousIndexGps);

    double lat1, lon1, lat2, lon2;
    lat1 = pointsGps[previousIndexGps].latitude;
    lon1 = pointsGps[previousIndexGps].longitude;
    lat2 = pointsGps[indexGps].latitude;
    lon2 = pointsGps[indexGps].longitude;

    // Serial.print("lat1: ");
    // Serial.print(lat1, 8);
    // Serial.print(", lon1: ");
    // Serial.print(lon1, 8);
    // Serial.print(", lat2: ");
    // Serial.print(lat2, 8);
    // Serial.print(", lon2: ");
    // Serial.println(lon2, 8);

    lat1 = radians(lat1);
    lon1 = radians(lon1);
    lat2 = radians(lat2);
    lon2 = radians(lon2);

    // Calculate distance and time between points
    double deltaLat = lat2 - lat1;
    double deltaLon = lon2 - lon1;
    long deltaTime = pointsGps[indexGps].timestamp - pointsGps[previousIndexGps].timestamp;

    // Serial.print("deltaTime: ");
    // Serial.println(deltaTime);

    double distance = VarioTool::calculateDistance(lat1, lon1, lat2, lon2); // en m
    double time = deltaTime / 1000.0;                                       // / 1000.0; // Convert timestamp to seconds

    // Serial.print("distance: ");
    // Serial.println(distance, 10);

    // Calculate speed
    double speed = distance / time; // en m/s
    double heading = atan2(sin(lon2 - lon1) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1));

    // Serial.print("speed: ");
    // Serial.println(speed, 10);
    // Serial.print("heading: ");
    // Serial.println(heading, 10);
    // Serial.print("indexVector: ");
    // Serial.println(indexVector);

    vectors[indexVector].x = sin(heading) * speed;
    vectors[indexVector].y = cos(heading) * speed;

    // Serial.print("vector ");
    // Serial.print(indexVector);
    // Serial.print(" : ");
    // Serial.print(vectors[indexVector].x, 10);
    // Serial.print(", ");
    // Serial.println(vectors[indexVector].y, 10);

    //  Serial.print("Wind speed: ");
    //  Serial.print(windSpeed);
    //  Serial.print(" m/s, Wind direction: ");
    // Serial.print(windDirection);
    // Serial.println(" deg");
    if (bufSizeVector < NB_VECTORS)
    {
        bufSizeVector++;
    }
    indexVector = (indexVector + 1) % NB_VECTORS;

    // set pointer to vector in FC
    Serial.print("setWindVectors");
    Serial.println(bufSizeVector);
    fc.setWindVectors(vectors, bufSizeVector);
}

bool SimpleCircleFit::linearSolve2x2(double matrix[4], double vector[2], double &x, double &y)
{
    double det = matrix[0] * matrix[3] - matrix[1] * matrix[2];
    if (det < 1e-8)
        return false; // no solution
    y = (matrix[0] * vector[1] - matrix[2] * vector[0]) / det;
    x = (vector[0] - matrix[1] * y) / matrix[0];

    return true; // solution found
}

Vector SimpleCircleFit::getMeanPointFromVectors(uint16_t start, uint16_t end, uint16_t nbVectors)
{

    Vector mean = {0, 0};

    for (int i = start; i < start + nbVectors; i++)
    {
        int index = i % NB_VECTORS;
        mean.x += vectors[index].x / nbVectors;
        mean.y += vectors[index].y / nbVectors;
    }

    return mean;
}

Circle SimpleCircleFit::computeFromVectors()
{
    Circle result = {0, 0, 0, 0};

    double Sxx, Sxy, Syy, v1, v2;

    uint16_t start, end;
    uint16_t nbVectors;
    if (bufSizeVector < NB_VECTORS)
    {
        start = 0;
        end = bufSizeVector;
        nbVectors = bufSizeVector;
    }
    else
    {
        start = indexVector;
        end = NB_VECTORS;
        nbVectors = NB_VECTORS;
    }
    if (nbVectors < 1)
    {
        result.s = 99.0;
        return result;
    }

    // Serial.print("start: ");
    // Serial.print(start);
    // Serial.print(" end: ");
    // Serial.print(end);
    // Serial.print(" nbVectors: ");
    // Serial.println(nbVectors);
    Vector m = getMeanPointFromVectors(start, end, nbVectors);
    vTaskDelay(delayT10);
    // Serial.print("m.x: ");
    // Serial.print(m.x, 16);
    // Serial.print(" m.y: ");
    // Serial.println(m.y, 16);

    // shiftPointsFromVectors(start, end, nbVectors, m);

    double x, y;
    Sxx = Sxy = Syy = v1 = v2 = 0;

    for (int i = start; i < start + nbVectors; i++)
    {
        int index = i % NB_VECTORS;
        double shiftedX = vectors[index].x - m.x;
        double shiftedY = vectors[index].y - m.y;
        Sxx += shiftedX * shiftedX;
        Sxy += shiftedX * vectors[index].y;
        Syy += shiftedY * shiftedY;
        v1 += 0.5 * (shiftedX * shiftedX * shiftedX + shiftedX * shiftedY * shiftedY);
        v2 += 0.5 * (shiftedY * shiftedY * shiftedY + shiftedX * shiftedX * shiftedY);
    }
    double matrix[4] = {Sxx, Sxy, Sxy, Syy};
    double vector[2] = {v1, v2};

    if (linearSolve2x2(matrix, vector, x, y) == false)
    {

        // not enough points or points are colinears
        return result;
    }

    double radius2 = x * x + y * y + (Sxx + Syy) / nbVectors;
    double radius = sqrt(radius2);

    result.a = x + m.x;
    result.b = y + m.y;
    result.r = radius;

    // residue
    result.s = 0;
    for (int i = start; i < start + nbVectors; i++)
    {
        int index = i % NB_VECTORS;
        Vector v;
        double len2;
        v.x = vectors[index].x - result.a;
        v.y = vectors[index].y - result.b;
        len2 = v.x * v.x + v.y * v.y;
        result.s += radius2 - len2;
    }

    Serial.print("a=");
    Serial.print(result.a, 10);
    Serial.print(" b=");
    Serial.print(result.b, 10);
    Serial.print(" r=");
    Serial.print(result.r, 10);
    Serial.print(" s=");
    Serial.println(result.s, 10);

    return result;
}

Circle SimpleCircleFit::fitCircleFromVectors()
{
    Circle circle = computeFromVectors();

    return circle;
}