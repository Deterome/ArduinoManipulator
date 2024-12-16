#pragma once

#include <Arduino.h>
#include "ManipulatorArm.h"
#include "../util/collections/ArduinoList.h"
#include "../util/types/vector/Vector3.h"
#include "../util/math/ArduinoDefaultMath.h"

#define GRAB_AND_RELEASE_SPEED 1

class Manipulator {
private:
    ArduinoList<ManipulatorArm*> manipulatorArms;
    MyServo* baseServo;
    Vector3 currentPosition;
    MyServo* grabServo;

    void changeArmsPosition(const Vector3& newPosition);
    /// @brief 
    /// @param armId id of arm to move (recursion must start with last manipulator arm)
    /// @param newPosition new position of the manipulator
    /// @return new position of the arm
    Vector2 recursivelyChangeArmsPosition(const uint8_t& armId, const Vector2& destinationPointInTwoDimensionalSpace, const float& allowedZoneAroundDest);
    Vector2 getArmBasePositionInTwoDimensionalSpace(const uint8_t& armId);
public:
    Manipulator(MyServo* baseServo, const ArduinoList<ManipulatorArm*>& manipulatorArms, MyServo* grabServo);

    void move(const Vector3& movement);
    void setCurrentPosition(const Vector3& newPosition);
    Vector3 getCurrentPosition() const;
    void grab();
    void release();
    void squeeze();
    void decompress();
};