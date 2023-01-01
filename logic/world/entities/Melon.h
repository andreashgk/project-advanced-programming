#ifndef GAME_MELON_H
#define GAME_MELON_H

#include "../Entity.h"
#include "../Viewer.h"

namespace world {

/**
 * A pickup for the player to collect and gain score in return. The amount that is received can be configured per
 * entity.
 */
class Melon : public Entity {
public:
    Melon(std::unique_ptr<Viewer> v, math::Transform tr, int score = 100);

    void tick(World& world) override;
    bool onCollide(World& world, const std::shared_ptr<Entity>& e) override;

private:
    bool collected = false;
    int score;
};

} // namespace world

#endif // GAME_MELON_H
