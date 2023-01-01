#include "PausedState.h"
#include "../assets/AssetServer.h"
#include "../ui/Label.h"
#include "../ui/List.h"
#include "../ui/Margin.h"
#include "../ui/utils.h"
#include "Manager.h"

namespace state {

PausedState::PausedState(sf::RenderWindow& window)
    : window(window), uiFont(assets::AssetServer::get().get<assets::Font>("november.ttf")) {}

void PausedState::draw() {
    sf::RectangleShape r;
    r.setPosition(0, 0);
    r.setSize({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
    r.setFillColor(sf::Color(50, 50, 50, 100));
    window.draw(r);

    sf::Text titleText = {"Paused", uiFont, static_cast<unsigned int>(scale(window.getSize().y, 1.8f))};
    titleText.setOutlineThickness(scale(window.getSize().y, 0.08f));
    auto title = std::make_shared<ui::Label>(ui::Label(titleText));
    sf::Text subTitleText = {"Press esc to continue", uiFont,
                             static_cast<unsigned int>(scale(window.getSize().y, 1.f))};
    subTitleText.setOutlineThickness(scale(window.getSize().y, 0.06f));
    auto subTitle = std::make_shared<ui::Label>(ui::Label(subTitleText));

    std::vector<std::shared_ptr<ui::Element>> v;
    v.emplace_back(std::move(std::make_shared<ui::Margin>(ui::Margin(title, 0, 0))));
    v.emplace_back(std::move(std::make_shared<ui::Margin>(ui::Margin(subTitle, 0, scale(window.getSize().y, 0.55f)))));
    ui::List ui(std::move(v), ui::List::Align::Center);
    ui.draw({static_cast<float>(window.getSize().x) / 2.f - ui.getSize().x / 2.f,
             static_cast<float>(window.getSize().y) / 2.f - ui.getSize().y / 2.f},
            window);
}

void PausedState::tick(Manager& stateManager) {
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        released = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && released)
        stateManager.pop();
}

void PausedState::handleEvent(Manager& stateManager, sf::Event event) {}

} // namespace state