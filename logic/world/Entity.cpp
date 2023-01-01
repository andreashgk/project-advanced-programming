#include "Entity.h"

#include <utility>

#include "../utils/Stopwatch.h"
#include "World.h"

namespace world {

Entity::Entity(std::unique_ptr<Viewer> viewer, const math::Transform& transform, float mass, math::Collider collider)
    : viewer(std::move(viewer)), transform(transform), collider(std::move(collider)), mass(mass) {}

void Entity::draw(World& world) { viewer->view(world, *this); }

const math::Transform& Entity::getTransform() const { return transform; }
void Entity::setTransform(const math::Transform& tra) { transform = tra; }

math::Collider Entity::getWorldCollider() const { return collider.scale(getTransform().scale) + transform.translation; }
math::Collider Entity::getRelativeCollider() const { return collider; }
void Entity::setRelativeCollider(math::Collider col) { Entity::collider = std::move(col); }

float Entity::getMass() const { return mass; }

const math::Vector2& Entity::getVelocity() const { return velocity; }
void Entity::setVelocity(const math::Vector2& vel) { Entity::velocity = vel; }

void Entity::applyForce(const math::Vector2& force) {
    if (mass == 0)
        return;
    velocity += (force / mass) * utils::Stopwatch::get().getDelta();
}

void Entity::despawn() { vWillDespawn = true; }
bool Entity::willDespawn() const { return vWillDespawn; }

} // namespace world
