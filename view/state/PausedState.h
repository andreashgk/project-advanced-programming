#ifndef GAME_PAUSEDSTATE_H
#define GAME_PAUSEDSTATE_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "../assets/Font.h"
#include "../ui/Element.h"
#include "State.h"

namespace state {

/**
 * A paused state which overlays the state before it.
 */
class PausedState : public State {
public:
    explicit PausedState(sf::RenderWindow& window);

    void draw() override;
    void tick(Manager& stateManager) override;
    void handleEvent(Manager& stateManager, sf::Event event) override;

private:
    sf::RenderWindow& window;
    bool released = false;

    const sf::Font& uiFont;
};

} // namespace state

#endif // GAME_PAUSEDSTATE_H
