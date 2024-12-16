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
