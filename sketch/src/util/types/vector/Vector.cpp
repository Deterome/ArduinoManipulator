#include "Vector.h"

Vector::Vector(const Vector& vector) {
    *this = vector;
}

Vector::Vector(uint8_t coordsCount) {
    this->coordsCount = coordsCount;
    this->coords = new float[coordsCount];
    for (uint16_t coordId = 0; coordId < coordsCount; coordId++) {
        this->coords[coordId] = 0;
    }
}

Vector::~Vector() {
    delete[] this->coords;
}

bool Vector::equals(const Vector & vector) {
    for (uint16_t coordId = 0; coordId < this->coordsCount; coordId++) {
        if (this->coords[coordId] != vector.coords[coordId]) return false;
    }
    
    return true;
}

float Vector::getCoordById(uint8_t coordId) const {
    return this->coords[coordId];
}

float Vector::getLength() const {
    float sqSum = 0;
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        sqSum += this->coords[coordId] * this->coords[coordId];
    }
    return sqrt(sqSum);
}

const Vector Vector::operator +(const Vector& rightVector) const {
    Vector vectorsSum(*this);
    uint8_t minCoordsCount = this->coordsCount < rightVector.coordsCount ? this->coordsCount : rightVector.coordsCount;
    for (uint8_t coordId = 0; coordId < minCoordsCount; coordId++) {
        vectorsSum.coords[coordId] += rightVector.coords[coordId];
    }
    return vectorsSum;
}

const Vector Vector::operator -(const Vector& rightVector) const {
    Vector vectorsSum(*this);
    uint8_t minCoordsCount = this->coordsCount < rightVector.coordsCount ? this->coordsCount : rightVector.coordsCount;
    for (uint8_t coordId = 0; coordId < minCoordsCount; coordId++) {
        vectorsSum.coords[coordId] -= rightVector.coords[coordId];
    }
    return vectorsSum;
}

const Vector Vector::operator *(const float num) const {
    Vector multVec(*this);
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        multVec.coords[coordId] *= num;
    }
    return multVec;
}

const Vector Vector::operator /(const float num) const {
    Vector divVec(*this);
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        divVec.coords[coordId] /= num;
    }
    return divVec;
}

const Vector& Vector::operator *=(const float num) const
{
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        this->coords[coordId] *= num;
    }
    return *this;
}

const Vector & Vector::operator /=(const float num) const {
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        this->coords[coordId] /= num;
    }
    return *this;
}

bool Vector::operator ==(const Vector &rightVector) {
    if (rightVector.coordsCount != this->coordsCount) return false;
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        if (rightVector.coords[coordId] != this->coords[coordId]) return false;
    }
    return true;
}

Vector& Vector::operator =(const Vector& vector) {
    this->coordsCount = vector.coordsCount;
    this->coords = new float[vector.coordsCount];
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        this->coords[coordId] = vector.coords[coordId];
    }
    return *this;
}

Vector& Vector::operator +=(const Vector& rightVector) {
    uint8_t minCoordsCount = this->coordsCount < rightVector.coordsCount ? this->coordsCount : rightVector.coordsCount;
    for (uint8_t coordId = 0; coordId < minCoordsCount; coordId++) {
        this->coords[coordId] += rightVector.coords[coordId];
    }
    return *this;
}
