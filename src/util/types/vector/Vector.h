#pragma once

#include <stdint.h>
#include <math.h>

class Vector {
protected:
    double* coords;
    uint8_t coordsCount;

    Vector(const Vector& vector);
    Vector(uint8_t coordsCount);
public:
    ~Vector();
    double getCoordById(uint8_t coordId) const;
    double getLength() const;
    const Vector operator + (const Vector& rightVector) const;
    const Vector operator - (const Vector& rightVector) const;
    const Vector operator * (const double num) const;
    Vector& operator = (const Vector& rightVector);
    Vector& operator += (const Vector& rightVector);
};
