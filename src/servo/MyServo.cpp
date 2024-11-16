#include "MyServo.h"

MyServo::MyServo(uint8_t pinOnDemultiplexer) : MyServo(pinOnDemultiplexer, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, uint16_t minWidthOfPulse, uint16_t maxWidthOfPulse) {
    this->pinOnDemultiplexer = pinOnDemultiplexer;
    this->minServoMicros = minWidthOfPulse;
    this->maxServoMicros = maxWidthOfPulse;
    this->servoAngleInMicros = minServoMicros;
}

uint8_t MyServo::getServoAngle() const {
    return this->microsToAngle(this->servoAngleInMicros);
}

uint16_t MyServo::getServoAngleInMicros() const {
    return this->servoAngleInMicros;
}

uint8_t MyServo::getPinOnDemultiplexer() const {
    return this->pinOnDemultiplexer;
}

void MyServo::setServoAngle(uint8_t servoAngle) {
    this->servoAngleInMicros = this->angleToMicros(servoAngle);
}

void MyServo::setServoAngleInMicros(uint16_t servoAngleInMicros) {
    this->servoAngleInMicros = servoAngleInMicros;
}

const uint16_t MyServo::angleToMicros(uint8_t angle) {
    return this->minServoMicros + ((this->maxServoMicros  - this->minServoMicros) / 180) * angle;
}

const uint8_t MyServo::microsToAngle(uint16_t angleInMicros) {
    return (angleInMicros - this->minServoMicros) / ((this->maxServoMicros - this->minServoMicros) / 180);
}
