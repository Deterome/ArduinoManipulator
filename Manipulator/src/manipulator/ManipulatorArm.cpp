#include "ManipulatorArm.h"

ManipulatorArm::ManipulatorArm(MyServo* servo, float length) {
    this->servo = servo;
    this->length = length;
}

float ManipulatorArm::getLength() const {
    return this->length;
}

MyServo* ManipulatorArm::getArmServo() {
    return this->servo;
}
