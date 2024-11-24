#include "ArduinoDefaultMath.h"

int ArduinoDefaultMath::findGreatestCommonDivisor(int firstNum, int secondNum) {
    while (1==1)
    {
        int& minNum = firstNum < secondNum ? firstNum : secondNum;
        int& maxNum = firstNum > secondNum ? firstNum : secondNum;

        if (maxNum % minNum == 0) {
            return minNum;
        } else {
            maxNum = maxNum % minNum;
        }
    }
}

double ArduinoDefaultMath::findRectangleDiameter(double firstSide, double secondSide) {
    return sqrt(firstSide*firstSide + secondSide*secondSide);
}
