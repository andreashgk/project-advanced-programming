#include "Player.h"

#include "../../resources/GameStatus.h"
#include "../../resources/Input.h"
#include "../../utils/Stopwatch.h"
#include "../World.h"

namespace world {

const float JUMP_COOLDOWN = 0.2f;
const float JUMP_TIMER = 0.02f;

const float WALK_FORCE = 900;
const float FLOAT_FORCE = 90;

Player::Player(std::unique_ptr<Viewer> viewer, const math::Transform& transform)
    : Entity(std::move(viewer), transform, 100, math::Collider({{-0.040f, 0.033f}, {0.043f, -0.0765f}})) {}

void Player::tick(World& world) {
    jumpCooldown -= utils::Stopwatch::get().getDelta();
    if (jumpCooldown < 0)
        jumpCooldown = 0;
    jumpTimer -= utils::Stopwatch::get().getDelta();
    if (jumpTimer < 0)
        jumpTimer = 0;

    auto input = world.getResources().get<resources::Input>();

    if (input->isJumping()) {
        jumpTimer = JUMP_TIMER;
    }

    // Get only the largest collision vector.
    math::Vector2 collideVec;
    for (const auto& collision : world.getCollisions().getEdges(shared_from_this())) {
        if (collision.second.has_value() && collideVec.getLength() < collision.second->getLength()) {
            collideVec = collision.second.value();
        }
    }

    if (collideVec.y < 0) {
        applyForce({input->getHorizontalAxis() * WALK_FORCE, 0});
    } else {
        applyForce({input->getHorizontalAxis() * FLOAT_FORCE, 0});
    }

    if (jumpTimer > 0 && jumpCooldown == 0 && collideVec.getLength() != 0) {
        auto side = collideVec.getNormalizedOrZero();
        side.y = 0;
        if (side.x == 0) {
            side.x = -input->getHorizontalAxis() * 0.2f;
        }
        side.x *= 0.30f;
        setVelocity(getVelocity() + math::Vector2(0, 0.9f) - side);
        jumpCooldown = JUMP_COOLDOWN;
    }

    // Move the camera is the player gets close to the top of the screen.
    float camSize = world.getCamera().getBox().getHeight();
    float height =
        getTransform().translation.y - (world.getCamera().getBox().getMin().y + world.getCamera().getPosition().y);
    if (height / camSize > 0.8) {
        float newY = getTransform().translation.y - camSize * 0.3f;
        world.getCamera().setPosition(
            {world.getCamera().getPosition().x, std::min(newY, world.getCamera().getDestination().y)});
    }

    // Kill the player when they fall below the screen.
    if (getTransform().translation.y < world.getCamera().getBox().getMin().y + world.getCamera().getPosition().y) {
        despawn();
        world.getResources().insert(resources::GameStatus(resources::GameStatus::Failed));
    }
}

} // namespace world
