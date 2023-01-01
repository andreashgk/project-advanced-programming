#ifndef GAME_ANIMATEDVIEWER_H
#define GAME_ANIMATEDVIEWER_H

#include "SFML/Graphics.hpp"

#include <functional>

#include "../../logic/math/Vector2.h"
#include "../../logic/world/Entity.h"
#include "../../logic/world/Viewer.h"
#include "../assets/Sprite.h"
#include "ColliderViewer.h"

namespace viewer {

using DespawnFunc = std::function<void(world::World& world, const world::Entity& entity)>;

/**
 * AnimatedViewer can render a single animated sprite state, as well as allow for a function to be run when the viewed
 * entity is despawned. An example where this is useful is spawning particles when a pickup is collected.
 */
class AnimatedViewer : public world::Viewer {
public:
    AnimatedViewer(sf::RenderWindow& window, const assets::Sprite::State& state, DespawnFunc despawnFunc);
    void view(world::World& w, const world::Entity& entity) override;

private:
    const assets::Sprite::State& state;
    DespawnFunc despawnFunc;
    float animStart = 0;
    sf::RenderWindow& window;
};

} // namespace viewer

#endif // GAME_ANIMATEDVIEWER_H
