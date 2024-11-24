#pragma once

#include <stdint.h>
#include <math.h>
#include "../types/vector/Vector2.h"
#include "../collections/ArduinoList.h"

class ArduinoVectorMath
{
public:
    static ArduinoList<Vector2> findCirclesIntersectionPoints(Vector2 firstCircleOrigin, uint16_t firstCircleRadius, Vector2 secondCircleOrigin, uint16_t secondCircleRadius);
    static Vector2 makeVectorFromLengthAndAngle(uint16_t vectorLength, int16_t angle);
    static double scalarMultiplication(Vector2 firstVec, Vector2 secondVec);
    static double findAngleBetweenVectors(Vector2 firstVec, Vector2 secondVec);
    static bool isPointInsideTheCircle(Vector2 point, Vector2 circleOrigin, double circleRadius);
};