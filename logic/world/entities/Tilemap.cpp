#include "Tilemap.h"

#include <cmath>
#include <stdexcept>
#include <string>

namespace world {

Tilemap::Tilemap(std::unique_ptr<Viewer> viewer, const math::Transform& transform,
                 std::shared_ptr<const TileStorage> tiles)
    : Entity(std::move(viewer), transform, 0), storage(std::move(tiles)) {
    recalculateHitbox();
}

void Tilemap::tick(World& world) {}

math::Collider Tilemap::getTileCollider(int x, int y) const {
    return storage->getTile(x, y).getCollider() + math::Vector2(static_cast<float>(x), -static_cast<float>(y)) +
           getTransform().translation;
}

void Tilemap::recalculateHitbox() {
    std::vector<math::AABB> boxes;
    std::size_t size = 0;
    for (int x = 0; x < storage->getWidth(); x++) {
        for (int y = 0; y < storage->getHeight(); y++) {
            size += storage->getTile(x, y).getCollider().getContent().size();
        }
    }
    boxes.reserve(size);
    for (int x = 0; x < storage->getWidth(); x++) {
        for (int y = 0; y < storage->getHeight(); y++) {
            for (const auto& box : storage->getTile(x, y).getCollider().getContent()) {
                boxes.emplace_back((box + math::Vector2(static_cast<float>(x) - float(storage->getWidth()) / 2.f,
                                                        static_cast<float>(y))));
            }
        }
    }
    setRelativeCollider(math::Collider(boxes));
}

std::shared_ptr<const TileStorage> Tilemap::getStorage() const { return storage; }

// Tile
// ----

Tile::Tile(std::string name, math::Collider collider) : name(std::move(name)), collider(std::move(collider)) {}

const std::string& Tile::getName() const { return name; }

const math::Collider& Tile::getCollider() const { return collider; }

// Tileset
// -------

Tileset::Tileset(std::initializer_list<Tile> tiles) : Tileset({tiles.begin(), tiles.end()}) {}

Tileset::Tileset(std::vector<Tile> _tiles) : tiles(std::move(_tiles)) {
    if (tiles.size() > std::numeric_limits<std::uint8_t>::max())
        throw std::overflow_error("Expected at most " + std::to_string(std::numeric_limits<std::uint8_t>::max()) +
                                  "tiles");
    for (auto it = tiles.begin(); it < tiles.end(); it++) {
        if (!tileLookup.insert({it->getName(), static_cast<std::uint16_t>(it - tiles.begin())}).second) {
            throw std::domain_error("All tiles in a tileset should have unique names");
        }
    }
}

const Tile& Tileset::getTile(std::uint8_t id) const { return tiles.at(static_cast<std::size_t>(id)); }

std::uint8_t Tileset::getIndex(const Tile& tile) const { return getIndex(tile.getName()); }

std::uint8_t Tileset::getIndex(const std::string& tileName) const {
    auto it = tileLookup.find(tileName);
    if (it == tileLookup.end()) {
        throw std::out_of_range("Unknown tile type with name " + tileName);
    }
    return it->second;
}

std::size_t Tileset::getSize() const { return tiles.size(); }

// TileStorage
// -----------

TileStorage::TileStorage(Tileset palette) : palette(std::move(palette)) {}

void TileStorage::resize(int newWidth, int newHeight, const Tile& fill, std::uint8_t variant) {
    std::uint16_t index = static_cast<std::uint16_t>((static_cast<std::uint16_t>(variant) << MASK_LEN) |
                                                     static_cast<std::uint16_t>(palette.getIndex(fill)));
    std::vector<std::uint16_t> n(static_cast<std::size_t>(newHeight * newWidth), index);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            n[static_cast<std::size_t>(x + y * newWidth)] = content[posId(x, y)];
        }
    }
    auto r = std::move(content);
    content = std::move(n);
    height = newHeight;
    width = newWidth;
}

const Tile& TileStorage::getTile(int x, int y) const {
    return palette.getTile(static_cast<std::uint8_t>(content[posId(x, y)] & MASK));
}

std::uint8_t TileStorage::getVariant(int x, int y) const {
    return static_cast<std::uint8_t>((content[posId(x, y)] >> MASK_LEN) & MASK);
}

void TileStorage::set(int x, int y, const Tile& t, std::uint8_t variant) {
    content[posId(x, y)] = static_cast<std::uint16_t>(static_cast<std::uint16_t>(palette.getIndex(t)) |
                                                      (static_cast<std::uint16_t>(variant) << MASK_LEN));
}

bool TileStorage::inside(int x, int y) const { return x >= 0 && y >= 0 && x < width && y < height; }

int TileStorage::getWidth() const { return width; }
int TileStorage::getHeight() const { return height; }

const Tileset& TileStorage::getPalette() const { return palette; }

inline std::size_t TileStorage::posId(int x, int y) const {
    if (!inside(x, y))
        throw std::out_of_range("tilemap location (" + std::to_string(x) + ", " + std::to_string(y) +
                                ") is out of range");
    return static_cast<size_t>(x + y * width);
}

} // namespace world
