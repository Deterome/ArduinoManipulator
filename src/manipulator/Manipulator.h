#pragma once

#include "ManipulatorArm.h"
#include "../util/collections/ArduinoList.h"
#include "../util/types/vector/Vector3.h"

#define GRAB_AND_RELEASE_SPEED 1

class Manipulator {
private:
    ArduinoList<ManipulatorArm*> manupulatorArms;
    MyServo* baseServo;
    Vector3 currentPosition;
    MyServo* grabServo;
public:
    Manipulator(MyServo* baseServo, const ArduinoList<ManipulatorArm*>& manipulatorArms, MyServo* grabServo);

    void move(Vector3 movement);
    void setCurrentPosition(Vector3 newPosition);
    Vector3 getCurrentPosition() const;
    void grab();
    void release();
    void squeeze();
    void decompress();
};