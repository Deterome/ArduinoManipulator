#pragma once

#define DEFAULT_MIN_SERVO180_MICROS 544
#define DEFAULT_MAX_SERVO180_MICROS 2400

#include <Arduino.h>

class MyServo {
private:
    uint16_t minServoMicros = DEFAULT_MIN_SERVO180_MICROS;
    uint16_t maxServoMicros = DEFAULT_MAX_SERVO180_MICROS;
    volatile uint16_t servoAngleInMicros;
    uint8_t pinOnDemultiplexer;

    const uint16_t angleToMicros(uint8_t angle);
    const uint8_t microsToAngle(uint16_t angleInMicros);
public:
    MyServo(uint8_t pinOnDemultiplexer);
    MyServo(uint8_t pinOnDemultiplexer, uint16_t minWidthOfPulse, uint16_t maxWidthOfPulse);

    uint8_t getServoAngle() const;
    uint16_t getServoAngleInMicros() const;
    uint8_t getPinOnDemultiplexer() const;
    void setServoAngle(uint8_t servoAngle);
    void setServoAngleInMicros(uint16_t servoAngleInMicros);
};
