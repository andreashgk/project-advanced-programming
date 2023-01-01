#ifndef GAME_FINISH_H
#define GAME_FINISH_H

#include "../Entity.h"
#include "../Viewer.h"

namespace world {

/**
 * An entity that represents the goal that the player has to reach. Multiple of these can exists in the world.
 */
class Finish : public Entity {
public:
    Finish(std::unique_ptr<Viewer> v, math::Transform tr);

    void tick(World& world) override;
    bool onCollide(World& world, const std::shared_ptr<Entity>& e) override;

private:
    bool collected = false;
};

} // namespace world

#endif // GAME_FINISH_H
