#include "ColliderViewer.h"

#include "../../logic/world/World.h"

namespace viewer {

ColliderViewer::ColliderViewer(sf::RenderWindow& w) : window(w) {}

math::Vector2 scale(const math::Vector2& in, const world::Camera& cam, const sf::RenderWindow& window) {
    float cameraMinX = cam.getBox().getMin().x;
    float cameraMinY = cam.getBox().getMin().y;
    float cameraMaxX = cam.getBox().getMax().x;
    float cameraMaxY = cam.getBox().getMax().y;
    float deltaX = cameraMaxX - cameraMinX;
    float deltaY = cameraMaxY - cameraMinY;

    return {((in.x - cam.getPosition().x - cameraMinX) / deltaX) * float(window.getSize().x),
            ((-in.y + cam.getPosition().y - cameraMinY) / deltaY) * float(window.getSize().y)};
}

void ColliderViewer::view(world::World& world, const world::Entity& entity) {
    auto cam = world.getCamera();

    auto col = entity.getWorldCollider().getContent();
    for (const auto& box : col) {
        auto min = scale(box.getMin(), cam, window);
        auto max = scale(box.getMax(), cam, window);
        sf::RectangleShape rect({max.x - min.x, max.y - min.y});
        rect.setPosition(min.x, min.y);
        rect.setOutlineColor(sf::Color::Magenta);
        rect.setOutlineThickness(1);
        rect.setFillColor(sf::Color::Transparent);
        window.draw(rect);
    }
}

} // namespace viewer