#ifndef GAME_GAMESTATE_H
#define GAME_GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "../../logic/world/World.h"
#include "../assets/Level.h"
#include "../level/Loader.h"
#include "../particle/Particle.h"
#include "../ui/Element.h"
#include "../ui/Label.h"
#include "State.h"

namespace state {

/**
 * The GameState is the main state of the game, which contains a game world and runs the game logic.
 */
class GameState : public State {
public:
    /**
     * Loads a level into a playable GameState. Will throw level::LoadError if the level could not be loaded
     * successfully.
     */
    GameState(const assets::Level& loader, sf::RenderWindow& window);

    void tick(Manager& stateManager) override;
    void draw() override;
    void handleEvent(Manager& stateManager, sf::Event event) override;

private:
    const assets::Level& level;

    float timeSinceFinish = 0;
    float timeSinceStart = 0;
    float timeSinceLastSubtractScore = 0;

    world::World world;
    sf::RenderWindow& window;

    std::unique_ptr<sf::Texture> backgroundTx;
    sf::RectangleShape background;

    bool escReleased = true;

    std::unique_ptr<ui::Element> ui;

    std::shared_ptr<ui::Label> scoreCounter;
    std::shared_ptr<ui::Label> timeCounter;
};

} // namespace state

#endif // GAME_GAMESTATE_H
