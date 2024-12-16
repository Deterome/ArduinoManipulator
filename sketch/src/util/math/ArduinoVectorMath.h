#pragma once

#include <stdint.h>
#include <math.h>
#include "../types/vector/Vector2.h"
#include "../collections/ArduinoList.h"

class ArduinoVectorMath
{
public:
    static ArduinoList<Vector2> findCirclesIntersectionPoints(const Vector2& firstCircleOrigin, const uint16_t firstCircleRadius, const Vector2& secondCircleOrigin, const uint16_t secondCircleRadius);
    static Vector2 makeVectorFromLengthAndAngle(const uint16_t vectorLength, const int16_t angle);
    static float scalarMultiplication(const Vector2& firstVec, const Vector2& secondVec);
    static float findAngleBetweenVectors(const Vector2& firstVec, const Vector2& secondVec);
    static float findRelationalAngleBetweenVectors(const Vector2& mainVec, const Vector2& relationalVec);
    static float radiansToAngles(const float radians);
    static float anglesToRadians(const float angle);
    static bool isPointInsideTheCircle(const Vector2& point, const Vector2& circleOrigin, float circleRadius);
    static Vector2 rotateVectorOnAngle(const Vector2& initialVec, const int16_t rotateAngle);
};