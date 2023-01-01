#ifndef GAME_VECTOR2_H
#define GAME_VECTOR2_H

#include <iostream>

namespace math {

/**
 * A vector consisting of two floating point components. Can be used to represent a point in space, or a direction and
 * length.
 */
struct Vector2 {
public:
    float x, y;

    /**
     * Creates a zero vector.
     */
    Vector2();
    Vector2(float x, float y);

    /**
     * Returns the length of the vector.
     */
    float getLength() const;
    /**
     * Normalizes the vector to have a length of 1. Should not be used if the vector is a zero vector.
     */
    Vector2 getNormalized() const;
    /**
     * Normalizes the vector to have a length of 1, or leaves it as it is if the length is zero.
     */
    Vector2 getNormalizedOrZero() const;
    /**
     * Normalizes this vector.
     */
    void normalize();
    /**
     * Normalizes this vector, or leaves it as zero when it is the zero vector.
     */
    void normalizeOrZero();

    Vector2 operator-() const;
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    float operator*(const Vector2& other) const;
    Vector2 operator*(float rhs) const;
    Vector2 operator/(float rhs) const;
    bool operator==(const Vector2& rhs) const;
    Vector2& operator+=(const Vector2& rhs);
    Vector2& operator-=(const Vector2& rhs);

    friend std::ostream& operator<<(std::ostream& os, Vector2 const& v);
};

} // namespace math

#endif // GAME_VECTOR2_H
