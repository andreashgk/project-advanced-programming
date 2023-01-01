#include "Collider.h"

#include <cmath>
#include <iostream>
#include <utility>

namespace math {

Collider::Collider(AABB box) : boxes({box}) { outer = box; }

Collider::Collider(std::vector<AABB> boxes) : boxes(std::move(boxes)) { outer = calcOuterBox(); }

const std::vector<AABB>& Collider::getContent() const { return boxes; }

const std::optional<AABB>& Collider::getOuterBox() const { return outer; }

std::optional<AABB> Collider::calcOuterBox() const {
    if (boxes.empty())
        return {};
    if (boxes.size() == 1)
        return boxes[0];
    Vector2 min;
    Vector2 max;
    for (const auto& box : boxes) {
        if (box.getMin().x < min.x)
            min.x = box.getMin().x;
        if (box.getMin().y < min.y)
            min.y = box.getMin().y;
        if (box.getMax().x > max.x)
            max.x = box.getMax().x;
        if (box.getMax().y > max.y)
            max.y = box.getMax().y;
    }
    return AABB(min, max);
}

bool Collider::intersects(const Collider& other) const {
    std::optional<AABB> thisOuter = getOuterBox();
    std::optional<AABB> otherOuter = other.getOuterBox();
    if (!thisOuter.has_value() || !otherOuter.has_value())
        return false;
    if (!thisOuter.value().intersects(otherOuter.value()))
        return false;

    for (const AABB& thisBox : boxes) {
        for (const AABB& otherBox : other.boxes) {
            if (thisBox.intersects(otherBox))
                return true;
        }
    }
    return false;
}

std::optional<Vector2> Collider::intersectVector(const Collider& other) const {
    std::optional<AABB> thisOuter = getOuterBox();
    std::optional<AABB> otherOuter = other.getOuterBox();
    if (!thisOuter.has_value() || !otherOuter.has_value())
        return {};
    if (!thisOuter.value().intersects(otherOuter.value()))
        return {};

    Vector2 highest;
    for (const AABB& thisBox : boxes) {
        for (const AABB& otherBox : other.boxes) {
            auto vec = thisBox.intersectVector(otherBox);
            if (vec.has_value()) {
                auto vecVal = vec.value();
                if (std::fabs(highest.x) < std::fabs(vecVal.x))
                    highest.x = vecVal.x;
                if (std::fabs(highest.y) < std::fabs(vecVal.y))
                    highest.y = vecVal.y;
            }
        }
    }
    if (highest.getLength() == 0)
        return {};
    return highest;
}

Collider Collider::operator+(const Vector2& other) const {
    std::vector<AABB> newCol{};
    newCol.reserve(boxes.size());
    for (const auto& box : boxes) {
        newCol.emplace_back(box + other);
    }
    auto o = outer;
    if (o.has_value()) {
        o.value() = {o.value().getMin() + other, o.value().getMax() + other};
    }
    return Collider(newCol, o);
}

std::ostream& operator<<(std::ostream& os, const Collider& v) {
    os << "Collider[";

    bool first = true;
    for (const auto& box : v.boxes) {
        if (!first)
            os << ", ";
        os << box;
        first = false;
    }

    os << "]";
    return os;
}

Collider::Collider(std::vector<AABB> boxes, std::optional<AABB> outer) : boxes(std::move(boxes)), outer(outer) {}

Collider Collider::scale(const Vector2& other) const {
    std::vector<AABB> newCollider;
    newCollider.reserve(boxes.size());
    for (const auto& box : boxes) {
        newCollider.emplace_back(box * other);
    }

    std::optional<AABB> newOuter = outer;
    if (newOuter.has_value())
        newOuter.value() = newOuter.value() * other;

    return {newCollider, newOuter};
}

} // namespace math