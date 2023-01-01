#include "PlayerViewer.h"

#include <cmath>

#include "../../logic/resources/Input.h"
#include "../../logic/utils/Stopwatch.h"
#include "../../logic/world/World.h"
#include "../assets/AssetServer.h"
#include "../assets/Sprite.h"
#include "../particle/Particle.h"
#include "../particle/Particles.h"

namespace viewer {

// Helper enum to determine which animation to play.
enum class PlayerState {
    Idle,
    Run,
    Jump,
    Fall,
    WallJump,
};

std::string playerStateToStr(PlayerState s) {
    switch (s) {
    case PlayerState::Idle:
        return "idle";
    case PlayerState::Run:
        return "run";
    case PlayerState::Jump:
        return "jump";
    case PlayerState::Fall:
        return "fall";
    case PlayerState::WallJump:
        return "wall_jump";
    }
    // This should never happen.
    throw std::out_of_range("cannot identify unknown player state");
}

PlayerViewer::PlayerViewer(sf::RenderWindow& window) : window(window) {}

void PlayerViewer::view(world::World& world, const world::Entity& entity) {
    if (first) {
        world.getResources().get<particle::Particles>()->insert(
            particle::Particle(entity.getTransform().translation,
                               assets::AssetServer::get().get<assets::Sprite>("spawn.yml").getState("spawn")));
        first = false;
        return;
    }
    if (entity.willDespawn()) {
        world.getResources().get<particle::Particles>()->insert(
            particle::Particle(entity.getTransform().translation,
                               assets::AssetServer::get().get<assets::Sprite>("spawn.yml").getState("despawn")));
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

    auto& sprite = assets::AssetServer::get().get<assets::Sprite>("player.yml");

    // Determine what state the player is currently in. We can use this to determine what animation to play.
    PlayerState playerState = PlayerState::Idle;
    {
        std::shared_ptr<resources::Input> input = world.getResources().get<resources::Input>();
        math::Vector2 velocity = entity.getVelocity();
        math::Vector2 collideVec;
        for (const auto& collision : world.getCollisions().getEdges(entity.shared_from_this())) {
            if (collision.second.has_value() && collideVec.getLength() < collision.second->getLength()) {
                collideVec = collision.second.value();
            }
        }

        if (collideVec.y < 0) {
            if (input->getHorizontalAxis() != 0)
                playerState = PlayerState::Run;
        } else {
            if (velocity.y > 0.0001) {
                playerState = PlayerState::Jump;
            } else if (velocity.y < -0.0001) {
                playerState = PlayerState::Fall;
            }
            if (std::fabs(collideVec.x) > std::numeric_limits<float>::epsilon() && collideVec.y >= 0) {
                playerState = PlayerState::WallJump;
            }
        }
    }
    std::string sn = playerStateToStr(playerState);

    auto& state = sprite.getState(sn);

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