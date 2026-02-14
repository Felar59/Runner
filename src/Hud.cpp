#include "HUD.hpp"
#include <iostream>

HUD::HUD(){
}

HUD::~HUD(){
}

void HUD::LoadRessources(sf::RenderWindow& window){
    // Load font
    if (!font.loadFromFile("assets/fonts/pixelFont.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }
    sf::Vector2u windowSize = window.getSize();

    // Load heart textures
    sf::Image heartImage;
    if (!heartImage.loadFromFile("assets/UI/hearts.png")) {
        std::cerr << "Error loading heart image" << std::endl;
        return;
    }
    if (!heartTextureOn.loadFromImage(heartImage, sf::IntRect(0, 0, 32, 32))){
        std::cerr << "Error loading heart texture On" << std::endl;
        return;
    }
    if (!heartTextureOff.loadFromImage(heartImage, sf::IntRect(32, 0, 32, 32))){
        std::cerr << "Error loading heart texture Off" << std::endl;
        return;
    }

    hearts.resize(3);
    for (size_t i = 0; i < hearts.size(); i++){
        hearts[i].setTexture(heartTextureOn);
        hearts[i].setScale(windowSize.x / 32.f / heartTextureOn.getSize().x, windowSize.y / 18.f / heartTextureOn.getSize().y);
        hearts[i].setPosition(i * (hearts[i].getGlobalBounds().width) + hearts[i].getGlobalBounds().width * 0.2, hearts[i].getGlobalBounds().height * 0.2);
    }
    // Load Menu OnOff textures
    sf::Image MenuOnOffImage;
    if (!MenuOnOffImage.loadFromFile("assets/UI/buttons.png")) {
        std::cerr << "Error loading MenuOnOff image" << std::endl;
        return;
    }
    if (!menuTextureOn.loadFromImage(MenuOnOffImage, sf::IntRect(0, 32, 32, 32))){
        std::cerr << "Error loading MenuOnOff texture On" << std::endl;
        return;
    }
    if (!menuTextureOff.loadFromImage(MenuOnOffImage, sf::IntRect(32, 32, 32, 32))){
        std::cerr << "Error loading MenuOnOff texture Off" << std::endl;
        return;
    }

    MenuOnOff.resize(2);
    for (size_t i = 0; i < MenuOnOff.size(); i++){
        MenuOnOff[i].setTexture(menuTextureOff);
        MenuOnOff[i].setScale(windowSize.x / 16.f / menuTextureOff.getSize().x, windowSize.y / 9.f / menuTextureOff.getSize().y);
        MenuOnOff[i].setPosition(windowSize.x - (MenuOnOff[i].getGlobalBounds().width), 0);
    }
    // Load score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(50.f/1920.f*windowSize.x);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Score: 0");
    scoreText.setPosition(windowSize.x/2 - (windowSize.x/4) - (scoreText.getGlobalBounds().width/2), 0);
    // Load difficulty text
    difficultyText.setFont(font);
    difficultyText.setCharacterSize(50.f/1920.f*windowSize.x);
    difficultyText.setFillColor(sf::Color::White);
    difficultyText.setString("Difficulty: 0");
    difficultyText.setPosition(windowSize.x/2 - (difficultyText.getGlobalBounds().width/2), 0);
}

void HUD::update(int score, int difficulty, int health, bool isPause){
    scoreValue = score;
    difficultyValue = difficulty;
    this->isPause = isPause;
    for (size_t i = 0; i < hearts.size(); i++){
        if (i < static_cast<size_t>(health)){
            hearts[i].setTexture(heartTextureOn);
        } else {
            hearts[i].setTexture(heartTextureOff);
        }
    }
}

void HUD::draw(sf::RenderWindow& window){
    // Draw score text
    scoreText.setString("Score: " + std::to_string(scoreValue));
    window.draw(scoreText);

    // Draw difficulty text
    difficultyText.setString("Difficulty: " + std::to_string(difficultyValue));
    window.draw(difficultyText);

    // Draw hearts
    for (size_t i = 0; i < hearts.size(); i++){
        window.draw(hearts[i]);
    }

    // Draw Menu OnOff
    window.draw(MenuOnOff[isPause]);
}