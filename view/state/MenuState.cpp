#include "MenuState.h"

#include <cmath>

#include "../assets/AssetServer.h"
#include "../assets/Texture.h"
#include "../ui/Label.h"
#include "../ui/List.h"
#include "../ui/Margin.h"
#include "../ui/utils.h"
#include "GameState.h"
#include "Manager.h"

namespace state {

// The menu does not actually have pages, but this is equivalent to the maximum amount of entries to show on the screen
// at one time.
const int ENTRIES_PER_PAGE = 5;

MenuState::MenuState(sf::RenderWindow& window)
    : uiFont(assets::AssetServer::get().get<assets::Font>("november.ttf")), window(window),
      levels(assets::AssetServer::get().getAll<assets::Level>()) {
    pageStart = levels.begin();
    pageEnd = std::next(levels.begin(), std::min(ENTRIES_PER_PAGE, static_cast<int>(levels.size())));
    selected = levels.begin();
}

void MenuState::draw() {
    // Draw the background.
    const sf::Texture& background = assets::AssetServer::get().get<assets::Texture>("bg_green.png").get();
    sf::RectangleShape bgRect({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
    bgRect.setTexture(&background);
    bgRect.setTextureRect(
        {0, 0,
         static_cast<int>(static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y) * 4.f *
                          static_cast<float>(background.getSize().y)),
         static_cast<int>(background.getSize().y) * 4});
    window.draw(bgRect);

    // Create the UI.
    sf::Text title = {"Fruit Boy", uiFont, static_cast<unsigned int>(scale(window.getSize().y, 1.75f))};
    title.setOutlineThickness(scale(window.getSize().y, 0.08f));
    title.setFillColor(sf::Color(255, 100, 135));

    std::vector<std::shared_ptr<ui::Element>> mainElems;
    mainElems.emplace_back(std::make_shared<ui::Margin>(ui::Margin(
        std::make_shared<ui::Label>(ui::Label(title)), 32, static_cast<float>(scale(window.getSize().y, 0.50f)))));

    std::vector<std::shared_ptr<ui::Element>> elems;

    for (auto it = pageStart; it != pageEnd; it++) {
        if (it == selected) {
            sf::Text tx = {"> " + it->second.getLoader()->getName() + "  ", uiFont,
                           static_cast<unsigned int>(scale(window.getSize().y, 1.1f))};
            tx.setFillColor(sf::Color(255, 150, 165));
            tx.setOutlineThickness(scale(window.getSize().y, 0.06f));
            elems.emplace_back(std::make_shared<ui::Margin>(
                ui::Margin(std::make_shared<ui::Label>(ui::Label(tx)), 32, scale(window.getSize().y, 0.42f))));
            continue;
        }

        sf::Text tx = {"  " + it->second.getLoader()->getName() + "  ", uiFont,
                       static_cast<unsigned int>(scale(window.getSize().y))};
        tx.setFillColor(sf::Color(220, 220, 220));
        tx.setOutlineThickness(scale(window.getSize().y, 0.05f));
        elems.emplace_back(std::make_shared<ui::Margin>(ui::Margin(
            std::make_shared<ui::Label>(ui::Label(tx)), 32, static_cast<float>(scale(window.getSize().y, 0.42f)))));
    }
    mainElems.emplace_back(std::make_shared<ui::List>(std::move(elems), ui::List::Align::Center));
    auto list = std::make_unique<ui::List>(ui::List(std::move(mainElems), ui::List::Align::Center));

    list->draw({static_cast<float>(window.getSize().x) / 2.f - list->getSize().x / 2.f,
                std::max(static_cast<float>(window.getSize().y) / 12.f,
                         static_cast<float>(window.getSize().y) / 2.f - list->getSize().y / 1.7f)},
               window);
}

void MenuState::tick(Manager& stateManager) {}

void MenuState::handleEvent(Manager& stateManager, sf::Event event) {
    if (event.type != sf::Event::KeyPressed) {
        return;
    }
    switch (event.key.code) {
    case sf::Keyboard::PageUp:
    case sf::Keyboard::Up:
    case sf::Keyboard::W:
        if (selected == levels.begin()) {
            return;
        }

        selected--;
        if (selected == pageStart && pageStart != levels.begin()) {
            pageStart--;
            pageEnd--;
        }
        break;
    case sf::Keyboard::PageDown:
    case sf::Keyboard::Down:
    case sf::Keyboard::S:
        if (selected == std::prev(levels.end()))
            return;
        selected++;
        if (selected == std::prev(pageEnd) && pageEnd != levels.end()) {
            pageStart++;
            pageEnd++;
        }
        break;
    case sf::Keyboard::Enter:
    case sf::Keyboard::Space: {
        try {
            stateManager.set(
                std::move(std::make_unique<state::GameState>(state::GameState(selected->second, window))));
        } catch (const level::LoadError& e) {
            std::cerr << "Could not load level `" + selected->second.getLoader()->getName() + "`." << std::endl;
            std::cerr << e.what() << std::endl;
        }
    } break;
    default:
        return;
    }
}

} // namespace state