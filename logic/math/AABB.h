#ifndef GAME_AABB_H
#define GAME_AABB_H

#include <iostream>
#include <optional>

#include "Vector2.h"

namespace math {

/**
 * An AABB is an axis-aligned bounding box. It is used to define a rectangular area that cannot be rotated.
 */
class AABB {
public:
    /**
     * Create a new axis aligned bounding box between the provided locations. There is no order in which the coordinates
     * must be provided.
     */
    AABB(const Vector2& v1, const Vector2& v2);

    /**
     * Returns the coordinate in the center of the box.
     */
    Vector2 getCenter() const;
    /**
     * Returns the width and height respectively of the box.
     */
    Vector2 getSize() const;
    /**
     * Get the point of the box where the X and Y coordinates are the lowest. This is the bottom left corner.
     */
    const Vector2& getMin() const;
    /**
     * Get the point of the box where the X and Y coordinates are the highest. This is the top right corner.
     */
    const Vector2& getMax() const;

    /**
     * Returns the width of the box. This corresponds with delta between the highest and lowest x value in the box.
     */
    float getWidth() const;
    /**
     * Returns the height of the box. This corresponds with delta between the highest and lowest y value in the box.
     */
    float getHeight() const;

    /**
     * Checks if a given vector is inside of the box. If the vector is exactly on an edge, this is not counted as being
     * inside of the box.
     *
     * @param vec The vector to check.
     * @return True if the vector is inside the box, false otherwise.
     */
    [[nodiscard]] bool contains(const Vector2& vec) const;
    /**
     * Checks if two boxes intersect with each other. This is the case if they have at least 1 point in common. This
     * point is not necessarily a corner, but may be inside of both boxes. If both boxes share a corner or a side, this
     * also counts as intersecting.
     *
     * @param box The box to check for an intersection with.
     * @return True if the boxes intersect, false otherwise.
     */
    [[nodiscard]] bool intersects(const AABB& box) const;
    /**
     * If the caller intersects with the provided box, this returns a vector of how much they intersect. More
     * specifically, this vector will correspond with the shortest possible distance the AABB that is provided as
     * argument should be translated so that the two boxes no longer intersect.
     *
     * @param box The box to check for an intersection with.
     * @return A value with the intersect vector of both boxes intersect, or a null_opt otherwise.
     */
    [[nodiscard]] std::optional<Vector2> intersectVector(const AABB& box) const;

    AABB operator+(const Vector2& other) const;
    AABB operator*(const Vector2& rhs) const;
    AABB operator*(float rhs) const;
    AABB operator/(float rhs) const;

    friend std::ostream& operator<<(std::ostream& os, AABB const& v);

private:
    Vector2 minVec, maxVec;
};

} // namespace math

#endif // GAME_AABB_H
