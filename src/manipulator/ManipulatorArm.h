#pragma once

#include "../servo/MyServo.h"
#include "../util/types/vector/Vector2.h"
#include "../util/math/ArduinoVectorMath.h"

class ManipulatorArm {
private:
    MyServo* servo;
    uint8_t length;
public:
    ManipulatorArm(MyServo* servo, uint8_t length);

    uint8_t getLength() const;
    MyServo* getArmServo();
    Vector2 getVectorizedArm() const;
};