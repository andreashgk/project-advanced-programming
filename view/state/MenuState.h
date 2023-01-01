#ifndef GAME_MENUSTATE_H
#define GAME_MENUSTATE_H

#include <SFML/Graphics.hpp>
#include <map>

#include "../assets/Font.h"
#include "../assets/Level.h"
#include "State.h"

namespace state {

/**
 * The main menu of the game. Contains a list of levels which can be selected and played.
 */
class MenuState : public State {
public:
    MenuState(sf::RenderWindow& window);

    void draw() override;
    void tick(Manager& stateManager) override;
    void handleEvent(Manager& stateManager, sf::Event event) override;

private:
    const assets::Font& uiFont;
    sf::RenderWindow& window;

    const std::map<std::string, assets::Level>& levels;
    // Iterators are fine here, as no levels will be added during runtime.
    std::map<std::string, assets::Level>::const_iterator pageStart;
    std::map<std::string, assets::Level>::const_iterator pageEnd;
    std::map<std::string, assets::Level>::const_iterator selected;
};

} // namespace state

#endif // GAME_MENUSTATE_H
