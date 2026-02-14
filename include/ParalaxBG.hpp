#ifndef PARALAX_BG_HPP
#define PARALAX_BG_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class ParalaxBG
{
private:
    float bg2Speed = 125.0f;
    float bg3Speed = 187.5f;
    float bg4Speed = 250.0f;

    std::vector<sf::Texture> bgTextures;
    std::vector<sf::Sprite> bgSprites;
    std::vector<sf::Sprite> bgSpritesv;
    std::vector<float> bgSpeed = {bg2Speed, bg3Speed, bg4Speed};

public:
    ParalaxBG();
    ~ParalaxBG();

    void LoadRessources(sf::RenderWindow& window, bool inMenu);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
};

#endif