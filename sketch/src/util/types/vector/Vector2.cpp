#include "Vector2.h"

Vector2::Vector2() : Vector(2) {}

Vector2::Vector2(float x, float y) : Vector(2) {
    this->coords[0] = x;
    this->coords[1] = y;
}

Vector2::Vector2(const Vector& vector) : Vector2(vector.getCoordById(0), vector.getCoordById(1)) {}

float Vector2::getX() const {
    return this->coords[0];
}

float Vector2::getY() const {
    return this->coords[1];
}