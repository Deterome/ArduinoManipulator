#include "Vector2.h"

Vector2::Vector2() : Vector(2) {}

Vector2::Vector2(double x, double y) : Vector(2) {
    this->coords[0] = x;
    this->coords[1] = y;
}

Vector2::Vector2(const Vector& vector) : Vector2(vector.getCoordById(0), vector.getCoordById(1)) {}

double Vector2::getX() const {
    return this->coords[0];
}

double Vector2::getY() const {
    return this->coords[1];
}