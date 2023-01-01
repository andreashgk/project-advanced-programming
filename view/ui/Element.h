#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

#include <SFML/Graphics.hpp>

namespace ui {

/**
 * A generic UI element that can be inherited from. By itself, this does nothing, but it allows for multiple different
 * types of elements to work together.
 */
class Element {
public:
    virtual ~Element() = default;

    /**
     * Draws the element on the screen for the current frame.
     *
     * @param pos The position (in screen/pixel coordinates)
     * @param target The window to draw the UI on.
     */
    virtual void draw(sf::Vector2f pos, sf::RenderWindow& target) const = 0;

    /**
     * Returns how large the element is. Takes all child elements into account, if they exist.
     *
     * @return A vector containing the size in pixels of the element on the x-axis(=width) and the size on the
     * y-axis(=height).
     */
    virtual sf::Vector2f getSize() const = 0;
};

} // namespace ui

#endif // GAME_ELEMENT_H
