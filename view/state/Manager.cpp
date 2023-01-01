#include "Manager.h"

#include <stdexcept>

namespace state {

Manager::Manager(std::unique_ptr<State> initialState) { stateStack.emplace_back(std::move(initialState)); }

void Manager::tick() {
    // This should not happen.
    if (stateStack.empty())
        throw std::runtime_error("State stack is empty");

    stateStack[stateStack.size() - 1]->tick(*this);
    if (changed) {
        changed = false;
        return;
    }
    for (auto& state : stateStack) {
        state->draw();
    }
}

void Manager::set(std::unique_ptr<State> state) {
    changed = true;
    stateStack.clear();
    stateStack.emplace_back(std::move(state));
}

void Manager::push(std::unique_ptr<State> state) { stateStack.emplace_back(std::move(state)); }

void Manager::pop() {
    if (stateStack.size() <= 1)
        throw std::domain_error("Cannot pop the only state from the state stack");
    changed = true;
    stateStack.pop_back();
}

void Manager::handleEvent(sf::Event event) {
    stateStack[stateStack.size() - 1]->handleEvent(*this, event);

    // We only want event handler on the top of the stateStack to receive input events, so if this event is such an
    // event, we can already return.
    switch (event.type) {
    case sf::Event::Closed:
    case sf::Event::Resized:
    case sf::Event::LostFocus:
    case sf::Event::GainedFocus:
        for (auto it = stateStack.end() - 2; it >= stateStack.begin(); --it) {
            (*it)->handleEvent(*this, event);
        }
        break;
    default:
        return;
    }
}

} // namespace state