#ifndef GAME_FONT_H
#define GAME_FONT_H

#include <SFML/Graphics/Font.hpp>
#include <map>
#include <string>

namespace assets {

class Font : public sf::Font {
public:
    static std::string getName();

    Font(const Font&) = delete;
    Font(Font&&) = default;

    static std::map<std::string, Font> findAll();

private:
    Font() = default;
};

} // namespace assets

#endif // GAME_FONT_H
