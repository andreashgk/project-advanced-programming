#ifndef GAME_LABEL_H
#define GAME_LABEL_H

#include "Element.h"

namespace ui {

/**
 * A simple text box.
 */
class Label : public Element {
public:
    Label(sf::Text t);

    void draw(sf::Vector2f pos, sf::RenderWindow& target) const override;

    /**
     * @copydoc Element::getSize()
     */
    sf::Vector2f getSize() const override;

    /**
     * @copydoc Element::draw()
     */
    sf::Text& getText();

    /**
     * Returns the text that will be rendered in the label.
     */
    const sf::Text& getText() const;

private:
    // THis text is made mutable since it acts like a cached UI element. THis way, it does not have to be recreated
    // each frame.
    mutable sf::Text t;
};

} // namespace ui

#endif // GAME_LABEL_H
