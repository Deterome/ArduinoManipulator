#include "ArduinoDefaultMath.h"

int ArduinoDefaultMath::findGreatestCommonDivisor(const int firstNum, const int secondNum) {
    while (1==1)
    {
        int minNum = firstNum < secondNum ? firstNum : secondNum;
        int maxNum = firstNum > secondNum ? firstNum : secondNum;

        if (maxNum % minNum == 0) {
            return minNum;
        } else {
            maxNum = maxNum % minNum;
        }
    }
}

float ArduinoDefaultMath::findRectangleDiameter(const float firstSide, const float secondSide) {
    return sqrt(firstSide*firstSide + secondSide*secondSide);
}

float ArduinoDefaultMath::map(float value, float min, float max) {
    float diff = max - min;
    if (value < min) {
        while (value < min) {
            value += diff;
        }
    } else if (value > max) {
        while (value > max) {
            value -= diff;
        }
    }
    return value;
}
