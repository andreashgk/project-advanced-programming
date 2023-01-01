#ifndef GAME_COLLIDERVIEWER_H
#define GAME_COLLIDERVIEWER_H

#include "SFML/Graphics.hpp"

#include "../../logic/math/Vector2.h"
#include "../../logic/world/Entity.h"
#include "../../logic/world/Viewer.h"

namespace viewer {

/**
 * A viewer which renders the outlines hitbox of an entity. Used for debugging.
 */
class ColliderViewer : public world::Viewer {
public:
    explicit ColliderViewer(sf::RenderWindow& w);
    void view(world::World& world, const world::Entity& entity) override;

private:
    sf::RenderWindow& window;
};

} // namespace viewer

#endif // GAME_COLLIDERVIEWER_H
