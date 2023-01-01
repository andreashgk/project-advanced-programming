#ifndef GAME_SFMLFACTORY_H
#define GAME_SFMLFACTORY_H

#include "SFML/Graphics.hpp"

#include "../logic/world/EntityFactory.h"

namespace viewer {

/**
 * The concrete implementation for the world::EntityFactory for the game. This is specifically designed to be used with
 * SFML, as it will couple entities with a viewer made for SFML.
 */
class SFMLFactory : public world::EntityFactory {
public:
    explicit SFMLFactory(sf::RenderWindow& window);

    [[nodiscard]] std::unique_ptr<world::Player> createPlayer() const override;
    [[nodiscard]] std::unique_ptr<world::Tilemap> createTilemap(std::shared_ptr<const world::TileStorage> tiles) const override;
    [[nodiscard]] std::unique_ptr<world::Melon> createMelon(int score) const override;
    [[nodiscard]] std::unique_ptr<world::Finish> createFinish() const override;

private:
    sf::RenderWindow& window;
};

} // namespace viewer

#endif // GAME_SFMLFACTORY_H
