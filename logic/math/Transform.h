#ifndef GAME_TRANSFORM_H
#define GAME_TRANSFORM_H

#include "Vector2.h"

namespace math {

/**
 * A transformation in space. Rotations are not supported.
 */
struct Transform {
public:
    Vector2 translation;
    Vector2 scale = {1, 1};

    Transform() = default;
    Transform(const Vector2& translation, const Vector2& scale);
};

} // namespace math

#endif // GAME_TRANSFORM_H
