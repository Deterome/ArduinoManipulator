#pragma once

#include "../servo/MyServo.h"

class ManipulatorArm {
private:
    MyServo* servo;
    float length;
public:
    ManipulatorArm(MyServo* servo, float length);

    float getLength() const;
    MyServo* getArmServo();
};