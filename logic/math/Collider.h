#ifndef GAME_COLLIDER_H
#define GAME_COLLIDER_H

#include <iostream>
#include <optional>
#include <vector>

#include "AABB.h"

namespace math {

/**
 * A collider represents the union of zero, one or multiple Axis-Aligned Bounding Boxes. If a collider consists of zero
 * AABBs, then it will never intersect with anything.
 */
class Collider {
public:
    Collider() = default;
    explicit Collider(AABB box);
    Collider(std::vector<AABB> boxes);

    /**
     * Returns all the boxes that make up this collider.
     */
    const std::vector<AABB>& getContent() const;

    /**
     * Returns a new AABB which has the minimum and maximum x and y values of every box contained in the collider. This
     * will be the smallest possible single AABB that fully contains all AABBs in the collider.
     */
    [[nodiscard]] const std::optional<AABB>& getOuterBox() const;

    /**
     * Checks if two colliders intersect. This is the case when at least one pair of boxes from both colliders intersect
     * with eachother.
     *
     * @param collider The other collider to check intersection with.
     * @return True if the colliders intersect, false otherwise.
     */
    [[nodiscard]] bool intersects(const Collider& collider) const;
    /**
     * Returns the intersection depth of two colliders if they intersect. This is once again the smallest distance
     * needed to resolve the collision. Note that if one collider is 'trapped' in between multiple boxes of the other,
     * the intersection cannot be resolved in a clean way.
     *
     * @param collider The other collider to check intersection with.
     * @return The intersection vector, or null_opt.
     */
    [[nodiscard]] std::optional<Vector2> intersectVector(const Collider& collider) const;

    /**
     * Scales the collider by the components of the provided vector.
     */
    Collider scale(const Vector2& other) const;

    Collider operator+(const Vector2& other) const;

    friend std::ostream& operator<<(std::ostream& os, Collider const& v);

private:
    Collider(std::vector<AABB> boxes, std::optional<AABB> outer);
    std::optional<AABB> outer{};
    std::vector<AABB> boxes{};

    [[nodiscard]] std::optional<AABB> calcOuterBox() const;
};

} // namespace math

#endif // GAME_COLLIDER_H
