#ifndef GAME_TILESET_H
#define GAME_TILESET_H

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <map>
#include <string>

#include "../../logic/math/Vector2.h"
#include "../../logic/world/entities/Tilemap.h"

namespace assets {

class Tileset {
public:
    static std::string getName();

    Tileset(const Tileset&) = delete;
    Tileset(Tileset&&) = default;
    static std::map<std::string, Tileset> findAll();

    const sf::Texture& getTexture() const;
    sf::IntRect getRect(const std::string& id, std::uint8_t variant) const;

    bool contains(const std::string& id, std::uint8_t variant) const;

private:
    Tileset() = default;

    int size{};
    sf::Texture tx;
    std::map<std::string, std::map<std::uint8_t, sf::Vector2i>> tiles;
};

} // namespace assets

#endif // GAME_TILESET_H
