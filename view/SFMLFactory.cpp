#include "SFMLFactory.h"

#include "assets/AssetServer.h"
#include "assets/Texture.h"
#include "particle/Particles.h"
#include "viewer/AnimatedViewer.h"
#include "viewer/PlayerViewer.h"
#include "viewer/TilemapViewer.h"

namespace viewer {

SFMLFactory::SFMLFactory(sf::RenderWindow& window) : window(window) {}

std::unique_ptr<world::Player> SFMLFactory::createPlayer() const {
    return std::make_unique<world::Player>(world::Player(std::make_unique<PlayerViewer>(PlayerViewer(window)), {}));
}

std::unique_ptr<world::Tilemap> SFMLFactory::createTilemap(std::shared_ptr<const world::TileStorage> tiles) const {
    return std::make_unique<world::Tilemap>(
        world::Tilemap(std::make_unique<TilemapViewer>(TilemapViewer(window, tiles)), {}, tiles));
}

std::unique_ptr<world::Melon> SFMLFactory::createMelon(int score) const {
    return std::make_unique<world::Melon>(
        world::Melon(std::make_unique<AnimatedViewer>(AnimatedViewer(
                         window, assets::AssetServer::get().get<assets::Sprite>("melon.yml").getState("idle"),
                         // Spawn a particle when the pickup gets picked up.
                         [](world::World& world, const world::Entity& entity) {
                             world.getResources().get<particle::Particles>()->insert(particle::Particle(
                                 entity.getTransform().translation,
                                 assets::AssetServer::get().get<assets::Sprite>("melon.yml").getState("collected")));
                         })),
                     {}, score));
}

std::unique_ptr<world::Finish> SFMLFactory::createFinish() const {
    return std::make_unique<world::Finish>(
        world::Finish(std::make_unique<AnimatedViewer>(AnimatedViewer(
                          window, assets::AssetServer::get().get<assets::Sprite>("finish.yml").getState("idle"),
                          [](world::World& world, const world::Entity& entity) {})),
                      {}));
}

} // namespace viewer
