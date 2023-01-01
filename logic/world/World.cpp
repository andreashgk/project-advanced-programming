#include "World.h"

#include <cmath>

#include "../utils/Stopwatch.h"
#include "Entity.h"

namespace world {

World::World(std::shared_ptr<EntityFactory> ef, Camera camera) : entityFactory(std::move(ef)), camera(camera) {}

void World::tick() {
    // Remove all entities that are queued to be despawned.
    int id = -1;
    for (const auto& e : entities) {
        ++id;
        if (!e || !e->willDespawn())
            continue;

        collisions.removeNode(e);
        entities[static_cast<unsigned long>(id)].reset();
        removedEntities.insert(static_cast<std::size_t>(id));
    }
    collisions.clearEdges();
    for (const auto& e : entities) {
        if (!e)
            continue;
        if (e->getMass() != 0) {
            // Apply gravity to the entity.
            e->setVelocity(e->getVelocity() + gravity * utils::Stopwatch::get().getDelta());
            // Apply (simplified) drag to the entity.
            auto vel = e->getVelocity();
            if (fabsf(vel.x) >= std::numeric_limits<float>::epsilon() ||
                fabsf(vel.y) >= std::numeric_limits<float>::epsilon()) {
                float len = vel.getLength();
                float drag = 0.5f * 150.f * len * len;
                e->applyForce(vel.getNormalized() * -drag);
            }
        }

        // Move the entity in the direction of the velocity if it is not zero.
        if (e->getVelocity().getLength() != 0) {
            auto tr = e->getTransform();
            tr.translation += e->getVelocity() * utils::Stopwatch::get().getDelta();
            e->setTransform(tr);
        }

        // Store the 'largest' collision to apply friction afterwards.
        math::Vector2 col;
        for (const auto& e2 : entities) {
            if (!e2 || e == e2)
                continue;

            // Calculate the collision vector between the entities.
            std::optional<math::Vector2> iVecOpt = e->getWorldCollider().intersectVector(e2->getWorldCollider());
            // If there is no collision, we do not need to handle it.
            if (!iVecOpt.has_value()) {
                continue;
            }

            math::Vector2 iVec = iVecOpt.value();

            collisions.setEdge(e, e2, iVec);

            // Store both results in a value to prevent short-circuit evaluation.
            bool e1Collide = e->onCollide(*this, e2);
            bool e2Collide = e2->onCollide(*this, e);
            if (!e1Collide || !e2Collide) {
                continue;
            }

            // Collisions do not affect static entities.
            if (e->getMass() == 0)
                continue;

            if (col.getLength() < iVec.getLength())
                col = iVec;

            // Move the entity back. If both entities are not static entities, the entity with the least amount of mass
            // will be moved the most.
            if (e2->getMass() != 0)
                iVec = iVec * (e2->getMass() / (e->getMass() + e2->getMass()));
            auto tr = e->getTransform();
            tr.translation -= iVec;
            e->setTransform(tr);

            // Set the velocity of the entity to zero in the direction of the collision.
            if (!(iVec.x == 0 && iVec.y == 0)) {
                e->setVelocity(e->getVelocity() -
                               iVec.getNormalizedOrZero() * (e->getVelocity() * iVec) / iVec.getLength());
            }
        }
        // Apply friction if the entity is colliding with another entity.
        if (e->getMass() > 0 && col.getLength() > std::numeric_limits<float>::epsilon() &&
            e->getVelocity().getLength() > std::numeric_limits<float>::epsilon()) {
            if (col.y != 0) {
                e->setVelocity(e->getVelocity() - e->getVelocity() * 15 * utils::Stopwatch::get().getDelta());
            } else {
                e->setVelocity(e->getVelocity() - e->getVelocity() * 6 * utils::Stopwatch::get().getDelta());
            }
        }
    }
    for (const auto& e : entities) {
        if (!e)
            continue;
        e->tick(*this);
    }
    // Try to move the camera closer to its destination.
    camera.setPosition(camera.getPosition() +
                       (camera.getDestination() - camera.getPosition()).getNormalizedOrZero() * camera.getSpeed());
}

void World::draw() {
    for (const auto& e : entities) {
        if (!e)
            continue;
        e->draw(*this);
    }
}

std::shared_ptr<Entity> World::spawnEntity(std::shared_ptr<Entity> entity, std::optional<math::Transform> transform) {
    if (transform.has_value()) {
        entity->setTransform(transform.value());
    }
    insertEntity(entity);
    return entity;
}

std::vector<std::shared_ptr<Entity>> World::getEntities() {
    std::vector<std::shared_ptr<Entity>> vec;
    for (const auto& e : entities) {
        if (!e)
            continue;
        vec.emplace_back(e);
    }
    return vec;
}

Camera& World::getCamera() { return camera; }
const Camera& World::getCamera() const { return camera; }

void World::insertEntity(std::shared_ptr<Entity> e) {
    collisions.insertNode(e);
    // If there are no free spots in the entity list, put the entity at the back.
    if (removedEntities.empty()) {
        entities.emplace_back(std::move(e));
        return;
    }
    // If there is a free spot in the vector, assign the first free spot to this entity.
    auto keyIt = removedEntities.begin();
    entities[*keyIt] = std::move(e);
    removedEntities.erase(keyIt);
}

const Collisions& World::getCollisions() const { return collisions; }

resources::Manager& World::getResources() { return resources; }

const resources::Manager& World::getResources() const { return resources; }

} // namespace world