#include "Margin.h"

namespace ui {

Margin::Margin(std::shared_ptr<Element> child, float x, float y)
    : child(std::move(child)), top(y), bottom(y), left(x), right(x) {}

Margin::Margin(std::shared_ptr<Element> child, float left, float right, float top, float bottom)
    : child(std::move(child)), left(left), right(right), top(top), bottom(bottom) {}

void Margin::draw(sf::Vector2f pos, sf::RenderWindow& target) const {
    pos.x += left;
    pos.y += top;
    child->draw(pos, target);
}

sf::Vector2f Margin::getSize() const { return {child->getSize().x + left + right, child->getSize().y + top + bottom}; }

} // namespace ui