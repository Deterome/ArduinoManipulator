#include "Vector.h"

Vector::Vector(const Vector& vector) {
    *this = vector;
}

Vector::Vector(uint8_t coordsCount) {
    this->coordsCount = coordsCount;
    this->coords = new float[coordsCount];
    for (uint16_t coordId = 0; coordId < coordsCount; coordId++) {
        this->coords[coordId] = 0.0f;
    }
}

Vector::~Vector() {
    if (this->coords != nullptr) delete[] this->coords;
}

bool Vector::equals(const Vector & vector) {
    for (uint16_t coordId = 0; coordId < this->coordsCount; coordId++) {
        if (this->coords[coordId] != vector.coords[coordId]) return false;
    }
    
    return true;
}

uint8_t Vector::getCoordsCount() const {
    return this->coordsCount;
}

float Vector::getLength() const {
    float sqSum = 0;
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        sqSum += this->coords[coordId] * this->coords[coordId];
    }
    return sqrt(sqSum);
}

Vector Vector::getNormalized() const {
    return *this/this->getLength();
}

float Vector::getCoordById(uint8_t coordId) const {
    return this->coords[coordId];
}

void Vector::setCoordById(uint8_t coordId, float value) {
    this->coords[coordId] = value;
}

Vector& Vector::forEachCoord(float (*func)(float)) {
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        this->coords[coordId] = func(this->coords[coordId]);
    }
    return *this;
}

Vector Vector::operator +(const Vector& rightVector) const {
    Vector vectorsSum(*this);
    uint8_t minCoordsCount = this->coordsCount < rightVector.coordsCount ? this->coordsCount : rightVector.coordsCount;
    for (uint8_t coordId = 0; coordId < minCoordsCount; coordId++) {
        vectorsSum.coords[coordId] += rightVector.coords[coordId];
    }
    return vectorsSum;
}

Vector Vector::operator -(const Vector& rightVector) const {
    Vector vectorsSum(*this);
    uint8_t minCoordsCount = this->coordsCount < rightVector.coordsCount ? this->coordsCount : rightVector.coordsCount;
    for (uint8_t coordId = 0; coordId < minCoordsCount; coordId++) {
        vectorsSum.coords[coordId] -= rightVector.coords[coordId];
    }
    return vectorsSum;
}

Vector Vector::operator *(const Vector &rightVector) const {
    Vector vectorsMult(*this);
    uint8_t minCoordsCount = this->coordsCount < rightVector.coordsCount ? this->coordsCount : rightVector.coordsCount;
    for (uint8_t coordId = 0; coordId < minCoordsCount; coordId++) {
        vectorsMult.coords[coordId] *= rightVector.coords[coordId];
    }
    return vectorsMult;
}

Vector Vector::operator *(const float num) const {
    Vector multVec(*this);
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        multVec.coords[coordId] *= num;
    }
    return multVec;
}

Vector Vector::operator /(const float num) const {
    Vector divVec(*this);
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        divVec.coords[coordId] /= num;
    }
    return divVec;
}

Vector &Vector::operator *=(const Vector &rightVector) {
    uint8_t minCoordsCount = this->coordsCount < rightVector.coordsCount ? this->coordsCount : rightVector.coordsCount;
    for (uint8_t coordId = 0; coordId < minCoordsCount; coordId++) {
        this->coords[coordId] *= rightVector.coords[coordId];
    }
    return *this;
}

Vector& Vector::operator *=(const float num) {
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        this->coords[coordId] *= num;
    }
    return *this;
}

Vector & Vector::operator /=(const float num) {
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        this->coords[coordId] /= num;
    }
    return *this;
}

bool Vector::operator ==(const Vector &rightVector) const {
    if (rightVector.coordsCount != this->coordsCount) return false;
    for (uint8_t coordId = 0; coordId < this->coordsCount; coordId++) {
        if (rightVector.coords[coordId] != this->coords[coordId]) return false;
    }
    return true;
}

bool Vector::operator !=(const Vector &rightVector) const {
    return !operator==(rightVector);
}

Vector& Vector::operator =(const Vector& vector) {
    if (this->coordsCount != vector.coordsCount) {
        this->coordsCount = vector.coordsCount;
        delete[] this->coords;
        this->coords = new float[vector.coordsCount];
    }
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
