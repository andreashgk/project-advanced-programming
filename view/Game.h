#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <SFML/Graphics.hpp>

#include "state/Manager.h"

namespace view {

/**
 * Wraps all the components of the game.
 */
class Game {
public:
    Game();

    /**
     * Does everything necessary in order to run the game. It contains the main game loop.
     */
    void run();

private:
    sf::RenderWindow window;
    state::Manager stateManager;
};

} // namespace view

#endif // GAME_GAME_H
