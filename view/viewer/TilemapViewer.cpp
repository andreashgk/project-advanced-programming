#include "TilemapViewer.h"

#include "../../logic/resources/Input.h"
#include "../../logic/world/World.h"
#include "../assets/AssetServer.h"
#include "viewUtils.hpp"

#include <cmath>

namespace viewer {

TilemapViewer::TilemapViewer(sf::RenderWindow& window, std::shared_ptr<const world::TileStorage> tiles)
    : window(window), tileStorage(std::move(tiles)) {}

void TilemapViewer::view(world::World& world, const world::Entity& entity) {
    auto cam = world.getCamera();
    // It is ok to run this every frame even if it throws an error.
    std::shared_ptr<TilemapViewConfig> cfg;
    try {
        cfg = world.getResources().get<TilemapViewConfig>();
    } catch (std::out_of_range& e) {
        std::cerr << "Tilemap does not have a tileset" << std::endl;
        return;
    }

    math::Vector2 scale = entity.getTransform().scale;

    math::Vector2 visibleTileCount = {cam.getBox().getWidth() / scale.x, cam.getBox().getHeight() / scale.y};
    float firstTileX =
        std::floor(float(-tileStorage->getWidth()) * scale.x / 2.f) + entity.getTransform().translation.x;
    float screenBorderX = cam.getBox().getMin().x + cam.getPosition().x;
    float extraWidth = screenBorderX - firstTileX;

    int startX = int(std::floor(extraWidth / scale.x)) - 1;
    int endX = startX + int(std::ceil(visibleTileCount.x)) + 1;

    float firstTileY = entity.getTransform().translation.y;
    float screenBorderY = cam.getBox().getMin().y + cam.getPosition().y;
    float extraHeight = screenBorderY - firstTileY;

    int startY = int(std::floor(extraHeight / scale.y));
    int endY = startY + int(std::ceil(visibleTileCount.y));

    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            sf::IntRect uvs;
            if (tileStorage->inside(x, y)) {
                uvs = cfg->getTileset().getRect(tileStorage->getTile(x, y).getName(), tileStorage->getVariant(x, y));
            } else if (cfg) {
                uvs = cfg->getTileset().getRect(cfg->getDefaultTileName(), cfg->getDefaultTileVariant());
            }

            math::AABB box =
                math::AABB({0., 0.}, {1., 1.}).operator+({float(x) - float(tileStorage->getWidth()) / 2.f, float(y)}) *
                    scale +
                entity.getTransform().translation;

            auto min = scaleCoordinate(box.getMin(), cam, window);
            auto max = scaleCoordinate(box.getMax(), cam, window);
            sf::RectangleShape rect({(max.x - min.x), (max.y - min.y)});
            rect.setPosition(min.x, min.y);
            rect.scale(1, 1);
            rect.setTexture(&cfg->getTileset().getTexture(), true);

            rect.setTextureRect(uvs);
            window.draw(rect);
        }
    }

#ifndef NDEBUG
    if (!world.getResources().get<resources::Input>()->isDebugMode())
        return;
    ColliderViewer col(window);
    col.view(world, entity);
#endif
}

// TilemapViewConfig
// -----------------

TilemapViewConfig::TilemapViewConfig(std::string tileset, std::string defaultTileName, std::uint8_t defaultTileVariant)
    : defaultTileName(std::move(defaultTileName)), defaultTileVariant(defaultTileVariant), tileset(std::move(tileset)) {
}

const std::string& TilemapViewConfig::getDefaultTileName() const { return defaultTileName; }
uint8_t TilemapViewConfig::getDefaultTileVariant() const { return defaultTileVariant; }

const assets::Tileset& TilemapViewConfig::getTileset() const {
    return assets::AssetServer::get().get<assets::Tileset>(tileset);
}

} // namespace viewer