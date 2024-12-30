#include "Manipulator.h"

void Manipulator::changeArmsPosition(const Vector3& newPosition) {
    Vector2 newPosInTwoDemensionalSpace(
        ArduinoDefaultMath::findRectangleDiameter(newPosition.getX(), newPosition.getY()), 
        newPosition.getZ()
    );

    ArduinoList<float> tempArmsAngles;
    for (uint8_t armId = 0; armId < this->manipulatorArms.size(); armId++) {
        tempArmsAngles.add(manipulatorArms[armId]->getArmServo()->getServoAngle());
    }
    
    this->recursivelyFindArmsRotationToMoveToDestionation(this->manipulatorArms.size() - 1, 
        newPosInTwoDemensionalSpace, 
        0,
        this->getArmBasePositionUsingListOfAnglesInTwoDimensionalSpace(this->manipulatorArms.size() - 1, tempArmsAngles), 
        tempArmsAngles,
        false
    );

    for (uint8_t armId = 0; armId < this->manipulatorArms.size(); armId++) {
        this->manipulatorArms[armId]->getArmServo()->setServoAngle(tempArmsAngles[armId]);
    }
}

bool Manipulator::recursivelyFindArmsRotationToMoveToDestionation(const uint8_t& armId, const Vector2& destinationPointInTwoDimensionalSpace, const float& allowedZoneAroundDest, Vector2 armBasePos, ArduinoList<float>& armsAngles, bool upstreamPassage) {
    ArduinoList<Vector2> possibleMovePoints = ArduinoVectorMath::findCirclesIntersectionPoints(destinationPointInTwoDimensionalSpace, armId == this->manipulatorArms.size() - 1 ? 0.01f : allowedZoneAroundDest, armBasePos, this->manipulatorArms[armId]->getLength());
    Serial.print("  arm id: ");
    Serial.println(armId);
    Serial.print("destination in two dimensional space: ");
    Serial.print(destinationPointInTwoDimensionalSpace.getX());
    Serial.print(" ");
    Serial.println(destinationPointInTwoDimensionalSpace.getY());
    Serial.print("possible move points: ");
    for (uint8_t i = 0; i < possibleMovePoints.size(); i++) {
        Serial.print(possibleMovePoints[i].getX());
        Serial.print(" ");
        Serial.print(possibleMovePoints[i].getY());
        Serial.print("; ");
    }
    Serial.println();
    Serial.print("allowed zone around dest: ");
    Serial.println(allowedZoneAroundDest);
    Serial.print("arm base pos: ");
    Serial.print(armBasePos.getX());
    Serial.print(" ");
    Serial.println(armBasePos.getY());
    if (possibleMovePoints.size() > 0) {
        for (uint8_t possibleMovePointId = 0; possibleMovePointId < possibleMovePoints.size(); possibleMovePointId++) {
            float rotationAngle = ArduinoVectorMath::findRelationalAngleBetweenVectors(
                getVectorOfArmUsingListOfAnglesInTwoDimensionalSpace(armId, armsAngles), 
                possibleMovePoints[possibleMovePointId] - armBasePos);
            Serial.print("rotation of arm ");
            Serial.print(armId);
            Serial.print(": ");
            Serial.print(rotationAngle);
            if (this->manipulatorArms[armId]->getArmServo()->checkIfServoCanRotateOnAngle(rotationAngle)) {
                Serial.print(" CAN ROTATE");
                armsAngles[armId] += rotationAngle;
                Serial.print("; new angle: ");
                Serial.println(armsAngles[armId]);
                if (armId == this->manipulatorArms.size() - 1 
                    || recursivelyFindArmsRotationToMoveToDestionation(armId + 1, 
                        destinationPointInTwoDimensionalSpace, 
                        allowedZoneAroundDest - this->manipulatorArms[armId + 1]->getLength(), 
                        possibleMovePoints[possibleMovePointId], 
                        armsAngles,
                        true)) {
                    return true;
                }
            }
            else Serial.println(" CAN'T ROTATE");
        }
    }
    if (!upstreamPassage && armId > 0){
        return recursivelyFindArmsRotationToMoveToDestionation(armId - 1, destinationPointInTwoDimensionalSpace, allowedZoneAroundDest + this->manipulatorArms[armId]->getLength(), this->getArmBasePositionUsingListOfAnglesInTwoDimensionalSpace(armId - 1, armsAngles), armsAngles, false);
    }
    Serial.println("error");
    // armsAngles[armId] += ArduinoVectorMath::findRelationalAngleBetweenVectors(getVectorOfArmUsingListOfAnglesInTwoDimensionalSpace(armId, armsAngles), destinationPointInTwoDimensionalSpace - armBasePos);
    return false;
}

Vector2 Manipulator::getVectorOfArmUsingListOfAnglesInTwoDimensionalSpace(uint8_t armId, const ArduinoList<float>& listOfArmsAngles) const {
    float armAngle = 0;
    for (uint8_t prevArmId = 0; prevArmId <= armId; prevArmId++) {
        armAngle += listOfArmsAngles.getById(prevArmId) - this->manipulatorArms.getById(prevArmId)->getArmServo()->getAngleShift();
    }
    return ArduinoVectorMath::makeVectorFromLengthAndAngle(this->manipulatorArms.getById(armId)->getLength(), armAngle);
}

Vector2 Manipulator::getVectorOfBase() const { 
    return ArduinoVectorMath::makeVectorFromLengthAndAngle(1, baseServo->getServoAngle());
}

Vector2 Manipulator::getArmBasePositionUsingListOfAnglesInTwoDimensionalSpace(const uint8_t& armId, const ArduinoList<float>& listOfArmsAngles) {
    Vector2 armBasePosition;
    for (uint8_t previousArmId = 0; previousArmId < armId; previousArmId++) {
        armBasePosition += getVectorOfArmUsingListOfAnglesInTwoDimensionalSpace(previousArmId, listOfArmsAngles);
    }
    return armBasePosition;
}

Manipulator::Manipulator(MyServo *baseServo, const ArduinoList<ManipulatorArm*> &manipulatorArms, MyServo *grabServo) {
    this->baseServo = baseServo;
    this->manipulatorArms = manipulatorArms;
    this->grabServo = grabServo;
    for (uint8_t armId = 0; armId < this->manipulatorArms.size(); armId++) {
        this->armsTotalLength += this->manipulatorArms[armId]->getLength();
    }
    this->setCurrentPosition(Vector3(0, 0, this->armsTotalLength));
}

void Manipulator::move(const Vector3& movement) {
    this->setCurrentPosition(this->currentPosition + movement);
}

void Manipulator::setCurrentPosition(Vector3 newPosition) {
    Serial.print("\nArms total length: ");
    Serial.println(this->armsTotalLength);
    if (newPosition.getLength() > this->armsTotalLength) {
        newPosition = newPosition.getNormalized() * this->armsTotalLength;
    }
    Serial.print("New pos: ");
    Serial.print(newPosition.getX());
    Serial.print(" ");
    Serial.print(newPosition.getY());
    Serial.print(" ");
    Serial.println(newPosition.getZ());
    if (!newPosition.equals(this->currentPosition)) {
        // вращаем сервопривод основания
        baseServo->rotate(
        ArduinoVectorMath::findRelationalAngleBetweenVectors(
            getVectorOfBase(), 
            Vector2(newPosition.getX(), newPosition.getY())
        ));
        // вращаем рычаги манипулятора
        this->changeArmsPosition(newPosition);

        this->currentPosition = newPosition;
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