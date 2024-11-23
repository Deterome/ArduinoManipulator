#pragma once

#define DEFAULT_MIN_SERVO180_MICROS 544
#define DEFAULT_MAX_SERVO180_MICROS 2400

#include <Arduino.h>

class MyServo {
private:
    uint16_t minServoMicros = DEFAULT_MIN_SERVO180_MICROS;
    uint16_t maxServoMicros = DEFAULT_MAX_SERVO180_MICROS;
    uint16_t lowerAngleLimit;
    uint16_t upperAngleLimit;
    volatile uint16_t servoAngleInMicros;
    volatile uint16_t servoAngle;
    uint8_t pinOnDemultiplexer;

    const uint16_t angleToMicros(uint16_t angle);
    const uint16_t microsToAngle(uint16_t angleInMicros);
public:
    MyServo(uint8_t pinOnDemultiplexer);
    MyServo(uint8_t pinOnDemultiplexer, uint16_t lowerAngleLimit, uint16_t upperAngleLimit);
    MyServo(uint8_t pinOnDemultiplexer, uint16_t lowerAngleLimit, uint16_t upperAngleLimit, uint16_t minWidthOfPulse, uint16_t maxWidthOfPulse);

    uint16_t getServoAngle() const;
    uint16_t getServoAngleInMicros() const;
    uint8_t getPinOnDemultiplexer() const;
    void rotate(uint16_t rotationAngle);
    void setServoAngle(uint16_t newServoAngle);
    void setServoAngleInMicros(uint16_t servoAngleInMicros);
    void setAngleToMax();
    void setAngleToMin();
};
