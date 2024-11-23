#include "ArduinoVectorMath.h"

Pair<Vector2> ArduinoVectorMath::findCirclesIntersectionPoints(Vector2 firstCircleOrigin, uint16_t firstCircleRadius, Vector2 secondCircleOrigin, uint16_t secondCircleRadius)
{
    // first of all, we build a triangle from our two points and the intersection point
    double distanceBetweenFirstAndSecondOrigins = (secondCircleOrigin - firstCircleOrigin).getLength();
    // then we lower the height from the intersection point
    
    // find the distance between the first point and the point where the height was lowered
    double distanceBetweenFirstPoinAndHeight = ((firstCircleRadius * firstCircleRadius) - (secondCircleRadius * secondCircleRadius) + (distanceBetweenFirstAndSecondOrigins * distanceBetweenFirstAndSecondOrigins)) / (2 * distanceBetweenFirstAndSecondOrigins);
    // find the height
    double height = sqrt((firstCircleRadius * firstCircleRadius) - (distanceBetweenFirstPoinAndHeight * distanceBetweenFirstPoinAndHeight));
    // find the point where the height was lowered
    Vector2 heightPoint = firstCircleOrigin + (secondCircleOrigin - firstCircleOrigin) * (distanceBetweenFirstPoinAndHeight / distanceBetweenFirstAndSecondOrigins);

    Vector2 firstInterceptionPoint(
        heightPoint.getX() + (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getY() - firstCircleOrigin.getY()), 
        heightPoint.getY() - (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getX() - firstCircleOrigin.getX())); 
    if (distanceBetweenFirstAndSecondOrigins == 0) return Pair<Vector2>(firstInterceptionPoint, firstInterceptionPoint); 
    
    Vector2 secondInterceptionPoint(
        heightPoint.getX() - (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getY() - firstCircleOrigin.getY()), 
        heightPoint.getY() + (height / distanceBetweenFirstAndSecondOrigins) * (secondCircleOrigin.getX() - firstCircleOrigin.getX())); 

    return Pair<Vector2>(firstInterceptionPoint, secondInterceptionPoint);
}

Vector2 ArduinoVectorMath::makeVectorFromLengthAndAngle(uint16_t vectorLength, int16_t angle) {
    return Vector2(cos(angle) * vectorLength, sin(angle) * vectorLength);
}
