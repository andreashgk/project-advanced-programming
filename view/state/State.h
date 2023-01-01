#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SFML/Graphics.hpp>

namespace state {

class Manager;

class State {
public:
    virtual ~State() = default;

    /**
     * Draws this state onto the screen.
     */
    virtual void draw() = 0;
    /**
     * Tick is called every frame, and is meant to update the state's data. No actual drawing happens here.
     *
     * @param stateManager The state::Manager that owns this state. Can be used to pop, push or replace states.
     */
    virtual void tick(Manager& stateManager) = 0;
    /**
     * Handles SFML incoming events. When this state is not on the top of the state stack, only non-input related events
     * will be passes through to this function.
     *
     * @param event The incoming event.
     */
    virtual void handleEvent(Manager& stateManager, sf::Event event) = 0;
};

} // namespace state

#endif // GAME_STATE_H
