#include "Vector.h"

Vector::Vector(const Vector& vector) {
    *this = vector;
}

Vector::Vector(uint8_t coordsCount) {
    this->coordsCount = coordsCount;
    this->coords = new double[coordsCount];
}

Vector::~Vector() {
    delete[] this->coords;
}

double Vector::getCoordById(uint8_t coordId) const {
    return this->coords[coordId];
}

double Vector::getLength() const {
    double sqSum = 0;
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

const Vector Vector::operator *(const double num) const {
    Vector multVec(*this);
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        multVec.coords[coordId] *= num;
    }
    return multVec;
}

Vector& Vector::operator =(const Vector& vector) {
    this->coordsCount = vector.coordsCount;
    this->coords = new double[vector.coordsCount];
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
