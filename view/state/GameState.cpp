#include "GameState.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <utility>

#include "../../logic/resources/GameStatus.h"
#include "../../logic/resources/Input.h"
#include "../../logic/resources/Score.h"
#include "../../logic/utils/Stopwatch.h"
#include "../SFMLFactory.h"
#include "../assets/AssetServer.h"
#include "../assets/Texture.h"
#include "../level/TomlLevel.h"
#include "../particle/Particles.h"
#include "../ui/Label.h"
#include "../ui/List.h"
#include "../ui/Margin.h"
#include "../ui/utils.h"
#include "../viewer/TilemapViewer.h"
#include "Manager.h"
#include "MenuState.h"
#include "PausedState.h"

namespace state {

math::AABB makeCameraAABB(sf::RenderWindow& w) {
    auto width = float(w.getSize().x) / float(w.getSize().x + w.getSize().y);
    auto height = float(w.getSize().y) / float(w.getSize().x + w.getSize().y);
    math::Vector2 windowVec(width, height);
    windowVec.x /= windowVec.y;
    windowVec.y /= windowVec.y;
    return {{-windowVec.x / 2, -windowVec.y / 2}, {windowVec.x / 2, windowVec.y / 2}};
}

GameState::GameState(const assets::Level& level, sf::RenderWindow& gameWindow)
    : level(level), window(gameWindow), world([&]() -> world::World {
          auto factory = std::make_shared<viewer::SFMLFactory>(gameWindow);
          return level.getLoader()->load(factory, makeCameraAABB(gameWindow));
      }()),
      backgroundTx(
          std::make_unique<sf::Texture>(assets::AssetServer::get().get<assets::Texture>("bg_brown.png").get())) {

    world.getResources().insert(particle::Particles());
    world.getResources().insert(resources::Score(10000));
    world.getResources().insert(resources::GameStatus(resources::GameStatus::Playing));

    // Create the UI.
    {
        auto& uiFont = assets::AssetServer::get().get<assets::Font>("november.ttf");

        auto s = std::to_string(world.getResources().get<resources::Score>()->score) + " Pts";
        sf::Text t(s, uiFont, static_cast<unsigned int>(scale(window.getSize().y, 1.1f)));
        scoreCounter = std::make_shared<ui::Label>(ui::Label(std::move(t)));

        std::stringstream ss;
        ss << std::setprecision(2) << std::fixed << timeSinceStart << "s";
        sf::Text t2(ss.str(), uiFont, static_cast<unsigned int>(scale(window.getSize().y, 0.8f)));
        timeCounter = std::make_shared<ui::Label>(ui::Label(std::move(t2)));

        std::vector<std::shared_ptr<ui::Element>> v;
        v.emplace_back(std::move(std::make_shared<ui::Margin>(ui::Margin(scoreCounter, 15, 5))));
        v.emplace_back(std::move(std::make_shared<ui::Margin>(ui::Margin(timeCounter, 15, 25))));
        ui = std::make_unique<ui::List>(ui::List(std::move(v)));
    }

    // Set up the background.
    backgroundTx->setRepeated(true);
    background.setTexture(backgroundTx.get());
    background.setTextureRect({0, 0, int(250 * world.getCamera().getBox().getWidth()),
                               2 * int(250 * world.getCamera().getBox().getHeight())});
    background.setSize({static_cast<float>(gameWindow.getSize().x), static_cast<float>(gameWindow.getSize().y * 2)});
}

void GameState::tick(Manager& stateManager) {
    float deltaTime = utils::Stopwatch::get().getDelta();

    world.getResources().get<particle::Particles>()->tick();
    resources::GameStatus::Status gameStatus = world.getResources().get<resources::GameStatus>()->get();
    if (gameStatus != resources::GameStatus::Playing) {
        bool wasFirst = timeSinceFinish == 0;
        timeSinceFinish += deltaTime;
        if (timeSinceFinish >= 1.f) {
            if (gameStatus == resources::GameStatus::Failed) {
                stateManager.set(std::make_unique<GameState>(GameState(level, window)));
                return;
            }
            if (gameStatus == resources::GameStatus::Finished && level.getNext().has_value()) {
                stateManager.set(std::make_unique<GameState>(
                    GameState(assets::AssetServer::get().get<assets::Level>(level.getNext().value()), window)));
                return;
            }
            stateManager.set(std::make_unique<state::MenuState>(state::MenuState(window)));
            return;
        }
        // We want to still handle the first frame after the game should finish (so all entities can react to it).
        if (!wasFirst)
            return;
    }
    timeSinceStart += deltaTime;

    // Subtract from the score if it has been long enough since the last time we did this.
    timeSinceLastSubtractScore += deltaTime;
    if (timeSinceLastSubtractScore > 0.1f) {
        auto& score = world.getResources().get<resources::Score>()->score;
        score -= int(timeSinceLastSubtractScore / 0.01f);
        if (score < 0)
            score = 0;
        timeSinceLastSubtractScore -= float(int(timeSinceLastSubtractScore / 0.1f)) * 0.1f;
    }

    float xAxis = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        xAxis = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        xAxis = 1;
    resources::Input input(xAxis, sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
#ifndef NDEBUG
    input.setDebugMode(sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde));
#endif
    world.getResources().insert(input);

    world.tick();

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        escReleased = true;
    else if (escReleased && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        stateManager.push(std::make_unique<PausedState>(PausedState(window)));
        escReleased = false;
    }

    scoreCounter->getText().setString(std::to_string(world.getResources().get<resources::Score>()->score) + " Pts");
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed << timeSinceStart << "s";
    timeCounter->getText().setString(ss.str());
}

void GameState::draw() {
    world::Camera& c = world.getCamera();
    background.setPosition(0, float(int(fabsf(c.getPosition().y * 185.f)) % int(window.getSize().y)) -
                                  static_cast<float>(window.getSize().y));
    window.draw(background);
    world.draw();

    world.getResources().get<particle::Particles>()->draw(world, window);

    scoreCounter->getText().setCharacterSize(static_cast<unsigned int>(scale(window.getSize().y, 1.1f)));
    scoreCounter->getText().setOutlineThickness(scale(window.getSize().y, 0.06f));

    timeCounter->getText().setCharacterSize(static_cast<unsigned int>(scale(window.getSize().y, 0.8f)));
    timeCounter->getText().setOutlineThickness(scale(window.getSize().y, 0.06f));
    ui->draw({0, 0}, window);
}

void GameState::handleEvent(Manager& stateManager, sf::Event event) {
    switch (event.type) {
    case sf::Event::Resized:
        // The window's aspect ratio might have changed, so we change the camera's AABB to reflect this change.
        world.getCamera().setBox(makeCameraAABB(window));
        // Also resize the background.
        background.setSize({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y * 2)});
    default:
        return;
    }
}

} // namespace state