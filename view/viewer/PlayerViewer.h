#ifndef GAME_PLAYERVIEWER_H
#define GAME_PLAYERVIEWER_H

#include "SFML/Graphics.hpp"

#include "../../logic/math/Vector2.h"
#include "../../logic/world/Entity.h"
#include "../../logic/world/Viewer.h"
#include "ColliderViewer.h"

namespace viewer {

/**
 * PlayerViewer is a special viewer type which draws the player entity. It decides which animations to draw and which
 * way the player should be facing. It is also responsible for showing the player spawn and despawn animations.
 */
class PlayerViewer : public world::Viewer {
public:
    PlayerViewer(sf::RenderWindow& window);
    void view(world::World& world, const world::Entity& entity) override;

private:
    float animStart = 0;
    sf::RenderWindow& window;
    bool first = true;
};

} // namespace viewer

#endif // GAME_PLAYERVIEWER_H
