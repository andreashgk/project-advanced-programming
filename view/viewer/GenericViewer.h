#pragma once

#include "SFML/Graphics.hpp"

#include "../../logic/math/Vector2.h"
#include "../../logic/world/Entity.h"
#include "../../logic/world/Viewer.h"

namespace viewer {

/**
 * GenericViewer is a viewer that always views the target entity as a simple texture.
 */
class GenericViewer : public world::Viewer {
public:
    GenericViewer(sf::RenderWindow& window, const sf::Texture& tx);
    void view(world::World& world, const world::Entity& entity) override;

private:
    sf::Sprite sprite;
    sf::RenderWindow& window;
};

} // namespace viewer
