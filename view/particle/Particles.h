#ifndef GAME_PARTICLES_H
#define GAME_PARTICLES_H

#include "Particle.h"

#include "../../logic/resources/Manager.h"
#include "../../logic/world/World.h"

namespace particle {

/**
 * A resource which keeps track of all particles in the world. See the Particle class for more info.
 */
class Particles : public resources::Resource {
public:
    void insert(const Particle& particle);

    /**
     * Updates all particles, removing any particles which have just finished playing their animation.
     */
    void tick();
    /**
     * Draws every active particle on the screen.
     */
    void draw(world::World& world, sf::RenderWindow& window);

private:
    std::vector<Particle> vec;
};

} // namespace particle

#endif // GAME_PARTICLES_H
