#include "Finish.h"

#include "../../resources/GameStatus.h"
#include "../World.h"

namespace world {

Finish::Finish(std::unique_ptr<Viewer> v, math::Transform tr)
    : Entity(std::move(v), tr, 0, math::Collider({{-0.050f, 0.06f}, {-0.036f, -0.16f}})) {}

void Finish::tick(World& world) {}

bool Finish::onCollide(World& world, const std::shared_ptr<Entity>& e) {
    if (e->getMass() == 0) {
        return false;
    }
    e->despawn();
    world.getResources().insert(resources::GameStatus(resources::GameStatus::Finished));
    return false;
}

} // namespace world