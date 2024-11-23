#pragma once

#include <stdint.h>
#include <math.h>
#include "../types/vector/Vector2.h"
#include "../types/Pair.h"

class ArduinoVectorMath
{
public:
    static Pair<Vector2> findCirclesIntersectionPoints(Vector2 firstCircleOrigin, uint16_t firstCircleRadius, Vector2 secondCircleOrigin, uint16_t secondCircleRadius);
    static Vector2 makeVectorFromLengthAndAngle(uint16_t vectorLength, int16_t angle);
};