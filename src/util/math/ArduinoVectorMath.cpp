#include "ArduinoVectorMath.h"

ArduinoList<Vector2> ArduinoVectorMath::findCirclesIntersectionPoints(Vector2 firstCircleOrigin, uint16_t firstCircleRadius, Vector2 secondCircleOrigin, uint16_t secondCircleRadius)
{
    ArduinoList<Vector2> intersectionPoints;
    double distanceBetweenFirstAndSecondOrigins = (secondCircleOrigin - firstCircleOrigin).getLength();
    if (distanceBetweenFirstAndSecondOrigins > firstCircleRadius + secondCircleRadius || 
    firstCircleRadius > distanceBetweenFirstAndSecondOrigins + secondCircleRadius ||
    secondCircleRadius > distanceBetweenFirstAndSecondOrigins + firstCircleRadius) return intersectionPoints;
    // first of all, we build a triangle from our two points and the intersection point
    // then we lower the height from the intersection point
    // find the distance between the first point and the point where the height was lowered
    double distanceBetweenFirstPoinAndHeight = ((firstCircleRadius * firstCircleRadius) - (secondCircleRadius * secondCircleRadius) + (distanceBetweenFirstAndSecondOrigins * distanceBetweenFirstAndSecondOrigins)) / (2 * distanceBetweenFirstAndSecondOrigins);
    // find the height
    double height = sqrt((firstCircleRadius * firstCircleRadius) - (distanceBetweenFirstPoinAndHeight * distanceBetweenFirstPoinAndHeight));
    // find the point where the height was lowered
    Vector2 heightPoint = firstCircleOrigin + (secondCircleOrigin - firstCircleOrigin) * (distanceBetweenFirstPoinAndHeight / distanceBetweenFirstAndSecondOrigins);

    intersectionPoints.add(Vector2(
        heightPoint.getX() + (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getY() - firstCircleOrigin.getY()), 
        heightPoint.getY() - (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getX() - firstCircleOrigin.getX())
    )); 
    if (distanceBetweenFirstAndSecondOrigins != firstCircleRadius + secondCircleRadius) {
        intersectionPoints.add(Vector2(
            heightPoint.getX() - (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getY() - firstCircleOrigin.getY()), 
            heightPoint.getY() + (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getX() - firstCircleOrigin.getX())
        )); 
    }

    return intersectionPoints;
}

Vector2 ArduinoVectorMath::makeVectorFromLengthAndAngle(uint16_t vectorLength, int16_t angle) {
    return Vector2(cos(angle) * vectorLength, sin(angle) * vectorLength);
}

double ArduinoVectorMath::scalarMultiplication(Vector2 firstVec, Vector2 secondVec) {
    return firstVec.getX() * secondVec.getX() + firstVec.getY() * secondVec.getY();
}

double ArduinoVectorMath::findAngleBetweenVectors(Vector2 firstVec, Vector2 secondVec) {
    return acos(scalarMultiplication(firstVec, secondVec)/(firstVec.getLength() * secondVec.getLength()));
}

bool ArduinoVectorMath::isPointInsideTheCircle(Vector2 point, Vector2 circleOrigin, double circleRadius) {
    Vector2 pointInCircleBasis = point - circleOrigin;
    return (pointInCircleBasis.getX()*pointInCircleBasis.getX() + pointInCircleBasis.getY()*pointInCircleBasis.getY()) <= (circleRadius*circleRadius);
}
