#pragma once

#include "Vector.h"

class Vector2 : public Vector 
{
public:
    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector& vector);

    float getX() const;
    float getY() const;
};