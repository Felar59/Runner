#ifndef Tiles_hpp
#define Tiles_hpp


#include <memory>
#include <SFML/Graphics.hpp>
#include "Tile.hpp"
#include "Enemy.hpp"
#include "FastNoiseLite.h"

class Enemy;

class Tiles
{
private:
    std::vector<std::vector<Tile>> tiles;
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    int TileSizeX = desktopMode.width/16;
    int TileSizeY = desktopMode.height/9;
    float GroundSpeed = 700.f;
    float moveOffset = 0.0f;

    FastNoiseLite noise;
    float noiseVariationForce = 0.0f;
    float elapsed = 0.1f; // Start variation at 0.1
    int frameMiddle = 0;

public:
    Tiles(sf::RenderWindow& window);
    ~Tiles();

    // Getters
    int size();
    std::vector<std::vector<Tile>>& getVector();
    float getSpeed();

    // Setters

    // Others
    void LoadRessources(sf::RenderWindow& window);
    void moves(float& deltaTime, std::vector<Enemy>& enemy);
    void draw(sf::RenderWindow& window);
    void shift(std::vector<std::vector<Tile>>& tiles, std::vector<Enemy>& enemy);
};

#endif