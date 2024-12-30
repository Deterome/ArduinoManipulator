#include "ArduinoVectorMath.h"

ArduinoList<Vector2> ArduinoVectorMath::findCirclesIntersectionPoints(const Vector2& firstCircleOrigin, const float firstCircleRadius, const Vector2& secondCircleOrigin, const float secondCircleRadius)
{
    ArduinoList<Vector2> intersectionPoints;
    float distanceBetweenFirstAndSecondOrigins = (secondCircleOrigin - firstCircleOrigin).getLength();
    if (distanceBetweenFirstAndSecondOrigins > firstCircleRadius + secondCircleRadius || 
    firstCircleRadius > distanceBetweenFirstAndSecondOrigins + secondCircleRadius ||
    secondCircleRadius > distanceBetweenFirstAndSecondOrigins + firstCircleRadius) return intersectionPoints;
    // first of all, we build a triangle from our two points and the intersection point
    // then we lower the height from the intersection point
    // find the distance between the first point and the point where the height was lowered
    float distanceBetweenFirstPoinAndHeight = ((firstCircleRadius * firstCircleRadius) - (secondCircleRadius * secondCircleRadius) + (distanceBetweenFirstAndSecondOrigins * distanceBetweenFirstAndSecondOrigins)) / (2 * distanceBetweenFirstAndSecondOrigins);
    // find the height
    float height = sqrt((firstCircleRadius * firstCircleRadius) - (distanceBetweenFirstPoinAndHeight * distanceBetweenFirstPoinAndHeight));
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

Vector2 ArduinoVectorMath::makeVectorFromLengthAndAngle(const float vectorLength, const float angle) {
    return Vector2(cos(anglesToRadians(angle)) * vectorLength, sin(anglesToRadians(angle)) * vectorLength);
}

float ArduinoVectorMath::scalarMultiplication(const Vector2& firstVec, const Vector2& secondVec) {
    return firstVec.getX() * secondVec.getX() + firstVec.getY() * secondVec.getY();
}

float ArduinoVectorMath::findAngleBetweenVectors(const Vector2& firstVec, const Vector2& secondVec) {
    float multiplicationOfVectorsLength = firstVec.getLength() * secondVec.getLength();
    if (multiplicationOfVectorsLength == 0) return 0;
    return radiansToAngles(acos(scalarMultiplication(firstVec, secondVec)/(multiplicationOfVectorsLength)));
}

float ArduinoVectorMath::findRelationalAngleBetweenVectors(const Vector2& mainVec, const Vector2& relationalVec) {
    float angleBetweenVecs = findAngleBetweenVectors(mainVec, relationalVec);
    Vector2 normalToMainVec = rotateVectorOnAngle(mainVec, 90);
    float angleBetweenNormalAndRelationalVec = findAngleBetweenVectors(relationalVec, normalToMainVec);
    return angleBetweenNormalAndRelationalVec < 90 ? angleBetweenVecs : -angleBetweenVecs;
}

float ArduinoVectorMath::radiansToAngles(const float radians) {
    return 180.0/M_PI*radians;
}

float ArduinoVectorMath::anglesToRadians(const float angle) {
    return M_PI/180*angle;
}

bool ArduinoVectorMath::isPointInsideTheCircle(const Vector2& point, const Vector2& circleOrigin, const float circleRadius) {
    Vector2 pointInCircleBasis = point - circleOrigin;
    return (pointInCircleBasis.getX()*pointInCircleBasis.getX() + pointInCircleBasis.getY()*pointInCircleBasis.getY()) <= (circleRadius*circleRadius);
}

Vector2 ArduinoVectorMath::rotateVectorOnAngle(const Vector2& initialVec, const int16_t rotateAngle) {
    return Vector2(
        initialVec.getX()*cos(anglesToRadians(rotateAngle))-initialVec.getY()*sin(anglesToRadians(rotateAngle)),
        initialVec.getX()*sin(anglesToRadians(rotateAngle))+initialVec.getY()*cos(anglesToRadians(rotateAngle))
    );
}
