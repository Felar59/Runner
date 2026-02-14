#ifndef UIBUTTONS_hpp
#define UIBUTTONS_hpp

#include <SFML/Graphics.hpp>

struct button{
    sf::Text text;
    sf::RectangleShape buttonHitbox;
    std::vector<sf::Sprite> onOffSprite;
    bool hovered = false;
};
enum class ButtonType{
    EXIT,
    SETTINGS,
    PLAY
};

inline bool ButtonPressed(ButtonType type, sf::Event& event, std::map<ButtonType, button>& buttons, sf::Vector2f& mousePos) {
    buttons[type].hovered = false; /// Reset hovered animation
    if (buttons[type].buttonHitbox.getGlobalBounds().contains(mousePos)) {
        buttons[type].hovered = true; // Set hovered animation
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){ // if clicked return true
            std::cout << "A button is clicked" << std::endl;
            return true;
        }
    }
    return false;
};

#endif