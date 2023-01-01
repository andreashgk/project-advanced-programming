#include "AnimatedViewer.h"

#include <utility>

#include "../../logic/resources/Input.h"
#include "../../logic/utils/Stopwatch.h"
#include "../../logic/world/World.h"
#include "../assets/AssetServer.h"

namespace viewer {

AnimatedViewer::AnimatedViewer(sf::RenderWindow& window, const assets::Sprite::State& state, DespawnFunc despawnFunc)
    : window(window), state(state), despawnFunc(std::move(despawnFunc)) {}

void AnimatedViewer::view(world::World& world, const world::Entity& entity) {
    if (entity.willDespawn()) {
        if (despawnFunc)
            despawnFunc(world, entity);
        return;
    }

    auto cam = world.getCamera();

    auto position = entity.getTransform().translation;
    auto scale = entity.getTransform().scale;

    float cameraMinX = cam.getBox().getMin().x;
    float cameraMinY = cam.getBox().getMin().y;
    float cameraMaxX = cam.getBox().getMax().x;
    float cameraMaxY = cam.getBox().getMax().y;
    float deltaX = cameraMaxX - cameraMinX;
    float deltaY = cameraMaxY - cameraMinY;

    sf::IntRect frame = state.getFrame(int(animStart * 1000.), entity.getVelocity().x < 0);
    auto s = sf::Sprite(state.getTexture(), frame);

    const float SCALE_FACTOR = 196;
    s.setScale(scale.x / deltaY * float(window.getSize().y) / SCALE_FACTOR,
               scale.x / deltaY * float(window.getSize().y) / SCALE_FACTOR);
    s.setPosition(((position.x - cam.getPosition().x - cameraMinX) / deltaX) * float(window.getSize().x) -
                      s.getScale().x * float(abs(frame.width)) / 2.f,
                  ((-position.y + cam.getPosition().y - cameraMinY) / deltaY) * float(window.getSize().y) -
                      s.getScale().y * float(frame.height) / 2.f);
    window.draw(s);
    animStart += utils::Stopwatch::get().getDelta();

#ifndef NDEBUG
    if (!world.getResources().get<resources::Input>()->isDebugMode())
        return;
    ColliderViewer col(window);
    col.view(world, entity);
#endif
}

} // namespace viewer