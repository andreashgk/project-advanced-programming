#ifndef GAME_MARGIN_H
#define GAME_MARGIN_H

#include <memory>

#include "Element.h"

namespace ui {

/**
 * An elements that adds empty space around another element.
 */
class Margin : public Element {
public:
    /**
     * Creates a margin with equal width on the left/right side and equal height on the top/bottom.
     *
     * @param child The element to create a margin around.
     * @param x The extra space to add on the left and right sides in pixels.
     * @param y The extra spoace to add on the top and bottom in pixels.
     */
    Margin(std::shared_ptr<Element> child, float x, float y);
    /**
     * Create a margin around an element.
     *
     * @param child The element to create a margin around.
     * @param left The extra space on the left side in pixels.
     * @param right The extra space on the right in pixels.
     * @param top The extra space on the top in pixels.
     * @param bottom The extra space on the bottom in pixels.
     */
    Margin(std::shared_ptr<Element> child, float left, float right, float top, float bottom);

    /**
     * @copydoc Element::draw()
     */
    void draw(sf::Vector2f pos, sf::RenderWindow& target) const override;
    /**
     * @copydoc Element::getSize()
     */
    sf::Vector2f getSize() const override;

private:
    float left = 0;
    float right = 0;
    float top = 0;
    float bottom = 0;
    std::shared_ptr<Element> child;
};

} // namespace ui

#endif // GAME_MARGIN_H
