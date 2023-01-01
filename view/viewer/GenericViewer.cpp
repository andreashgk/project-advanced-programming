#include "GenericViewer.h"

#include "../../logic/resources/Input.h"
#include "../../logic/world/World.h"
#include "ColliderViewer.h"

namespace viewer {

GenericViewer::GenericViewer(sf::RenderWindow& window, const sf::Texture& tx) : window(window), sprite(tx) {}

void GenericViewer::view(world::World& world, const world::Entity& entity) {
    auto cam = world.getCamera();

    auto position = entity.getTransform().translation;
    auto scale = entity.getTransform().scale;

    float cameraMinX = cam.getBox().getMin().x;
    float cameraMinY = cam.getBox().getMin().y;
    float cameraMaxX = cam.getBox().getMax().x;
    float cameraMaxY = cam.getBox().getMax().y;
    float deltaX = cameraMaxX - cameraMinX;
    float deltaY = cameraMaxY - cameraMinY;

    auto s = sprite;
    const float SCALE_FACTOR = 128;
    s.setScale(scale.x / deltaY * float(window.getSize().y) / SCALE_FACTOR,
               scale.x / deltaY * float(window.getSize().y) / SCALE_FACTOR);
    s.setPosition(((position.x - cam.getPosition().x - cameraMinX) / deltaX) * float(window.getSize().x) -
                      s.getScale().x * float(sprite.getTexture()->getSize().x) / 2.f,
                  ((-position.y + cam.getPosition().y - cameraMinY) / deltaY) * float(window.getSize().y) -
                      s.getScale().y * float(sprite.getTexture()->getSize().y) / 2.f);
    window.draw(s);

#ifndef NDEBUG
    if (!world.getResources().get<resources::Input>()->isDebugMode())
        return;
    ColliderViewer col(window);
    col.view(world, entity);
#endif
}

} // namespace viewer
