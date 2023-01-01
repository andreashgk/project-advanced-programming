#ifndef GAME_LOADER_H
#define GAME_LOADER_H

#include "../../logic/world/World.h"

namespace level {

/**
 * An error thrown while trying to create a world from a level. When this error is thrown, it usually indicates that the
 * data in the level file might be malformed.
 */
class LoadError : public std::runtime_error {
public:
    LoadError(const std::string& message);
    ~LoadError() override = default;
};

/**
 * Abstract class that can be derived from to specify a level format.
 */
class Loader {
public:
    virtual ~Loader() = default;

    /**
     * Returns the display name of the level shown in the level selector. May be empty if the level was not loaded
     * successfully.
     */
    virtual const std::string& getName() = 0;

    /**
     * Returns true if the level file has been loaded successfully. If this is false, an error has happened and the
     * Loader::load() function cannot be used.,
     */
    virtual bool isLoaded() const = 0;

    /**
     * Loads the level specification into a playable game world.
     *
     * @param entityFactory The world::EntityFactory used to create entities.
     * @param cameraBounds The starting bounds of the camera object.
     * @throws LoadError If there was a problem trying to load the world.
     */
    virtual world::World load(std::shared_ptr<world::EntityFactory> entityFactory, math::AABB cameraBounds) const = 0;
};

} // namespace level

#endif // GAME_LOADER_H
