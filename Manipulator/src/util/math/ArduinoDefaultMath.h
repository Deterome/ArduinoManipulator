#pragma once

#include <math.h>

class ArduinoDefaultMath {

public:
    static int findGreatestCommonDivisor(const int firstNum, const int secondNum);
    static float findRectangleDiameter(const float firstSide, const float secondSide); 
    static float map(float value, float min, float max);
};

