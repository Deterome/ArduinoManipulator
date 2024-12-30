#pragma once

#include <Arduino.h>
#include "ManipulatorArm.h"
#include "../util/collections/ArduinoList.h"
#include "../util/types/vector/Vector3.h"
#include "../util/math/ArduinoDefaultMath.h"
#include "../util/math/ArduinoVectorMath.h"
#include "../util/types/Optional.h"

#define GRAB_AND_RELEASE_SPEED 1

class Manipulator {
private:
    ArduinoList<ManipulatorArm*> manipulatorArms;
    MyServo* baseServo;
    Vector3 currentPosition;
    MyServo* grabServo;
    float armsTotalLength = 0;

    void changeArmsPosition(const Vector3& newPosition);
    /// @brief 
    /// @param armId id of arm to move (recursion must start with last manipulator arm)
    /// @param newPosition new position of the manipulator
    /// @return new position of the arm
    // Vector2 recursivelyChangeArmsPosition(const uint8_t& armId, const Vector2& destinationPointInTwoDimensionalSpace, const float& allowedZoneAroundDest);
    Vector2 getArmBasePositionUsingListOfAnglesInTwoDimensionalSpace(const uint8_t& armId, const ArduinoList<float>& listOfArmsAngles);
    bool recursivelyFindArmsRotationToMoveToDestionation(const uint8_t& armId, const Vector2& destinationPointInTwoDimensionalSpace, const float& allowedZoneAroundDest, Vector2 armBasePos, ArduinoList<float>& armsAngles, bool upstreamPassage);
    // bool recursivelyTryToMoveArmsToDestination(const uint8_t& armId, const float& allowedZoneAroundDest, const Vector2& destinationInTwoDimensionalSpace, const Vector2& armBasePos);
    Vector2 getVectorOfArmUsingListOfAnglesInTwoDimensionalSpace(uint8_t armId, const ArduinoList<float>& listOfArmsAngles) const;
    Vector2 getVectorOfBase() const;
public:
    Manipulator(MyServo* baseServo, const ArduinoList<ManipulatorArm*>& manipulatorArms, MyServo* grabServo);

    void move(const Vector3& movement);
    void setCurrentPosition(Vector3 newPosition);
    Vector3 getCurrentPosition() const;
    void grab();
    void release();
    void squeeze();
    void decompress();
};