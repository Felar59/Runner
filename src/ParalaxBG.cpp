#include "ParalaxBG.hpp"
#include <iostream>

ParalaxBG::ParalaxBG() {
}
ParalaxBG::~ParalaxBG() {
}

void ParalaxBG::LoadRessources(sf::RenderWindow& window, bool inMenu) {
    bgTextures.resize(4);
    bgSprites.resize(4);
    bgSpritesv.resize(3);
    bgSpeed.resize(3);

    float TileSizeY = window.getSize().y / 9;
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2 bgSize = {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) };
    sf::Texture tempTexture;
    for (int i = 0; i < 4; ++i) {
        if (!bgTextures[i].loadFromFile("assets/ParalaxBg/para" + std::to_string(i+1) + ".png")) {
            std::cerr << "Error loading texture para" << i+1 << ".png" << std::endl;
            return;
        }
        bgSprites[i].setTexture(bgTextures[i]);
        float Ypos = 0;
        if (!inMenu){
            Ypos = -TileSizeY/2;
        }
        bgSprites[i].setPosition(0, Ypos);
        sf::Vector2 ScaleBackgrounds = {bgSize.x / bgTextures[i].getSize().x, bgSize.y / bgTextures[i].getSize().y};
        bgSprites[i].setScale(ScaleBackgrounds.x, ScaleBackgrounds.y);
        if (i > 0) {
            bgSpritesv[i-1] = bgSprites[i];
            bgSpritesv[i-1].setPosition(bgSprites[i].getPosition().x + bgSprites[i].getGlobalBounds().width - 1.f, bgSprites[i].getPosition().y);
        }
    }
}

void ParalaxBG::Update(float deltaTime){
    for (int i = 1; i < 4; i++) {
        bgSprites[i].setPosition(bgSprites[i].getPosition().x - (bgSpeed[i-1]*deltaTime), bgSprites[i].getPosition().y);
        bgSpritesv[i-1].setPosition(bgSpritesv[i-1].getPosition().x - (bgSpeed[i-1]*deltaTime), bgSpritesv[i-1].getPosition().y);
        if (bgSprites[i].getPosition().x <= -bgSprites[i].getGlobalBounds().width) {
            bgSprites[i].setPosition(bgSpritesv[i-1].getPosition().x + bgSpritesv[i-1].getGlobalBounds().width - 1.f, bgSprites[i].getPosition().y);
        }
        if (bgSpritesv[i-1].getPosition().x <= -bgSprites[i].getGlobalBounds().width) {
            bgSpritesv[i-1].setPosition(bgSprites[i].getPosition().x + bgSprites[i].getGlobalBounds().width - 1.f, bgSpritesv[i-1].getPosition().y);
        }
    }
}

void ParalaxBG::Draw(sf::RenderWindow& window) {
    for (size_t i = 0; i < bgSprites.size(); i++) {
        window.draw(bgSprites[i]);
        if (i > 0) {
            window.draw(bgSpritesv[i - 1]);
        }
    }   
}