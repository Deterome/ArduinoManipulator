#pragma once

#include "ManipulatorArm.h"
#include "../util/collections/ArduinoList.h"
#include "../util/types/vector/Vector3.h"
#include "../util/math/ArduinoDefaultMath.h"

#define GRAB_AND_RELEASE_SPEED 1

class Manipulator {
private:
    ArduinoList<ManipulatorArm*> manupulatorArms;
    MyServo* baseServo;
    Vector3 currentPosition;
    MyServo* grabServo;

    void changeArmsPosition(Vector3 newPosition);
    /// @brief 
    /// @param armId id of arm to move (recursion must start with last manipulator arm)
    /// @param newPosition new position of the manipulator
    /// @return new position of the arm
    Vector2 recursivelyChangeArmsPosition(uint16_t armId, Vector2 destinationPointInTwoDimensionalSpace, double allowedZoneAroundDest);
    Vector2 getArmBasePositionInTwoDimensionalSpace(uint16_t armId);
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