#include "Camera.h"

namespace world {

Camera::Camera(const math::Vector2& position, const math::AABB& box)
    : position(position), box(box), destination(position), speed(0) {}

Camera::Camera(const math::Vector2& position, const math::AABB& box, const math::Vector2& destination, float speed)
    : position(position), box(box), destination(destination), speed(speed) {}

const math::AABB& Camera::getBox() const { return box; }
void Camera::setBox(const math::AABB& box) { Camera::box = box; }

const math::Vector2& Camera::getPosition() const { return position; }
void Camera::setPosition(const math::Vector2& position) { Camera::position = position; }

const math::Vector2& Camera::getDestination() const { return destination; }
void Camera::setDestination(const math::Vector2& destination) { Camera::destination = destination; }

float Camera::getSpeed() const { return speed; }
void Camera::setSpeed(float speed) { Camera::speed = speed; }

} // namespace world
