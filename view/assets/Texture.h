#ifndef GAME_TEXTURE_H
#define GAME_TEXTURE_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace assets {

class Texture {
public:
    static std::string getName();

    Texture(const Texture&) = delete;
    Texture(Texture&&) = default;

    static std::map<std::string, Texture> findAll();

    const sf::Texture& get() const;

private:
    Texture() = default;
    sf::Texture tx;
};

} // namespace assets

#endif // GAME_TEXTURE_H
