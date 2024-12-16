#pragma once

#include "Vector.h"

class Vector3 : public Vector  {
public:
    const static Vector3 ZERO; 

    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector& vector);

    float getX() const;
    float getY() const;
    float getZ() const;
};
