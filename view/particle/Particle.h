#ifndef GAME_PARTICLE_H
#define GAME_PARTICLE_H

#include "../../logic/math/Vector2.h"
#include "../../logic/world/World.h"
#include "../assets/Sprite.h"

namespace particle {

/**
 * A particle is a visual which may appear in a world for a short amount of time. It is not classes as an entity as it
 * does not interact with the world.
 */
class Particle {
public:
    explicit Particle(const math::Vector2& position, const assets::Sprite::State& anim);

    /**
     * Updates the frame that the animation it is when enough time has passed.
     */
    void tick();
    /**
     * Draws the particle on the screen.
     */
    void draw(world::World& world, sf::RenderWindow& window);

    /**
     * The location of the particle. This is the center of its texture.
     */
    const math::Vector2& getPosition() const;
    /**
     * @return True if the particle has finished playing its animation, false otherwise.
     */
    bool ended() const;

private:
    math::Vector2 pos;
    float timeSinceStart = 0;
    const assets::Sprite::State& animation;
    sf::Sprite sprite;
};

} // namespace particle

#endif // GAME_PARTICLE_H
