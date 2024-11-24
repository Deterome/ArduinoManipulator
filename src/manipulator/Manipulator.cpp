#include "Manipulator.h"

void Manipulator::changeArmsPosition(Vector3 newPosition) {
    Vector2 newPosInTwoDeminsionalSpace(
        ArduinoDefaultMath::findRectangleDiameter(newPosition.getX(), newPosition.getY()), 
        newPosition.getZ()
    );
    this->recursivelyChangeArmsPosition(this->manupulatorArms.size() - 1, newPosInTwoDeminsionalSpace, 0);
}

Vector2 Manipulator::recursivelyChangeArmsPosition(uint16_t armId, Vector2 destinationPointInTwoDimensionalSpace, double allowedZoneAroundDest) {
    double distanceBetweenArmTopAndNewArmPos;
    Vector2 armBasePos = this->getArmBasePositionInTwoDimensionalSpace(armId);
    Vector2 curArmPos = armBasePos + this->manupulatorArms[armId]->getVectorizedArm();
    ArduinoList<Vector2> interceptionPoints = ArduinoVectorMath::findCirclesIntersectionPoints(destinationPointInTwoDimensionalSpace, allowedZoneAroundDest, armBasePos, this->manupulatorArms[armId]->getLength());
    if (interceptionPoints.size() == 2) {
        double firstDist = (interceptionPoints[0] - curArmPos).getLength();
        double secondDist = (interceptionPoints[1] - curArmPos).getLength();
        distanceBetweenArmTopAndNewArmPos = firstDist < secondDist ? firstDist : secondDist;
    } else if (interceptionPoints.size() == 1) {
        distanceBetweenArmTopAndNewArmPos = (interceptionPoints[0] - curArmPos).getLength();
    } else {
        if (armId > 0) {
            armBasePos = recursivelyChangeArmsPosition(armId - 1, destinationPointInTwoDimensionalSpace, allowedZoneAroundDest + this->manupulatorArms[armId]->getLength());
        }
        Vector2 vectorBetweenArmBottomAndDest = destinationPointInTwoDimensionalSpace - armBasePos;
        if (vectorBetweenArmBottomAndDest.getLength() - allowedZoneAroundDest <= this->manupulatorArms[armId]->getLength()) {
            return this->recursivelyChangeArmsPosition(armId, destinationPointInTwoDimensionalSpace, allowedZoneAroundDest);
        } else {
            vectorBetweenArmBottomAndDest /= vectorBetweenArmBottomAndDest.getLength();
            vectorBetweenArmBottomAndDest *= this->manupulatorArms[armId]->getLength();
            distanceBetweenArmTopAndNewArmPos = (vectorBetweenArmBottomAndDest-curArmPos).getLength();
        }
    }

    this->manupulatorArms[armId]->getArmServo()->rotate(
        2*asin(distanceBetweenArmTopAndNewArmPos/(2* this->manupulatorArms[armId]->getLength()))
    );

    return armBasePos + this->manupulatorArms[armId]->getVectorizedArm();
}

Vector2 Manipulator::getArmBasePositionInTwoDimensionalSpace(uint16_t armId) {
    Vector2 armBasePosition;
    for (uint16_t previousArmId = 0; previousArmId < armId; previousArmId++) {
        armBasePosition += this->manupulatorArms[previousArmId]->getVectorizedArm();
    }
    return armBasePosition;
}

Manipulator::Manipulator(MyServo *baseServo, const ArduinoList<ManipulatorArm *> &manipulatorArms, MyServo *grabServo)
{
    this->baseServo = baseServo;
    this->manupulatorArms = manipulatorArms;
    this->grabServo = grabServo;
}

void Manipulator::move(Vector3 movement) {
    this->setCurrentPosition(this->currentPosition + movement);
}

void Manipulator::setCurrentPosition(Vector3 newPosition) {
    // вращаем сервопривод основания
    baseServo->rotate(
        ArduinoVectorMath::findAngleBetweenVectors(
            Vector2(currentPosition.getX(), currentPosition.getY()), 
            Vector2(newPosition.getX(), newPosition.getY())
        ));    
    // вращаем рычаги манипулятора
    this->changeArmsPosition(newPosition);

    this->currentPosition = newPosition;
}

Vector3 Manipulator::getCurrentPosition() const {
    return this->currentPosition;
}

void Manipulator::grab() {
    this->grabServo->setAngleToMax();
}

void Manipulator::release() {
    this->grabServo->setAngleToMin();
}

void Manipulator::squeeze() {
    this->grabServo->rotate(GRAB_AND_RELEASE_SPEED);
}

void Manipulator::decompress() {
    this->grabServo->rotate(-GRAB_AND_RELEASE_SPEED);
}