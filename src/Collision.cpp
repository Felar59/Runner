#include "Collision.hpp"

bool RectRectCollision(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2) {
    sf::FloatRect rect1Bounds = rect1.getGlobalBounds();
    sf::FloatRect rect2Bounds = rect2.getGlobalBounds();

    return rect1Bounds.intersects(rect2Bounds);
};
