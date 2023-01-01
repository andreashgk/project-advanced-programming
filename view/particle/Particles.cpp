#include "Particles.h"

namespace particle {

void Particles::insert(const Particle& p) { vec.emplace_back(p); }

void Particles::tick() {
    std::vector<Particle> newParticles;
    for (auto& particle : vec) {
        particle.tick();
        if (!particle.ended()) {
            newParticles.emplace_back(particle);
        }
    }
    vec = std::move(newParticles);
}

void Particles::draw(world::World& world, sf::RenderWindow& window) {
    for (auto& particle : vec) {
        particle.draw(world, window);
    }
}

} // namespace particle
