#include "Tileset.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace assets {

std::string Tileset::getName() { return "Tileset"; }

std::map<std::string, Tileset> Tileset::findAll() {
    std::map<std::string, Tileset> map;
    for (const auto& entry : std::filesystem::directory_iterator("./assets/tilesets")) {
        if (entry.path().extension() == ".yml")
            continue;
        std::string filename = entry.path().string();

        Tileset tileset;
        if (!tileset.tx.loadFromFile(entry.path().string())) {
            continue;
        }
        std::string configPath = filename.substr(0, filename.find_last_of('.')) + ".yml";
        try {
            YAML::Node tilesetCfg = YAML::LoadFile(configPath);

            tileset.size = tilesetCfg["tile_size"].as<int>();
            if (tileset.size <= 0) {
                throw std::domain_error("tile_size must be greater than 0");
            }

            YAML::Node tiles = tilesetCfg["tiles"];
            for (const auto& tile : tiles) {
                auto id = tile.first.as<std::string>();

                std::map<std::uint8_t, sf::Vector2i> tileVariants;
                for (const auto& variant : tile.second) {
                    auto variantId = variant.first.as<std::uint8_t>();
                    auto tilePos = variant.second.as<std::vector<int>>();
                    if (tilePos.size() != 2) {
                        throw std::domain_error(
                            "A position of a tile texture in the tileset must have exactly 2 components");
                    }

                    if (tilePos[0] < 0 || tilePos[0] >= tileset.tx.getSize().x || tilePos[1] < 0 ||
                        tilePos[1] >= tileset.tx.getSize().y) {
                        throw std::domain_error("Tile `" + id + "` position on texture `[" +
                                                std::to_string(tilePos[0]) + ", " + std::to_string(tilePos[1]) +
                                                "]` is out of range");
                    }
                    tileVariants.insert({variantId, {tilePos[0], tilePos[1]}});
                }
                tileset.tiles.insert({id, tileVariants});
            }
        } catch (std::exception& e) {
            std::cerr << "Could not parse \"" << configPath << "\": " << e.what() << std::endl;
            continue;
        }
        map.insert({entry.path().filename().string(), std::move(tileset)});
    }
    return map;
}

sf::IntRect Tileset::getRect(const std::string& id, std::uint8_t variant) const {
    auto it = tiles.find(id);
    if (it == tiles.end())
        throw std::out_of_range("Could not find tile with ID `" + id + "`");
    auto it2 = it->second.find(variant);
    if (it2 == it->second.end())
        throw std::out_of_range("Could not find variant " + std::to_string(variant) + " tile with ID `" + id + "`");
    return {it2->second.x, it2->second.y, size, size};
}

const sf::Texture& Tileset::getTexture() const { return tx; }

bool Tileset::contains(const std::string& id, std::uint8_t variant) const {
    auto it = tiles.find(id);
    if (it == tiles.end())
        return false;
    return it->second.find(variant) != it->second.end();
}

} // namespace assets