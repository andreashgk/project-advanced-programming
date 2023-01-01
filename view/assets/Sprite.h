#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace assets {

class Sprite {
public:
    static std::string getName();

    class State {
    public:
        State(const sf::Texture& tx, int frame_delta, int size, int duration);
        State(State&&) = default;
        State(const State&) = default;

        int getDuration() const;
        const sf::Texture& getTexture() const;
        sf::IntRect getFrame(int timeSinceStart, bool mirror = false) const;
        sf::IntRect getFrame(float timeSinceStart, bool mirror = false) const;

    private:
        sf::Texture tx{};
        int frame_delta;
        int duration;
        int size;
        int frame_count;
    };

    Sprite(Sprite&&) = default;
    Sprite(const Sprite&) = delete;
    static std::map<std::string, Sprite> findAll();

    const State& getState(const std::string& name) const;

private:
    Sprite() = default;

    std::map<std::string, State> states{};
};

} // namespace assets

#endif // GAME_SPRITE_H
