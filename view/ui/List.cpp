#include "List.h"

namespace ui {

List::List(std::vector<std::shared_ptr<Element>> elems, List::Align align) : content(std::move(elems)), align(align) {}

void List::draw(sf::Vector2f pos, sf::RenderWindow& target) const {
    float xMax = getSize().x;
    for (const auto& elem : content) {
        auto np = pos;
        switch (align) {
        case Align::Left:
            // Nothing needs to be done, this is already the case by default.
            break;
        case Align::Right:
            np.x += xMax - elem->getSize().x;
            break;
        case Align::Center:
            np.x += (xMax - elem->getSize().x) / 2.f;
            break;
        }
        elem->draw(np, target);
        pos.y += elem->getSize().y;
    }
}

sf::Vector2f List::getSize() const {
    float sizeX = 0;
    float sizeY = 0;
    for (const auto& elem : content) {
        auto b = elem->getSize();
        if (b.x > sizeX)
            sizeX = b.x;
        sizeY += b.y;
    }
    return {sizeX, sizeY};
}

const std::vector<std::shared_ptr<Element>>& List::getContent() const { return content; }
void List::setContent(const std::vector<std::shared_ptr<Element>>& content) { List::content = content; }

List::Align List::getAlign() const { return align; }
void List::setAlign(List::Align align) { this->align = align; }

} // namespace ui