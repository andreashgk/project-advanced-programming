#include "Melon.h"

#include "../../resources/Score.h"
#include "../World.h"

namespace world {

Melon::Melon(std::unique_ptr<Viewer> v, math::Transform tr, int score)
    : Entity(std::move(v), tr, 0, {{{{-0.025f, -0.025f}, {0.025f, 0.025f}}}}), score(score) {}

void Melon::tick(World& world) {}

bool Melon::onCollide(World& world, const std::shared_ptr<Entity>& e) {
    if (collected || e->getMass() == 0)
        return false;
    despawn();
    world.getResources().get<resources::Score>()->score += score;
    collected = true;
    return false;
}

} // namespace world