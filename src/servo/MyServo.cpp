#include "MyServo.h"

MyServo::MyServo(uint8_t pinOnDemultiplexer) : MyServo(pinOnDemultiplexer, 0, 180, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, uint16_t lowerAngleLimit, uint16_t upperAngleLimit) : MyServo(pinOnDemultiplexer, lowerAngleLimit, upperAngleLimit, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, uint16_t lowerAngleLimit, uint16_t upperAngleLimit, uint16_t minWidthOfPulse, uint16_t maxWidthOfPulse) {
    this->lowerAngleLimit = lowerAngleLimit;
    this->upperAngleLimit = upperAngleLimit;
    this->pinOnDemultiplexer = pinOnDemultiplexer;
    this->minServoMicros = minWidthOfPulse;
    this->maxServoMicros = maxWidthOfPulse;
    this->servoAngleInMicros = minServoMicros;
}

uint16_t MyServo::getServoAngle() const {
    return this->servoAngle;
}

uint16_t MyServo::getServoAngleInMicros() const {
    return this->servoAngleInMicros;
}

uint8_t MyServo::getPinOnDemultiplexer() const {
    return this->pinOnDemultiplexer;
}

void MyServo::rotate(uint16_t rotationAngle) {
    this->setServoAngle(this->servoAngle + rotationAngle);
}

void MyServo::setServoAngle(uint16_t newServoAngle) {
    if (this->servoAngle != newServoAngle) {
        if (newServoAngle < this->lowerAngleLimit) newServoAngle = this->lowerAngleLimit;
        else if (newServoAngle > this->upperAngleLimit) newServoAngle = this->upperAngleLimit;

        this->servoAngle = newServoAngle;
        this->servoAngleInMicros = this->angleToMicros(newServoAngle);
    }
}

void MyServo::setServoAngleInMicros(uint16_t servoAngleInMicros) {
    this->servoAngleInMicros = servoAngleInMicros;
}

void MyServo::setAngleToMax() {
    this->setServoAngle(this->upperAngleLimit);
}

void MyServo::setAngleToMin() {
    this->setServoAngle(this->lowerAngleLimit);
}

const uint16_t MyServo::angleToMicros(uint16_t angle) {
    return this->minServoMicros + ((this->maxServoMicros  - this->minServoMicros) / 180) * angle;
}

const uint16_t MyServo::microsToAngle(uint16_t angleInMicros) {
    return (angleInMicros - this->minServoMicros) / ((this->maxServoMicros - this->minServoMicros) / 180);
}
