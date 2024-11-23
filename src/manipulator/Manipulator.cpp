#include "Manipulator.h"

Manipulator::Manipulator(MyServo *baseServo, const ArduinoList<ManipulatorArm *> &manipulatorArms, MyServo *grabServo) {
    this->baseServo = baseServo;
    this->manupulatorArms = manipulatorArms;
    this->grabServo = grabServo;
}

void Manipulator::move(Vector3 movement) {
    this->currentPosition += movement;
}

void Manipulator::setCurrentPosition(Vector3 newPosition) {
    this->currentPosition = newPosition;
}

Vector3 Manipulator::getCurrentPosition() const {
    return this->currentPosition;
}

void Manipulator::grab() {
    this->grabServo.setAngleToMax();
}

void Manipulator::release() {
    this->grabServo.setAngleToMin();
}

void Manipulator::squeeze() {
    this->grabServo.rotate(GRAB_AND_RELEASE_SPEED);
}

void Manipulator::decompress() {
    this->grabServo.rotate(-GRAB_AND_RELEASE_SPEED);
}