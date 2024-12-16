#pragma once

#include <stdint.h>
#include <math.h>

class Vector {
protected:
    float* coords;
    uint8_t coordsCount;

    Vector(const Vector& vector);
    Vector(uint8_t coordsCount);
public:
    ~Vector();
    bool equals(const Vector& vector);
    float getCoordById(uint8_t coordId) const;
    float getLength() const;
    const Vector operator + (const Vector& rightVector) const;
    const Vector operator - (const Vector& rightVector) const;
    const Vector operator * (const float num) const;
    const Vector operator / (const float num) const;
    const Vector& operator *= (const float num) const;
    const Vector& operator /= (const float num) const;
    bool operator == (const Vector& rightVector);
    Vector& operator = (const Vector& rightVector);
    Vector& operator += (const Vector& rightVector);
};
