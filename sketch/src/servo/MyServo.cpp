#include "MyServo.h"

MyServo::MyServo(uint8_t pinOnDemultiplexer) : MyServo(pinOnDemultiplexer, 1, AngleOverloadAction::CLAMP, 0, 180, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef) : MyServo(pinOnDemultiplexer, angleIncreaseCoef, AngleOverloadAction::CLAMP, 0, 180*angleIncreaseCoef, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction) : MyServo(pinOnDemultiplexer, angleIncreaseCoef, angleOverloadAction, 0, 180*angleIncreaseCoef, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, uint16_t lowerAngleLimit, uint16_t upperAngleLimit) : MyServo(pinOnDemultiplexer, angleIncreaseCoef, angleOverloadAction, lowerAngleLimit, upperAngleLimit, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, uint16_t lowerAngleLimit, uint16_t upperAngleLimit, uint16_t minWidthOfPulse, uint16_t maxWidthOfPulse) {
    this->angleIncreaseCoef = angleIncreaseCoef;
    this->lowerAngleLimit = lowerAngleLimit;
    this->upperAngleLimit = upperAngleLimit;
    this->pinOnDemultiplexer = pinOnDemultiplexer;
    this->minServoMicros = minWidthOfPulse;
    this->maxServoMicros = maxWidthOfPulse;
    this->angleOverloadAction = angleOverloadAction;
    this->setServoAngle(this->lowerAngleLimit);
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

void MyServo::rotate(int16_t rotationAngle) {
    this->setServoAngle(this->servoAngle + rotationAngle);
}

void MyServo::setServoAngle(int16_t newServoAngle) {
    if (this->servoAngle != newServoAngle) {
        if (this->angleOverloadAction == AngleOverloadAction::CLAMP) {
            if (newServoAngle < (int16_t)this->lowerAngleLimit) newServoAngle = this->lowerAngleLimit;
            else if (newServoAngle > (int16_t)this->upperAngleLimit) newServoAngle = this->upperAngleLimit;
        } else if (this->angleOverloadAction == AngleOverloadAction::REPEAT) {
            if (newServoAngle < (int16_t)this->lowerAngleLimit) {
                newServoAngle = this->upperAngleLimit - ((int16_t)this->lowerAngleLimit - newServoAngle);
            } else if (newServoAngle > (int16_t)this->upperAngleLimit) { 
                newServoAngle = this->lowerAngleLimit + newServoAngle % this->upperAngleLimit;
            }
        }

        this->servoAngle = newServoAngle;
        this->servoAngleInMicros = this->angleToMicros(newServoAngle/this->angleIncreaseCoef);
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
