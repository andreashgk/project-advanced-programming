#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <memory>
#include <string>

#include "../level/Loader.h"

namespace assets {

/**
 * Wrapper around a level::Loader.
 */
class Level {
public:
    static std::string getName();

    static std::map<std::string, Level> findAll();

    const std::unique_ptr<level::Loader>& getLoader() const;

    /**
     * Returns the identifier of the next level, or a null_opt if this is the last level.
     * We cannot return the actual level, as reference optionals are not allowed in C++17.
     */
    const std::optional<std::string>& getNext() const;

private:
    Level(std::unique_ptr<level::Loader> loader);

    std::unique_ptr<level::Loader> loader;
    std::optional<std::string> nextLevel;
};

} // namespace assets

#endif // GAME_LEVEL_H
