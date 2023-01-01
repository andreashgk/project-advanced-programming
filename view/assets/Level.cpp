#include "Level.h"

#include <filesystem>
#include <fstream>

#include "../level/TomlLevel.h"

namespace assets {

std::string Level::getName() { return "Level"; }

std::map<std::string, Level> Level::findAll() {
    std::map<std::string, Level> levels;
    auto prevLevel = levels.end();

    for (const auto& entry : std::filesystem::directory_iterator("./assets/levels")) {
        if (entry.is_directory())
            continue;
        std::ifstream file;
        file.open(entry.path());
        if (file.bad()) {
            std::cerr << "Could not read level `" + entry.path().string() + "`." << std::endl;
        }

        // Temporarily store the previous highest level, so we can change its next level to the current level.
        auto oldLevel = prevLevel;

        std::string extension = entry.path().extension().string();
        if (extension == ".toml") {
            level::TomlLevel loader(file, entry.path().filename().string());
            if (!loader.isLoaded()) {
                std::cerr << "Could not parse level `" + entry.path().string() + "`." << std::endl;
                continue;
            }
            prevLevel = levels.insert({entry.path().filename().string(), Level(std::make_unique<level::TomlLevel>(loader))}).first;
        } else {
            std::cerr << "Unknown format type for level `" + entry.path().string() + "`." << std::endl;
        }
        file.close();

        if (oldLevel == levels.end())
            continue;
        oldLevel->second.nextLevel = entry.path().filename().string();
    }
    return levels;
}

const std::unique_ptr<level::Loader>& Level::getLoader() const { return loader; }

const std::optional<std::string>& Level::getNext() const { return nextLevel; }

Level::Level(std::unique_ptr<level::Loader> loader) : loader(std::move(loader)) {}

} // namespace assets
