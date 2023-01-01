#include "Texture.h"

#include <filesystem>

namespace assets {

std::string Texture::getName() { return "Texture"; }

std::map<std::string, Texture> Texture::findAll() {
    std::map<std::string, Texture> map;
    for (const auto& entry : std::filesystem::directory_iterator("./assets/textures")) {
        Texture tx;
        if (!tx.tx.loadFromFile(entry.path().string())) {
            continue;
        }
        tx.tx.setRepeated(true);
        map.insert({entry.path().filename().string(), std::move(tx)});
    }
    return map;
}

const sf::Texture& Texture::get() const { return tx; }

} // namespace assets
