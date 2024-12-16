#include "Manipulator.h"

void Manipulator::changeArmsPosition(const Vector3& newPosition) {
    Vector2 newPosInTwoDemensionalSpace(
        ArduinoDefaultMath::findRectangleDiameter(newPosition.getX(), newPosition.getY()), 
        newPosition.getZ()
    );
    this->recursivelyChangeArmsPosition(this->manipulatorArms.size() - 1, newPosInTwoDemensionalSpace, 0);
}

Vector2 Manipulator::recursivelyChangeArmsPosition(const uint8_t& armId, const Vector2& destinationPointInTwoDimensionalSpace, const float& allowedZoneAroundDest) {
    Vector2 finalDestination = destinationPointInTwoDimensionalSpace;
    Vector2 armBasePos = this->getArmBasePositionInTwoDimensionalSpace(armId);
    
    ArduinoList<Vector2> interceptionPoints = ArduinoVectorMath::findCirclesIntersectionPoints(destinationPointInTwoDimensionalSpace, allowedZoneAroundDest, armBasePos, this->manipulatorArms[armId]->getLength());
    if (interceptionPoints.size() == 2) {
        Vector2 curArmPos = armBasePos + this->manipulatorArms[armId]->getVectorizedArm();
        float firstDistance = (interceptionPoints[0] - curArmPos).getLength();
        float secondDistance = (interceptionPoints[1] - curArmPos).getLength();
        finalDestination = firstDistance < secondDistance ? interceptionPoints[0] : interceptionPoints[1];
    } else if (interceptionPoints.size() == 1) {
        finalDestination = interceptionPoints[0];
    } else {
        interceptionPoints.~ArduinoList();
        if (armId > 0) {
            armBasePos = recursivelyChangeArmsPosition(armId - 1, destinationPointInTwoDimensionalSpace, allowedZoneAroundDest + this->manipulatorArms[armId]->getLength());
            if (this->manipulatorArms[armId]->getLength() - ((finalDestination-armBasePos).getLength() - allowedZoneAroundDest) >= 0) {
                return this->recursivelyChangeArmsPosition(armId, destinationPointInTwoDimensionalSpace, allowedZoneAroundDest);
            } 
        } 
    }

    this->manipulatorArms[armId]->getArmServo()->rotate(
        ArduinoVectorMath::findRelationalAngleBetweenVectors(this->manipulatorArms[armId]->getVectorizedArm(), finalDestination - armBasePos)
    );

    return armBasePos + this->manipulatorArms[armId]->getVectorizedArm();
}

Vector2 Manipulator::getArmBasePositionInTwoDimensionalSpace(const uint8_t& armId) {
    Vector2 armBasePosition;
    for (uint8_t previousArmId = 0; previousArmId < armId; previousArmId++) {
        armBasePosition += this->manipulatorArms[previousArmId]->getVectorizedArm();
    }
    return armBasePosition;
}

Manipulator::Manipulator(MyServo *baseServo, const ArduinoList<ManipulatorArm*> &manipulatorArms, MyServo *grabServo) {
    this->baseServo = baseServo;
    this->manipulatorArms = manipulatorArms;
    this->grabServo = grabServo;

    uint16_t allArmsLength = 0;
    for (uint8_t armId = 0; armId < this->manipulatorArms.size(); armId++) {
        allArmsLength += this->manipulatorArms[armId]->getLength();
    }

    this->setCurrentPosition(Vector3(0, 0, allArmsLength));
}

void Manipulator::move(const Vector3& movement) {
    this->setCurrentPosition(this->currentPosition + movement);
}

void Manipulator::setCurrentPosition(const Vector3& newPosition) {
    if (!newPosition.equals(this->currentPosition)) {
        // вращаем сервопривод основания
        baseServo->rotate(
        ArduinoVectorMath::findRelationalAngleBetweenVectors(
            Vector2(currentPosition.getX(), currentPosition.getY()), 
            Vector2(newPosition.getX(), newPosition.getY())
        ));

        // вращаем рычаги манипулятора
        this->changeArmsPosition(newPosition);
        
        this->currentPosition = getArmBasePositionInTwoDimensionalSpace(this->manipulatorArms.size() - 1) + this->manipulatorArms[this->manipulatorArms.size() - 1]->getVectorizedArm();
    }
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