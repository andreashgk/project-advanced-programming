#ifndef GAME_VIEWUTILS_HPP
#define GAME_VIEWUTILS_HPP

#include "SFML/Graphics.hpp"

#include "../../logic/math/Vector2.h"
#include "../../logic/world/World.h"

namespace viewer {

math::Vector2 scaleCoordinate(const math::Vector2& in, const world::Camera& cam, const sf::RenderWindow& window) {
    return {((in.x - cam.getPosition().x - cam.getBox().getMin().x) / cam.getBox().getWidth()) *
                float(window.getSize().x),
            ((-in.y + cam.getPosition().y - cam.getBox().getMin().y) / cam.getBox().getHeight()) *
                float(window.getSize().y)};
}

} // namespace viewer

#endif // GAME_VIEWUTILS_HPP
