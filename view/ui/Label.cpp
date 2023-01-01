#include "Label.h"

#include <iostream>
#include <utility>

namespace ui {

Label::Label(sf::Text t) : t(std::move(t)) {}

void Label::draw(sf::Vector2f pos, sf::RenderWindow& target) const {
    t.setPosition(pos);
    target.draw(t);
}

sf::Vector2f Label::getSize() const { return {t.getGlobalBounds().width, t.getGlobalBounds().height}; }

sf::Text& Label::getText() { return t; }

const sf::Text& Label::getText() const { return t; }

} // namespace ui