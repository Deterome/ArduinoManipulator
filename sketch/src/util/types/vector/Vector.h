#pragma once

#include <stdint.h>
#include <math.h>

class Vector {
protected:
    float* coords = nullptr;
    uint8_t coordsCount = 0;

    Vector(const Vector& vector);
    Vector(uint8_t coordsCount);
public:
    ~Vector();
    bool equals(const Vector& vector);
    uint8_t getCoordsCount() const;
    float getLength() const;
    Vector getNormalized() const;
    float getCoordById(uint8_t coordId) const;
    void setCoordById(uint8_t coordId, float value);
    Vector& forEachCoord(float (*func)(float));
    Vector operator + (const Vector& rightVector) const;
    Vector operator - (const Vector& rightVector) const;
    Vector operator * (const Vector& rightVector) const;
    Vector operator * (const float num) const;
    Vector operator / (const float num) const;
    Vector& operator *= (const Vector& rightVector);
    Vector& operator *= (const float num);
    Vector& operator /= (const float num);
    Vector& operator = (const Vector& rightVector);
    Vector& operator += (const Vector& rightVector);
    bool operator == (const Vector& rightVector) const;
    bool operator != (const Vector& rightVector) const;
};
