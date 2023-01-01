#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "../Entity.h"

namespace world {

/**
 * The main character of the game. It is controlled by inputs provided by the player.
 */
class Player : public Entity {
public:
    Player(std::unique_ptr<Viewer> viewer, const math::Transform& transform);
    void tick(World& world) override;

private:
    float jumpCooldown{};
    float jumpTimer{};
};

} // namespace world

#endif // GAME_PLAYER_H
