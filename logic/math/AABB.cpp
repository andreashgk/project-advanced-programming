#include "AABB.h"

#include <cmath>
#include <iostream>
#include <optional>

namespace math {

AABB::AABB(const Vector2& v1, const Vector2& v2)
    : minVec(std::fmin(v1.x, v2.x), std::fmin(v1.y, v2.y)), maxVec(std::fmax(v1.x, v2.x), std::fmax(v1.y, v2.y)) {}

Vector2 AABB::getSize() const { return maxVec - minVec; }

Vector2 AABB::getCenter() const { return (minVec + maxVec) / 2; }

const Vector2& AABB::getMin() const { return minVec; }
const Vector2& AABB::getMax() const { return maxVec; }

float AABB::getWidth() const { return maxVec.x - minVec.x; }
float AABB::getHeight() const { return maxVec.y - minVec.y; }

bool AABB::contains(const Vector2& vec) const {
    return vec.x > minVec.x && vec.x < maxVec.x && vec.y > minVec.y && vec.y < maxVec.y;
}

bool AABB::intersects(const AABB& box) const {
    return std::fabs((minVec.x + getWidth() / 2.f) - (box.minVec.x + box.getWidth() / 2.f)) * 2.f <=
               (getWidth() + box.getWidth()) &&
           std::fabs((minVec.y + getHeight() / 2.f) - (box.minVec.y + box.getHeight() / 2.f)) * 2.f <=
               (getHeight() + box.getHeight());
}

std::optional<Vector2> AABB::intersectVector(const AABB& box) const {
    if (!intersects(box))
        return {};

    float dx = std::fmin(box.maxVec.x - minVec.x, maxVec.x - box.minVec.x);
    float dy = std::fmin(box.maxVec.y - minVec.y, maxVec.y - box.minVec.y);
    if (std::fabs(dx) < std::fabs(dy)) {
        if (minVec.x + maxVec.x / 2.f > box.minVec.x + box.maxVec.x / 2.f)
            return {{- dx, 0.f}};
        return {{dx, 0.f}};
    } else {
        if (minVec.y + maxVec.y / 2.f > box.minVec.y + box.maxVec.y / 2.f)
            return {{0.f, - dy}};
        return {{0.f, dy}};
    }
}

AABB AABB::operator+(const Vector2& other) const { return {minVec + other, maxVec + other}; }

std::ostream& operator<<(std::ostream& os, const AABB& v) {
    return os << "AABB(" << v.minVec << ", " << v.maxVec << ")";
}

AABB AABB::operator/(float rhs) const { return {minVec / rhs, maxVec / rhs}; }
AABB AABB::operator*(float rhs) const { return {minVec * rhs, maxVec * rhs}; }
AABB AABB::operator*(const Vector2& rhs) const {
    return {{minVec.x * rhs.x, minVec.y * rhs.y}, {maxVec.x * rhs.x, maxVec.y * rhs.y}};
}

} // namespace math