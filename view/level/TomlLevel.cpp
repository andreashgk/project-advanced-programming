#include "TomlLevel.h"

#include <cmath>
#include <iostream>
#include <toml.hpp>

#include "../assets/AssetServer.h"
#include "../assets/Tileset.h"
#include "../viewer/TilemapViewer.h"

namespace level {

TomlLevel::TomlLevel(std::istream& istream, std::string filename) {
    toml::value local_table;
    try {
        local_table = std::move(toml::parse(istream, std::move(filename)));
        name = toml::find<std::string>(local_table, "name");
        if (name.size() < 3 || name.size() > 26) {
            std::cerr << toml::format_error(
                             "[error] level name should be between 3 and 16 characters", local_table.at("name"),
                             "the actual level name is " + std::to_string(name.size()) + " characters long", {}, true)
                      << std::endl;
            return;
        }
    } catch (const toml::exception& err) {
        std::cerr << err.what() << std::endl;
        return;
    } catch (const std::out_of_range& err) {
        std::cerr << err.what() << std::endl;
        return;
    }
    table_opt = std::move(local_table);
}

bool TomlLevel::isLoaded() const { return table_opt.has_value(); }

// Helper function to facilitate the loading of keys of certain types. Will throw appropriate errors if a key has the
// wrong type.
template <class T>
T parse(const toml::value& val);

template <>
math::Vector2 parse<math::Vector2>(const toml::value& val) {
    if (val.as_array().size() != 2) {
        throw toml::type_error(toml::format_error("[error] position should contain exactly two components", val,
                                                  std::to_string(val.as_array().size()) + " components found here", {},
                                                  true),
                               val.location());
    }
    return {toml::find<float>(val, 0), toml::find<float>(val, 1)};
}

const std::string& TomlLevel::getName() { return name; }

world::World TomlLevel::load(std::shared_ptr<world::EntityFactory> entityFactory, math::AABB cameraBounds) const try {
    if (!table_opt.has_value()) {
        throw LoadError("Level was not loaded successfully.");
    }
    const toml::value& table = table_opt.value();

    toml::value cameraTable = table.at("camera");
    world::Camera camera(parse<math::Vector2>(toml::find(cameraTable, "pos")), cameraBounds,
                         parse<math::Vector2>(toml::find(cameraTable, "destination")),
                         toml::find<float>(cameraTable, "speed"));

    world::World world(entityFactory, camera);
    std::string tilesetName;
    {
        toml::value tilemapConfig = toml::find(table, "tilemap_config");

        tilesetName = toml::find<std::string>(tilemapConfig, "tileset");

        if (!assets::AssetServer::get().contains<assets::Tileset>(tilesetName)) {
            throw LoadError(toml::format_error("[error] tileset does not exist", tilemapConfig.at("tileset"),
                                               "defined here", {}, true));
        }
        std::string defaultTile = toml::find<std::string>(tilemapConfig, "default_tile");
        std::uint8_t defaultVariant = toml::find<std::uint8_t>(tilemapConfig, "default_variant");

        world.getResources().insert(viewer::TilemapViewConfig(tilesetName, defaultTile, defaultVariant));
    }

    auto& entities = toml::find<toml::array>(table, "entity");
    for (const toml::value& entity : entities) {

        const std::string& entityType = toml::find<std::string>(entity, "type");
        math::Transform transform(parse<math::Vector2>(toml::find(entity, "pos")),
                                  entity.contains("scale") ? parse<math::Vector2>(toml::find(entity, "scale"))
                                                           : math::Vector2(1., 1.));

        if (entityType == "player") {
            world.spawnEntity(entityFactory->createPlayer(), transform);
        } else if (entityType == "melon") {
            world.spawnEntity(entityFactory->createMelon(toml::find_or<int>(entity, "score", 100)), transform);
        } else if (entityType == "finish") {
            world.spawnEntity(entityFactory->createFinish(), transform);
        } else if (entityType == "tilemap") {

            const toml::value& tilemapData = entity.at("tilemap");
            int width = toml::find<int>(tilemapData, "width");
            int height = toml::find<int>(tilemapData, "height");
            if (width <= 0) {
                throw toml::type_error(toml::format_error("[error] tilemap dimensions should be greater than zero",
                                                          tilemapData.at("width"), "incorrect width found here", {},
                                                          true),
                                       tilemapData.at("width").location());
            }
            if (height <= 0) {
                throw toml::type_error(toml::format_error("[error] tilemap dimensions should be greater than zero",
                                                          tilemapData.at("height"), "incorrect height found here", {},
                                                          true),
                                       tilemapData.at("height").location());
            }

            std::vector<world::Tile> paletteContent;
            for (const toml::value& paletteEntry : tilemapData.at("palette").as_array()) {

                const std::string& name = toml::find<std::string>(paletteEntry, "name");
                std::vector<math::AABB> collider;

                if (paletteEntry.contains("hitbox")) {
                    for (const toml::value& box : paletteEntry.at("hitbox").as_array()) {
                        if (box.as_array().size() != 4) {
                            throw LoadError(toml::format_error(
                                "[error] bounding box should contain exactly four components", box,
                                std::to_string(box.as_array().size()) + " components found here", {}, true));
                        }

                        collider.emplace_back(math::AABB({toml::find<float>(box, 0), toml::find<float>(box, 1)},
                                                         {toml::find<float>(box, 2), toml::find<float>(box, 3)}));
                    }
                }
                paletteContent.emplace_back(name, std::move(collider));
            }
            world::TileStorage tileStorage(world::Tileset(std::move(paletteContent)));
            const world::Tileset& tileset = tileStorage.getPalette();
            tileStorage.resize(width, height, tileset.getTile(0), 0);

            std::vector<std::string> content = toml::find<std::vector<std::string>>(tilemapData, "content");
            if (content.size() != width * height) {
                throw LoadError(toml::format_error(
                    "[error] content array size should be equal to width*height", tilemapData.at("content"),
                    std::to_string(content.size()) + " components found here, expected " +
                        std::to_string(width * height),
                    {}, true));
            }
            int x = 0;
            int y = 0;
            const auto& tilesetAsset = assets::AssetServer::get().get<assets::Tileset>(tilesetName);
            try {
                for (const auto& tile : content) {
                    if (std::count(tile.begin(), tile.end(), ':') != 1) {
                        throw LoadError("tile entry should contain exactly one `:` character");
                    }
                    // Helper function to check if a string can be converted to a positive integer.
                    auto isNumerical = [](const std::string& s) -> bool {
                        for (auto it = s.begin(); it < s.end(); it++) {
                            if (!std::isdigit(*it))
                                return false;
                        }
                        return true;
                    };

                    std::string id = tile.substr(0, tile.find(':'));
                    std::string variant = tile.substr(tile.find(':') + 1, tile.size() - (tile.find(':') + 1));

                    if (!isNumerical(id) || id.empty()) {
                        throw LoadError("the palette index should be numerical");
                    }
                    int idNum = std::stoi(id);
                    if (idNum < 0 || idNum > std::numeric_limits<std::uint8_t>::max()) {
                        throw LoadError("the palette index should be a whole number between 0 and " +
                                        std::to_string(std::numeric_limits<std::uint8_t>::max()));
                    }

                    if (!isNumerical(variant) || variant.empty()) {
                        throw LoadError("the variant number should be numerical");
                    }
                    int variantNum = std::stoi(variant);
                    if (variantNum < 0 || variantNum > std::numeric_limits<std::uint8_t>::max()) {
                        throw LoadError("the variant number should be a whole number between 0 and " +
                                        std::to_string(std::numeric_limits<std::uint8_t>::max()));
                    }

                    if (idNum >= tileset.getSize()) {
                        throw LoadError("tilemap palette does not contain a tile with id `" + std::to_string(idNum) +
                                        "`");
                    }
                    // Make sure the tile exists.
                    const world::Tile& t = tileset.getTile(static_cast<uint8_t>(std::stoi(id)));
                    if (!tilesetAsset.contains(t.getName(), static_cast<uint8_t>(variantNum))) {
                        throw LoadError("tileset does not contain a tile `" + t.getName() + "` with variant `" +
                                        std::to_string(variantNum) + "`");
                    }

                    tileStorage.set(x, y, t, static_cast<uint8_t>(std::stoi(variant)));
                    x++;
                    if (x == width) {
                        x = 0;
                        y++;
                    }
                }
            } catch (LoadError& e) {
                throw LoadError(toml::format_error("[error] could not load tilemap content",
                                                   tilemapData.at("content").at(static_cast<size_t>(x + width * y)),
                                                   "incorrect tile entry found here", {e.what()}, true));
            }

            world.spawnEntity(
                entityFactory->createTilemap(std::make_shared<world::TileStorage>(std::move(tileStorage))), transform);
        } else {
            throw LoadError(toml::format_error("[error] unknown entity type `" + entityType + "`", entity.at("type"),
                                               "found here", {}, true));
        }
    }
    return world;
} catch (toml::type_error& err) {
    throw LoadError(err.what());
} catch (std::out_of_range& err) {
    throw LoadError(err.what());
}

LoadError::LoadError(const std::string& message) : std::runtime_error(message) {}

} // namespace level
