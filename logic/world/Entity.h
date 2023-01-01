#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <memory>

#include "../math/Collider.h"
#include "../math/Transform.h"
#include "../math/Vector2.h"
#include "Viewer.h"

namespace world {

/**
 * Base class for entity entity in the world. An entity can be anything ranging from a character controlled by the
 * player to a tilemap that represents a world.
 *
 * Entities have collision and physics built in. A collider can be left empty to not have the entity collide with
 * anything. Entity::onCollide() can also be overridden to allow for custom behaviour on collisions. This can be used
 * for colliders which act like sensors (such as a finish point, or a pickup).
 *
 * The entity's mass can be set to zero to make it act like a static rigidbody, unaffected by forced such as gravity or
 * collision forces trying to move the entity. Will still push away other (dynamic) entities trying to collide with it.
 * When two static entities collide, the collision is ignored.
 */
class Entity : public std::enable_shared_from_this<Entity> {
public:
    Entity(std::unique_ptr<Viewer> viewer, const math::Transform& transform, float mass = 0,
           math::Collider collider = {});
    Entity(Entity&&) = default;
    virtual ~Entity() = default;

    /**
     * Called once every frame to execute any entity-specific logic. This function is also able to modify the world or
     * another entity.
     */
    virtual void tick(World& world) = 0;
    /**
     * Called once every frame to send a draw event to the viewer. This is always sent after every entity in the world
     * has been ticked. Changes to the world that happen here should be kept to a minimum.
     */
    virtual void draw(World& world);
    /**
     * If overridden, this function allows for some specific logic to happen when a collision happens.
     *
     * @param world The game world.
     * @param entity The entity with which the collision is happening.
     * @return True if the collision should continue, false if it should be cancelled.
     */
    virtual bool onCollide(World& world, const std::shared_ptr<Entity>& entity) { return true; };

    /**
     * Returns info about the transform of the entity, such as it's translation in the world.
     */
    [[nodiscard]] const math::Transform& getTransform() const;
    void setTransform(const math::Transform& transform);

    /**
     * Returns the collider translated to the entity's position in the world.
     *
     * @return The translated collider.
     */
    math::Collider getWorldCollider() const;
    /**
     * Returns the collider of the entity.
     * The coordinates in this collider are centered around the entity position which will be (0, 0) in the collider's
     * coordinates. This collider can be empty.
     *
     * @return The collider.
     */
    math::Collider getRelativeCollider() const;

    /**
     * Returns the mass of the entity. If zero, the entity is static and is not affected by forces applied to it.
     */
    float getMass() const;

    /**
     * Returns the current velocity of the entity.
     */
    const math::Vector2& getVelocity() const;
    void setVelocity(const math::Vector2& velocity);

    void applyForce(const math::Vector2& force);

    /**
     * despawn() flags the entity to be despawned. It will be actually removed at the end of the tick.
     */
    void despawn();
    /**
     * @return Whether the entity will be despawned at the end of the tick.
     */
    bool willDespawn() const;

protected:
    void setRelativeCollider(math::Collider collider);

private:
    std::unique_ptr<Viewer> viewer;

    math::Transform transform;
    math::Collider collider;

    float mass;
    math::Vector2 velocity{};

    bool vWillDespawn = false;
};

} // namespace world

#endif // GAME_ENTITY_H
