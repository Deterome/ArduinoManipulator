#pragma once

#include "Vector.h"

class Vector3 : public Vector 
{
public:
    Vector3();
    Vector3(double x, double y, double z);
    Vector3(const Vector& vector);

    double getX() const;
    double getY() const;
    double getZ() const;
};
