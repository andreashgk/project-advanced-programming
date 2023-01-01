#include "Vector2.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace math {

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}

float Vector2::getLength() const { return std::sqrt(x * x + y * y); }

Vector2 Vector2::getNormalized() const {
    if (x == 0 && y == 0)
        throw std::overflow_error("Trying to normalize a zero vector");
    float len = getLength();
    return {x / len, y / len};
}

Vector2 Vector2::getNormalizedOrZero() const {
    if (x == 0 && y == 0) {
        return {0, 0};
    }
    float len = getLength();
    return {x / len, y / len};
}

void Vector2::normalize() {
    if (x == 0 && y == 0)
        throw std::overflow_error("Trying to normalize a zero vector");
    normalizeOrZero();
}

void Vector2::normalizeOrZero() {
    float len = getLength();
    if (len == 0) {
        x = 0;
        y = 0;
        return;
    }
    x /= len;
    y /= len;
}

Vector2 Vector2::operator-() const { return {- x, - y}; }
Vector2 Vector2::operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
Vector2 Vector2::operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
Vector2 Vector2::operator*(float rhs) const { return {x * rhs, y * rhs}; }
Vector2 Vector2::operator/(float rhs) const { return {x / rhs, y / rhs}; }
Vector2& Vector2::operator+=(const Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}
bool Vector2::operator==(const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
float Vector2::operator*(const Vector2& other) const { return x * other.x + y * other.y; }

std::ostream& operator<<(std::ostream& os, const Vector2& v) { return os << "Vector2(" << v.x << ", " << v.y << ")"; }

} // namespace math
