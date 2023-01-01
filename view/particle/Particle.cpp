#include "Particle.h"

#include "../../logic/utils/Stopwatch.h"

namespace particle {

Particle::Particle(const math::Vector2& position, const assets::Sprite::State& anim)
    : timeSinceStart(0), sprite(anim.getTexture()), animation(anim) {
    pos = position;
}

void Particle::tick() {
    timeSinceStart += utils::Stopwatch::get().getDelta();
    sprite.setTextureRect(animation.getFrame(timeSinceStart));
}

void Particle::draw(world::World& world, sf::RenderWindow& window) {
    auto cam = world.getCamera();

    auto position = pos;
    math::Vector2 scale(1., 1.);

    float cameraMinX = cam.getBox().getMin().x;
    float cameraMinY = cam.getBox().getMin().y;
    float cameraMaxX = cam.getBox().getMax().x;
    float cameraMaxY = cam.getBox().getMax().y;
    float deltaX = cameraMaxX - cameraMinX;
    float deltaY = cameraMaxY - cameraMinY;

    sprite.setTextureRect(animation.getFrame(timeSinceStart));
    const float SCALE_FACTOR = 196;
    sprite.setScale(scale.x / deltaY * float(window.getSize().y) / SCALE_FACTOR,
                    scale.x / deltaY * float(window.getSize().y) / SCALE_FACTOR);
    sprite.setPosition(((position.x - cam.getPosition().x - cameraMinX) / deltaX) * float(window.getSize().x) -
                           sprite.getScale().x * float(abs(animation.getFrame(timeSinceStart).width)) / 2.f,
                       ((-position.y + cam.getPosition().y - cameraMinY) / deltaY) * float(window.getSize().y) -
                           sprite.getScale().y * float(animation.getFrame(timeSinceStart).height) / 2.f);

    window.draw(sprite);
}

const math::Vector2& Particle::getPosition() const { return pos; }

bool Particle::ended() const { return animation.getDuration() <= int(timeSinceStart * 1000.f); }

} // namespace particle
