#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <memory>
#include <set>
#include <vector>

#include "../data/Graph.h"
#include "../math/Transform.h"
#include "../math/Vector2.h"
#include "../resources/Manager.h"
#include "Camera.h"
#include "Entity.h"
#include "EntityFactory.h"

namespace world {

class Entity;

// We cannot use a weak_ptr here, as it cannot be hashed. Using a shared_ptr is fine however: the node in the graph
// will be deleted as soon as the entity is removed from the world.
typedef data::Graph<std::shared_ptr<const Entity>, math::Vector2> Collisions;

/**
 * The world acts like the container for everything. It contains all entities, resources, etc. It is responsible for
 * handling certain things like physics and collisions between multiple entities.
 */
class World {
public:
    /**
     * Creates a new world.
     *
     * @param vf The implementation for the entity factory. This allows the world to create entities with some specific
     * viewer code without the world having to know what this code is.
     * @param camera The initial state of the world camera.
     */
    explicit World(std::shared_ptr<EntityFactory> vf, Camera camera);
    World(const World&) = delete;
    World(World&&) = default;

    /**
     * Called once per frame. Handles all the logic in the world, such as collisions and applying gravity. Will also
     * tick every entity and despawn entities that are queued to be despawned.
     */
    void tick();
    /**
     * Sends an event to all the viewers to draw the entities in the world.
     */
    void draw();

    /**
     * Adds an entity to the world.
     *
     * @param entity The entity to add to the world.
     * @param transform The transform the entity should have when spawning.
     * @return A shared_ptr reference to the newly spawned entity (for convenience).
     */
    std::shared_ptr<Entity> spawnEntity(std::shared_ptr<Entity> entity, std::optional<math::Transform> transform);
    /**
     * Returns all the entities currently in the world.
     */
    std::vector<std::shared_ptr<Entity>> getEntities();

    /**
     * Returns the world's resource manager. See `resources::Manager` for more info.
     */
    resources::Manager& getResources();
    const resources::Manager& getResources() const;

    /**
     * Returns the world's camera. This reference will always stay valid as long as the world itself is not moved.
     */
    Camera& getCamera();
    const Camera& getCamera() const;

    /**
     * Returns the collision graph, which provides info about which entities are colliding with each other and how much.
     */
    const Collisions& getCollisions() const;

private:
    /**
     * Inserts a new entity into the world. The entity is a unique pointer, ensuring that it is not yet used elsewhere.
     *
     * @param e A unique pointer to the entity to be added to the world.
     */
    void insertEntity(std::shared_ptr<Entity> e);

    Camera camera;
    resources::Manager resources;

    std::shared_ptr<EntityFactory> entityFactory;
    std::vector<std::shared_ptr<Entity>> entities;
    std::set<unsigned long> removedEntities;

    Collisions collisions;

    math::Vector2 gravity = {0, -1.381f};
};

} // namespace world

#endif // GAME_WORLD_H
