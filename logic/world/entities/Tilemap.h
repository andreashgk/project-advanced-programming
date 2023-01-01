#ifndef GAME_TILEMAP_H
#define GAME_TILEMAP_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "../Entity.h"

namespace world {

/**
 * A tile type which can be placed inside of a tilemap.
 */
class Tile {
public:
    Tile(std::string name, math::Collider collider);

    /**
     * The name of the tile type. Should correspond with a tile name in a tileset asset.
     */
    const std::string& getName() const;
    /**
     * The tile's collider. This is normalized to be between (0, 0) and (1, 1). The collider can still go outside of
     * these coordinates, which would mean that the collider extends outside the actual tile.
     */
    const math::Collider& getCollider() const;

private:
    std::string name;
    math::Collider collider;
};

/**
 * Mappings for all the tile types supported by a tilemap. Each type has to be unique and up to 256 different types are
 * allowed. Cannot be modified after construction.
 */
class Tileset {
public:
    Tileset(std::initializer_list<Tile> tiles);
    explicit Tileset(std::vector<Tile> tiles);

    Tileset(const Tileset&) = default;
    Tileset(Tileset&&) = default;

    /**
     * Returns the amount of different tile types in the tileset.
     */
    std::size_t getSize() const;

    /**
     * Returns the tile that corresponds with the provided index.
     */
    const Tile& getTile(std::uint8_t id) const;
    /**
     * Returns the index for the given tile type.
     */
    std::uint8_t getIndex(const Tile& tile) const;
    /**
     * Returns the index for a tile type with the given name.
     */
    std::uint8_t getIndex(const std::string& tileName) const;

private:
    std::vector<Tile> tiles;
    std::unordered_map<std::string, std::uint8_t> tileLookup;
};

/**
 * A storage container for tiles. Represents a m*n matrix, with each element being a tile type.
 * Tile types are stored in a palette, and the actual matrix uses palette indices.
 */
class TileStorage {
public:
    /**
     * Creates a new zero width and height tile storage with a palette.
     */
    explicit TileStorage(Tileset palette);

    TileStorage(const TileStorage&) = default;
    TileStorage(TileStorage&&) = default;

    /**
     * The amount of tiles that the storage spans in the x-axis.
     */
    int getWidth() const;
    /**
     * The amount of tiles that the storage spans in the y-axis.
     */
    int getHeight() const;
    /**
     * Returns all the tile types that may be present in the tile storage.
     */
    const Tileset& getPalette() const;

    /**
     * Checks if a given tile position is inside of the tile storage.
     */
    bool inside(int x, int y) const;
    /**
     * Provides the type of tile found at the location that is provided as an argument. This location should be inside
     * of the tilemap.
     */
    const Tile& getTile(int x, int y) const;
    /**
     * Returns the variant of the tile found at the location that is provided as an argument. This location should be
     * inside of the tilemap.
     */
    std::uint8_t getVariant(int x, int y) const;
    /**
     * Changes the tile type and variant at a location. This location should be inside of the tilemap.
     */
    void set(int x, int y, const Tile& t, std::uint8_t variant);

    /**
     * Changes the size of the tile storage to the new provided side. If this is smaller than the previous storage, the
     * excess tiles are discarded. If the resulting TileStorage is larger, the provided tile type will be used to fill
     * the new positions.
     *
     * @param width The new width.
     * @param height The new height.
     * @param fill The tile type to use when there is new empty space to fill.
     */
    void resize(int width, int height, const Tile& fill, std::uint8_t variant);

private:
    const std::uint16_t MASK = 0b11111111;
    const std::uint16_t MASK_LEN = 8;

    inline std::size_t posId(int x, int y) const;

    int width = 0;
    int height = 0;

    Tileset palette;
    std::vector<std::uint16_t> content;
};

/**
 * A tilemap is an entity which is made up of different squares called tiles. Usually, it is used to represent the
 * environment of a level.
 * Tiles can have hitboxes
 */
class Tilemap : public Entity {
public:
    Tilemap(std::unique_ptr<Viewer> viewer, const math::Transform& transform, std::shared_ptr<const TileStorage> tiles);

    void tick(World& world) override;

    /**
     * Returns the underlying storage for the tilemap, which contains all tiles.
     */
    std::shared_ptr<const TileStorage> getStorage() const;

    /**
     * Returns the collider of the tile at the provided position if there would be a non-air tile. Does not check if the
     * tilemap actually has a tile at this position, or if the position is a valid position in the tilemap. The returned
     * coordinates are translated to the tilemap's position.
     *
     * @param x The x coordinate of the tile.
     * @param y The y coordinate of the tile.
     * @return An axis-aligned bounding box representing the collider of the tile at coordinates (X, Y) in the tilemap.
     */
    math::Collider getTileCollider(int x, int y) const;

private:
    void recalculateHitbox();

    std::shared_ptr<const TileStorage> storage;
};

} // namespace world

#endif // GAME_TILEMAP_H
