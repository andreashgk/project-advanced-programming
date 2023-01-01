#include "Font.h"

#include <filesystem>

namespace assets {

std::string Font::getName() { return "Font"; }

std::map<std::string, Font> Font::findAll() {
    std::map<std::string, Font> fonts;

    for (const auto& entry : std::filesystem::directory_iterator("./assets/fonts")) {
        Font font;
        if (!font.loadFromFile(entry.path().string())) {
            continue;
        }
        fonts.insert({entry.path().filename().string(), std::move(font)});
    }
    return fonts;
}

} // namespace assets