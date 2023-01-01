#ifndef GAME_ENTITYFACTORY_H
#define GAME_ENTITYFACTORY_H

#include <memory>

#include "entities/Finish.h"
#include "entities/Melon.h"
#include "entities/Player.h"
#include "entities/Tilemap.h"

namespace world {

/**
 * An abstract entity factory. Every world needs an entity factory in order to allow for entities to be created.
 */
class EntityFactory {
public:
    virtual ~EntityFactory() = default;

    [[nodiscard]] virtual std::unique_ptr<Player> createPlayer() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Tilemap> createTilemap(
        std::shared_ptr<const world::TileStorage> tiles) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Melon> createMelon(int score) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Finish> createFinish() const = 0;
};

} // namespace world

#endif // GAME_ENTITYFACTORY_H
