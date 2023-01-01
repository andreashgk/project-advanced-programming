#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "../math/AABB.h"

namespace world {

/**
 * The 'object' that specifies what part of the world can be viewed. It is not an actual entity.
 */
class Camera {
public:
    Camera(const math::Vector2& position, const math::AABB& box);
    Camera(const math::Vector2& position, const math::AABB& box, const math::Vector2& destination, float speed);

    /**
     * The bounds of what the camera can view, proportional to world coordinates. Always centered. To get the actual
     * world coordinates, this box should be translated by the camera's position.
     */
    const math::AABB& getBox() const;
    void setBox(const math::AABB& box);

    /**
     * @return The position of the center most point that the camera is viewing.
     */
    const math::Vector2& getPosition() const;
    void setPosition(const math::Vector2& position);

    /**
     * @return The final/maximum position the camera should be in.
     */
    const math::Vector2& getDestination() const;
    void setDestination(const math::Vector2& destination);

    /**
     * Returns the speed at which the camera can move to its destination by itself. Can be equal to zero.
     */
    float getSpeed() const;
    void setSpeed(float speed);

private:
    math::AABB box;
    math::Vector2 position;
    math::Vector2 destination;
    float speed;
};

} // namespace world

#endif // GAME_CAMERA_H
