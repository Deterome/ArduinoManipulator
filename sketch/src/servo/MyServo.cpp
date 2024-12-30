#include "MyServo.h"

MyServo::MyServo(uint8_t pinOnDemultiplexer) : MyServo(pinOnDemultiplexer, 1, AngleOverloadAction::CLAMP, 0, 0, 180, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef) : MyServo(pinOnDemultiplexer, angleIncreaseCoef, AngleOverloadAction::CLAMP, 0, 0, 180*angleIncreaseCoef, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction) : MyServo(pinOnDemultiplexer, angleIncreaseCoef, angleOverloadAction, 0, 0, 180*angleIncreaseCoef, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, int16_t angleShift) : MyServo(pinOnDemultiplexer, angleIncreaseCoef, angleOverloadAction, angleShift, 0, 180*angleIncreaseCoef, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, int16_t angleShift, uint16_t lowerAngleLimit, uint16_t upperAngleLimit) : MyServo(pinOnDemultiplexer, angleIncreaseCoef, angleOverloadAction, angleShift, lowerAngleLimit, upperAngleLimit, DEFAULT_MIN_SERVO180_MICROS, DEFAULT_MAX_SERVO180_MICROS) {}

MyServo::MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, int16_t angleShift, uint16_t lowerAngleLimit, uint16_t upperAngleLimit, uint16_t minWidthOfPulse, uint16_t maxWidthOfPulse) {
    this->angleIncreaseCoef = angleIncreaseCoef;
    this->lowerAngleLimit = lowerAngleLimit;
    this->upperAngleLimit = upperAngleLimit;
    this->pinOnDemultiplexer = pinOnDemultiplexer;
    this->minServoMicros = minWidthOfPulse;
    this->maxServoMicros = maxWidthOfPulse;
    this->angleOverloadAction = angleOverloadAction;
    this->angleShift = angleShift;
    this->setServoAngle(this->lowerAngleLimit);
}

void MyServo::setInterpolationMode(bool isEnabled) {
    if (isEnabled) this->interpolatedAngleInMicros = this->servoAngleInMicros;
    this->isInterpolationEnabled = isEnabled;
}

void MyServo::setInterpolationStep(uint16_t interpolationStep) {
    this->interpolationStep = interpolationStep;
}

void MyServo::setAngleShift(int16_t angleShift) {
    this->angleShift = angleShift;
    this->setServoAngle(this->servoAngle);
}

int16_t MyServo::getAngleShift() const { 
    return this->angleShift; 
}

bool MyServo::checkIfServoCanRotateOnAngle(float angle) {
    float newAngle = this->servoAngle + angle;
    return this->angleOverloadAction == AngleOverloadAction::REPEAT || 
        newAngle >= this->lowerAngleLimit && newAngle <= this->upperAngleLimit;
}

float MyServo::getServoAngle() const {
    return this->servoAngle - this->angleShift;
}

uint16_t MyServo::getServoAngleInMicros() {
    if (!isInterpolationEnabled || this->interpolatedAngleInMicros == this->servoAngleInMicros) return this->servoAngleInMicros;
    else {
        if (this->interpolatedAngleInMicros < this->servoAngleInMicros) {
            this->interpolatedAngleInMicros += interpolationStep;
            if (this->interpolatedAngleInMicros > this->servoAngleInMicros) this->interpolatedAngleInMicros = this->servoAngleInMicros;
        } else if (this->interpolatedAngleInMicros > this->servoAngleInMicros) {
            this->interpolatedAngleInMicros -= interpolationStep;
            if (this->interpolatedAngleInMicros < this->servoAngleInMicros) this->interpolatedAngleInMicros = this->servoAngleInMicros;
        }
        return this->interpolatedAngleInMicros;
    }    
}

uint8_t MyServo::getPinOnDemultiplexer() const {
    return this->pinOnDemultiplexer;
}

void MyServo::rotate(float rotationAngle) {
    this->setServoAngle(this->servoAngle + rotationAngle);
}

void MyServo::setServoAngle(float newServoAngle) {
    newServoAngle += this->angleShift;
    if (this->servoAngle != newServoAngle) {
        if (this->angleOverloadAction == AngleOverloadAction::CLAMP) {
            if (newServoAngle < this->lowerAngleLimit) newServoAngle = this->lowerAngleLimit;
            else if (newServoAngle > this->upperAngleLimit) newServoAngle = this->upperAngleLimit;
        } else if (this->angleOverloadAction == AngleOverloadAction::REPEAT) {
            newServoAngle = ArduinoDefaultMath::map(newServoAngle, this->lowerAngleLimit, this->upperAngleLimit);
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
