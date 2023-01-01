#ifndef GAME_LIST_H
#define GAME_LIST_H

#include <memory>
#include <vector>

#include "Element.h"

namespace ui {

/**
 * A container for multiple elements contained in a vertical bar. It has the width of the widest element, and the height
 * of all elements combined.
 *
 * The contained elements can be aligned in a few different ways. See the List::Align enum for more info.
 */
class List : public Element {
public:
    /**
     * The different ways elements in the list can be aligned. This refers to the horizontal position of the element
     * relative to the width of the list.
     */
    enum class Align {
        /**
         * Puts the left edge of each element on the left edge of the list.
         *
         * The following is an example of left-aligned elements:
         * ```
         * Abc
         * Defghi
         * Jk
         * ```
         */
        Left,
        /**
         * Puts the right edge of each element on the right edge of the list.
         *
         * The following is an example of right-aligned elements:
         * ```
         *    Abc
         * Defghi
         *     Jk
         * ```
         */
        Right,
        /**
         * Puts the center of each element on the same x-coordinate, in the center (width-wise) of the list.
         *
         * The following is an example of right-aligned elements:
         * ```
         *   abc
         * defghij
         *   klm
         * ```
         */
        Center,
    };
    List(std::vector<std::shared_ptr<Element>> elems, Align align = Align::Left);

    /**
     * @copydoc Element::draw()
     */
    void draw(sf::Vector2f pos, sf::RenderWindow& target) const override;
    /**
     * @copydoc Element::getSize()
     */
    sf::Vector2f getSize() const override;

    /**
     * Indicates what the current alignment strategy for the contained elements is.
     */
    Align getAlign() const;
    /**
     * Change the way elements are aligned inside the list.
     */
    void setAlign(Align align);

    /**
     * Lists all elements contained in the list.
     */
    const std::vector<std::shared_ptr<Element>>& getContent() const;
    /**
     * Overwrites all elements in the list.
     */
    void setContent(const std::vector<std::shared_ptr<Element>>& content);

private:
    Align align;
    std::vector<std::shared_ptr<Element>> content;
};

} // namespace ui

#endif // GAME_LIST_H
