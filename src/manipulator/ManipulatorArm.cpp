#include "ManipulatorArm.h"

ManipulatorArm::ManipulatorArm(MyServo* servo, uint8_t length) {
    this->servo = servo;
    this->length = length;
}

uint8_t ManipulatorArm::getLength() const {
    return this->length;
}

uint8_t ManipulatorArm::getArmAngle() const {
    return this->servo->getServoAngle();
}

Vector2 ManipulatorArm::getVectorizedArm() const {
    return ArduinoVectorMath::makeVectorFromLengthAndAngle(this->length, this->servo->getServoAngle());
}
