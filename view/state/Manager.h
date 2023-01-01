#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "State.h"

namespace state {

/**
 * Manages all the current game states. It is a stack of states, where all states are drawn (from the bottom of the
 * stack to the top), but only the logic for the state on top of the stack is executed.
 * This stack can allow for paused states which still show the game world behind it.
 */
class Manager {
public:
    explicit Manager(std::unique_ptr<State> initialState);

    /**
     * Called every frame to update the state. This is for example used for the game loop, but also to render the state
     * to the window. Only the state on the top of the stack will actually be ticked, but all states will be drawn to
     * the screen, starting with the state first on the stack and ending with the state on the top of the stack.
     */
    void tick();
    /**
     * Allows states to handle different SFML events. The state on the top of the stack will receive every event, but
     * other states on the stack will only receive window-related events such as resizing, and no input events.
     *
     * @param event The event to be handled.
     */
    void handleEvent(sf::Event event);

    /**
     * Pops every state and replaces it with a single new state.
     *
     * @param state The state to push onto the stack after all states have been popped.
     */
    void set(std::unique_ptr<State> state);
    /**
     * Pushes a state onto the top of the state stack.
     *
     * @param state
     */
    void push(std::unique_ptr<State> state);
    /**
     * Removes the top state on the state stack. There needs to be at least 1 state left. If this is not the case and
     * the stack becomes empty, an exception is thrown and the pop does not succeed.
     */
    void pop();

private:
    // Do not use std::stack, as we need to be able to access every element in the stack.
    std::vector<std::unique_ptr<State>> stateStack;
    bool changed = false;
};

} // namespace state

#endif // GAME_MANAGER_H
