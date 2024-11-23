#pragma once

#include "Vector.h"

class Vector2 : public Vector 
{
public:
    Vector2();
    Vector2(double x, double y);
    Vector2(const Vector& vector);

    double getX() const;
    double getY() const;
};