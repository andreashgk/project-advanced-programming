#include "Game.h"

#include "../logic/utils/Stopwatch.h"
#include "state/MenuState.h"

namespace view {

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Fruit Boy"),
      stateManager(std::move(std::make_unique<state::MenuState>(state::MenuState(window)))) {}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                // Update the view of the window to match the new resolution. This will allow it's content to scale
                // properly.
                window.setView({{float(window.getSize().x) / 2, float(window.getSize().y) / 2},
                                {float(window.getSize().x), float(window.getSize().y)}});
                // Intentionally no `break;`!
            case sf::Event::LostFocus:
                // Slight hack: calculate the delta twice if we resize the window or re-gain focus, the game loop will
                // have been frozen for a while, causing a very large deltatime. This could mess up physics such as
                // collisions.
                utils::Stopwatch::get().nextFrame();
                break;
            default:
                break;
            }
            stateManager.handleEvent(event);
        }

        utils::Stopwatch::get().nextFrame();

        window.clear();
        stateManager.tick();
        window.display();
    }
}

} // namespace view
