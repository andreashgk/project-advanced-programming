#ifndef GAME_TILEMAPVIEWER_H
#define GAME_TILEMAPVIEWER_H

#include "SFML/Graphics.hpp"

#include "../../logic/resources/Manager.h"
#include "../../logic/world/Viewer.h"
#include "../../logic/world/entities/Tilemap.h"
#include "../assets/Tileset.h"
#include "ColliderViewer.h"

namespace viewer {

/**
 * Per-world settings that alter how tilemaps in the world are rendered.
 */
class TilemapViewConfig : public resources::Resource {
public:
    TilemapViewConfig(std::string tileset, std::string defaultTileName, std::uint8_t defaultTileVariant);

    /**
     * The name that identifies the tile that will be drawn 'outside' of the tilemap.
     */
    const std::string& getDefaultTileName() const;
    /**
     * The variant to use for the default tile.
     */
    uint8_t getDefaultTileVariant() const;

    /**
     * The name of the tileset asset to use for tilemaps in the world.
     */
    const assets::Tileset& getTileset() const;

private:
    std::string tileset;

    std::string defaultTileName;
    std::uint8_t defaultTileVariant;
};

/**
 * A tilemap viewer will view an entity as a matrix of different square tiles. It will always try to fill the entire
 * screen with tiles, even when the actual tilemap does not have a tile at a given location. This is useful to draw a
 * 'background' tile. The default tile in the TilemapViewConfig is used for this.
 * Tilemaps will not draw any tiles that are outside of what the camera can see, so big tilemaps should not be too big
 * of a performance hit.
 */
class TilemapViewer : public world::Viewer {
public:
    TilemapViewer(sf::RenderWindow& window, std::shared_ptr<const world::TileStorage> tiles);

    void view(world::World& world, const world::Entity& entity) override;

private:
    sf::RenderWindow& window;

    std::shared_ptr<const world::TileStorage> tileStorage;
};

} // namespace viewer

#endif // GAME_TILEMAPVIEWER_H
