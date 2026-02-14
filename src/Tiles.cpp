#include "Tiles.hpp"
#include "Enemy.hpp"
#include <iostream>
#include <random>

Tiles::Tiles(sf::RenderWindow& window){
    // Initialize the tiles
    LoadRessources(window);
    tiles.resize(9, std::vector<Tile>(20, Tile()));
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.1f);
    noise.SetSeed(rand() % 500);
    for (int x = 0; x < 20; x++){
        for (int y = 6; y < 9; ++y) {
            
            tiles[y][x] = Tile(x * TileSizeX, y * TileSizeY, TileSizeX, TileSizeY, false);
            if (y == 6){
                tiles[y][x].setSprite(sprites[3]);
            } else{
                tiles[y][x].setSprite(sprites[7]);
            }
        }
    }
}

Tiles::~Tiles(){
    tiles.clear();
};

// Getters
int Tiles::size(){
    return tiles.size();
}

std::vector<std::vector<Tile>>& Tiles::getVector(){
    return tiles;
}

float Tiles::getSpeed(){
    return GroundSpeed;
}

// Others
void Tiles::LoadRessources(sf::RenderWindow& window){
    int TileSize = 16;
    sf::Image Tileset;
    if (!Tileset.loadFromFile("assets/Tiles/Tileset.png")){
        std::cerr << "Error loading Tileset" << std::endl;
    }
    sf::Texture tempTexture;
    // pair vector
    std::vector<sf::Vector2f> textureCoords = {
        {3, 0}, {0, 0}, {2, 0}, {1, 0}, // TopLeftRight, TopLeft, TopRight, Top
        {3, 1}, {0, 1}, {2, 1}, {1, 1},  // LeftRight, Left, Right, Middle
        {0, 2}, {1, 2}, {2, 2}, {3, 2}, // Middle Variation
    };
    for (const auto& coord : textureCoords) {
        if (!tempTexture.loadFromImage(Tileset, sf::IntRect(coord.x * TileSize, coord.y * TileSize, TileSize, TileSize))){
            std::cerr << "Error loading texture" << std::endl;
        }
        textures.push_back(tempTexture);
    }
    for (auto& texture : textures){
        texture.getSize();
        sf::Sprite tempSprite;
        tempSprite.setTexture(texture);
        tempSprite.setScale(TileSizeX / static_cast<float>(texture.getSize().x),
                    TileSizeY / static_cast<float>(texture.getSize().y));
        sprites.push_back(tempSprite);
    } 
}

void Tiles::draw(sf::RenderWindow& window){
    for (size_t y = 0; y < tiles.size(); y++){
        for (size_t x = 0; x < tiles[y].size(); x++){
            tiles[y][x].draw(window);
        }
    }
}

void Tiles::shift(std::vector<std::vector<Tile>>& tiles, std::vector<Enemy>& enemies){
    // Shift all tiles to the left
    for (size_t y = 0; y < tiles.size(); y++){
        for (size_t x = 0; x < tiles[y].size() - 1; x++){
            tiles[y][x] = tiles[y][x+1];
            tiles[y][x].setPosition(x * TileSizeX, y * TileSizeY);
        }
    }
    elapsed += 0.0015f;
    float maxVariation = std::min(elapsed, 0.7f);
    noiseVariationForce += maxVariation;
    float n = noise.GetNoise(noiseVariationForce, 0.0f);
    int groundHeight = std::min(8, static_cast<int>((n + 1.0f) / 1.4f * (tiles.size() - 1)));
    for (size_t y = 0; y < tiles.size(); y++){

        if (int(y) >= groundHeight) {
            tiles[y][19] = Tile(20 * TileSizeX, y * TileSizeY, TileSizeX, TileSizeY, false);
        } else {
            tiles[y][19] = Tile();
        }

        // Set the texture of the 18 column
        if (!tiles[y][18].isVoid()) {
            bool top = tiles[y-1][18].isVoid();
            bool left = tiles[y][17].isVoid();
            bool right = tiles[y][19].isVoid();
        
            if (top && left && right) {
                tiles[y][18].setSprite(sprites[0]);
            } else if (top && left) {
                tiles[y][18].setSprite(sprites[1]);
            } else if (top && right) {
                tiles[y][18].setSprite(sprites[2]);
            } else if (top) {
                tiles[y][18].setSprite(sprites[3]);
            } else if (left && right) {
                tiles[y][18].setSprite(sprites[4]);
            } else if (left) {
                tiles[y][18].setSprite(sprites[5]);
            } else if (right) {
                tiles[y][18].setSprite(sprites[6]);
            } else {
                if (rand() % 10 == 0){
                    tiles[y][18].setSprite(sprites[8 + frameMiddle]);
                    frameMiddle = (frameMiddle + 1) % 4;
                } else {                
                    tiles[y][18].setSprite(sprites[7]);
                }
            }
        }
    }
    if (tiles[groundHeight-1][17].isVoid() && tiles[groundHeight-1][18].isVoid()){ // not spawn behind a wall
        for(auto& enemy : enemies){
            enemy.summonEnemy({tiles[groundHeight][19].getPos().x - TileSizeX, tiles[groundHeight][19].getPos().y - TileSizeY}, elapsed);
        }
    }
}

void Tiles::moves(float& deltaTime, std::vector<Enemy>& enemies){
    moveOffset += GroundSpeed * deltaTime;

    if (moveOffset >= TileSizeX){
        moveOffset -= TileSizeX;
        shift(tiles, enemies);
    }

    for (size_t y = 0; y < tiles.size(); y++){
        for (size_t x = 0; x < tiles[y].size(); x++){
            float newX = x * TileSizeX - moveOffset;
            tiles[y][x].setPosition(newX, y * TileSizeY);
        }
    }
}