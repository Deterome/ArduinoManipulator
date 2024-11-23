#include "Vector3.h"

Vector3::Vector3() : Vector(3) {}

Vector3::Vector3(double x, double y, double z) : Vector(3) {
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = z;
}

Vector3::Vector3(const Vector& vector) : Vector(vector) {}

double Vector3::getX() const {
    return this->coords[0];
}
double Vector3::getY() const {
    return this->coords[1];
}
double Vector3::getZ() const {
    return this->coords[2];
}