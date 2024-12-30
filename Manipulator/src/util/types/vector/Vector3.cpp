#include "Vector3.h"

const Vector3 Vector3::ZERO = Vector3(0, 0, 0);

Vector3::Vector3() : Vector(3) {}

Vector3::Vector3(float x, float y, float z) : Vector(3) {
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = z;
}

Vector3::Vector3(const Vector &vector) : Vector(vector) {}

float Vector3::getX() const {
    return this->coords[0];
}
float Vector3::getY() const {
    return this->coords[1];
}
float Vector3::getZ() const {
    return this->coords[2];
}

void Vector3::setX(float value) {
    this->coords[0] = value;
}

void Vector3::setY(float value) {
    this->coords[1] = value;
}

void Vector3::setZ(float value) {
    this->coords[2] = value;
}