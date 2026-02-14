#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

class HUD
{
private:
    sf::Font font;
    sf::Text scoreText;
    int scoreValue = 0;

    sf::Text difficultyText;
    int difficultyValue = 0;

    sf::Texture heartTextureOff;
    sf::Texture heartTextureOn;
    std::vector<sf::Sprite> hearts;
    
    sf::Texture menuTextureOn;
    sf::Texture menuTextureOff;
    std::vector<sf::Sprite> MenuOnOff;
    bool isPause = false;
public:
    HUD();
    ~HUD();

    void LoadRessources(sf::RenderWindow& window);
    void update(int score, int difficulty, int health, bool isPause);
    void draw(sf::RenderWindow& window);
};

#endif