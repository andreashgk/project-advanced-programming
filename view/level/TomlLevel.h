#ifndef GAME_TOMLLEVEL_H
#define GAME_TOMLLEVEL_H

#define TOML11_COLORIZE_ERROR_MESSAGE

#include <optional>
#include <stdexcept>
#include <string>
#include <toml.hpp>

#include "../../logic/world/World.h"
#include "Loader.h"

namespace level {

class TomlLevel : public Loader {
public:
    TomlLevel(std::istream& istream, std::string filename = "unknown file");

    const std::string& getName() override;

    bool isLoaded() const override;

    world::World load(std::shared_ptr<world::EntityFactory>, math::AABB cameraBounds) const override;

private:
    std::string name;
    std::optional<toml::value> table_opt;
};

} // namespace level

#endif // GAME_TOMLLEVEL_H
