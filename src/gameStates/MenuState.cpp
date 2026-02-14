#include "MenuState.hpp"
#include "GameState.hpp"
#include "LoopManagement.hpp"
#include <iostream>

MenuState::MenuState() {
    std::cout << "Création de l'état Menu" << std::endl;
};
MenuState::~MenuState(){
    std::cout << "Déstruction de l'état Menu" << std::endl;

};

std::string MenuState::toString(ButtonType type) {
    switch (type) {
        case ButtonType::PLAY: return "Play";
        case ButtonType::SETTINGS: return "Settings";
        case ButtonType::EXIT: return "Exit";
        default: return "UNKNOWN";
    }
}


bool MenuState::LoadRessources(sf::RenderWindow& window) {
    // Load background
    if (!layoutTexture.loadFromFile("assets/UI/layout.png")) {
        std::cerr << "Error loading layout texture" << std::endl;
        return false;
    }
    layoutSprite.setTexture(layoutTexture);

    // Load font
    if (!font.loadFromFile("assets/fonts/pixelFont.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return false;
    }
    sf::Vector2u windowSize = window.getSize();

    // Load buttons textures
    sf::Image buttonImage;
    if (!buttonImage.loadFromFile("assets/UI/buttons.png")) {
        std::cerr << "Error loading button image" << std::endl;
        return false;
    }
    sf::Texture buttonTextureOn;
    if (!buttonTextureOn.loadFromImage(buttonImage, sf::IntRect(0, 0, 64, 32))){
        std::cerr << "Error loading button texture On" << std::endl;
        return false;
    }
    sf::Texture buttonTextureOff;
    if (!buttonTextureOff.loadFromImage(buttonImage, sf::IntRect(64, 0, 64, 32))){
        std::cerr << "Error loading button texture Off" << std::endl;
        return false;
    }
    buttonTextures.resize(2);
    buttonTextures[0] = buttonTextureOff;
    buttonTextures[1] = buttonTextureOn;
    // Load buttons

    for (int i = 0; i < 3; i++){
        ButtonType newButton = static_cast<ButtonType>(i);
        buttons[newButton].text.setString(toString(newButton));
        buttons[newButton].text.setFont(font);
        buttons[newButton].text.setCharacterSize(65.f/1920.f*windowSize.x);
        buttons[newButton].text.setFillColor(sf::Color::White);
        
        buttons[newButton].onOffSprite.resize(2);
        for(size_t j = 0; j < 2; j++){ 
            buttons[newButton].onOffSprite[j].setTexture(buttonTextures[j]);
            buttons[newButton].onOffSprite[j].setScale((windowSize.x / 4) / buttonTextureOn.getSize().x,
                                               (windowSize.y / 8) / buttonTextureOn.getSize().y);
            buttons[newButton].onOffSprite[j].setPosition(windowSize.x/2 - buttons[newButton].onOffSprite[j].getGlobalBounds().width/2,
                                                  windowSize.y/2 - buttons[newButton].onOffSprite[j].getGlobalBounds().height * (i-0.5f)); 
        }
        buttons[newButton].buttonHitbox.setSize(sf::Vector2f(buttons[newButton].onOffSprite[0].getGlobalBounds().width, buttons[newButton].onOffSprite[0].getGlobalBounds().height));
        buttons[newButton].buttonHitbox.setPosition(buttons[newButton].onOffSprite[0].getPosition());
        buttons[newButton].buttonHitbox.setFillColor(sf::Color::Transparent);
        buttons[newButton].text.setPosition(buttons[newButton].buttonHitbox.getPosition().x + buttons[newButton].buttonHitbox.getGlobalBounds().width / 2 - buttons[newButton].text.getGlobalBounds().width / 2,
                                    buttons[newButton].buttonHitbox.getPosition().y + buttons[newButton].buttonHitbox.getGlobalBounds().height / 2 - buttons[newButton].text.getGlobalBounds().height /1.5);
    }

    layoutSprite.setScale(10.f / 1920.f * windowSize.x, 10 / 1080.f * windowSize.y);
    layoutSprite.setPosition(windowSize.x / 2 - layoutSprite.getGlobalBounds().width / 2,
                              windowSize.y / 2 - layoutSprite.getGlobalBounds().height / 2);

    paralaxBG.LoadRessources(window, true);
    
    // start Clock
    clock.restart();
    lastFrameTime = clock.getElapsedTime();
    return true;
};

bool MenuState::ButtonPressed(ButtonType type, sf::Event& event, sf::Vector2f& mousePos) {
    buttons[type].hovered = false; /// Reset hovered animation
    if (buttons[type].buttonHitbox.getGlobalBounds().contains(mousePos)) {
        buttons[type].hovered = true; // Set hovered animation
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){ // if clicked return true
            loopMain->callSoundManager("button_click");
            std::cout << toString(type) << " button clicked" << std::endl;
            return true;
        }
    }
    return false;
};

void MenuState::PlayEvent(sf::RenderWindow& window, sf::Event& event, sf::Vector2f& mousePos) {
    if(ButtonPressed(ButtonType::PLAY, event, mousePos)){
        std::unique_ptr<GameState> gameState = std::make_unique<GameState>(window);
        gameState->setLoopManager(loopMain);
        loopMain->changeState(std::move(gameState), window);
    }
};

void MenuState::SettingsEvent(sf::RenderWindow& window, sf::Event& event, sf::Vector2f& mousePos) {
    if(ButtonPressed(ButtonType::SETTINGS, event, mousePos)){
        std::cout << "Settings button clicked" << std::endl;
        settingsOpen = true;
    }
};

void MenuState::ExitEvent(sf::RenderWindow& window, sf::Event& event, sf::Vector2f& mousePos) {
    if (ButtonPressed(ButtonType::EXIT, event, mousePos)){
        window.close();
    }
};

void MenuState::handleEvents(sf::RenderWindow& window, sf::Event& event) {
    mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (settingsOpen)
    {
        settings.handleEvents(window, event);
    } else {
        PlayEvent(window, event, mousePos);
        SettingsEvent(window, event, mousePos);
        ExitEvent(window, event, mousePos);
    }
    
};

void MenuState::update() {
    sf::Time currentTime = clock.getElapsedTime();
    deltaTime = (currentTime - lastFrameTime).asSeconds();
    lastFrameTime = currentTime;
    
    paralaxBG.Update(deltaTime);
};

void MenuState::draw(sf::RenderWindow& window) {
    paralaxBG.Draw(window);
    window.draw(layoutSprite);

    if (settingsOpen)
    {
        settings.draw(window, buttons);
    } else {
        for (const auto& button : buttons) {
            window.draw(button.second.onOffSprite[button.second.hovered ? 0 : 1]);
            window.draw(button.second.text);
        }
    }
    
}

void MenuState::setLoopManager(LoopManagement* manager) {
    loopMain = manager;
}

void MenuState::isPaused(bool pausedState) {
    if (isPause && !pausedState){
        // Reset the clock when unpausing
        clock.restart();
        lastFrameTime = clock.getElapsedTime();
    }
    isPause = pausedState;
}

