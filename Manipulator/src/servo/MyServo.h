#pragma once

#define DEFAULT_MIN_SERVO180_MICROS 544
#define DEFAULT_MAX_SERVO180_MICROS 2400

#include <Arduino.h>
#include "../util/math/ArduinoDefaultMath.h"

enum class AngleOverloadAction {CLAMP, REPEAT};

class MyServo {
private:
    uint16_t minServoMicros = DEFAULT_MIN_SERVO180_MICROS;
    uint16_t maxServoMicros = DEFAULT_MAX_SERVO180_MICROS;
    int16_t lowerAngleLimit;
    int16_t upperAngleLimit;
    volatile uint16_t servoAngleInMicros = DEFAULT_MIN_SERVO180_MICROS;
    volatile float servoAngle = 0;
    uint8_t pinOnDemultiplexer;
    float angleIncreaseCoef;
    AngleOverloadAction angleOverloadAction;
    int16_t angleShift = 0;
    bool isInterpolationEnabled = false;
    uint16_t interpolatedAngleInMicros = 0;
    uint16_t interpolationStep = (DEFAULT_MAX_SERVO180_MICROS - DEFAULT_MIN_SERVO180_MICROS)/3;

    const uint16_t angleToMicros(uint16_t angle);
    const uint16_t microsToAngle(uint16_t angleInMicros);
public:
    MyServo(uint8_t pinOnDemultiplexer);
    MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef);
    MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction);
    MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, int16_t angleShift);
    MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, int16_t angleShift, int16_t lowerAngleLimit, int16_t upperAngleLimit);
    MyServo(uint8_t pinOnDemultiplexer, float angleIncreaseCoef, AngleOverloadAction angleOverloadAction, int16_t angleShift, int16_t lowerAngleLimit, int16_t upperAngleLimit, uint16_t minWidthOfPulse, uint16_t maxWidthOfPulse);

    void setInterpolationMode(bool isEnabled);
    void setInterpolationStep(uint16_t interpolationStep);
    void setAngleShift(int16_t angleShift);
    int16_t getAngleShift() const;
    bool checkIfServoCanRotateOnAngle(float angle);
    float getServoAngle() const;
    uint16_t getServoAngleInMicros();
    uint8_t getPinOnDemultiplexer() const;
    void rotate(float rotationAngle);
    void setServoAngle(float newServoAngle);
    void setServoAngleInMicros(uint16_t servoAngleInMicros);
    void setAngleToMax();
    void setAngleToMin();
};
