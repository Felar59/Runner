#include "LoopManagement.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

LoopManagement::LoopManagement(){
    pause = new PauseMenu();
};

LoopManagement::~LoopManagement(){
};

void LoopManagement::run(sf::RenderWindow& window){
    pause->LoadRessources(window);
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape){
                    inPause = !inPause;
                }
            }
            if (!inPause){
                currentState->handleEvents(window, event);
                currentState->isPaused(inPause);
            } else{
                pause->handleEvents(window, event, inPause);
                currentState->isPaused(inPause);
            }
        }
        window.clear();
        if (!inPause){
            currentState->update();
        }
        currentState->draw(window);

        if(inPause){
            pause->drawPause(window);
        }
        window.display();
    }
};

void LoopManagement::changeState(std::unique_ptr<ManageState> newState, sf::RenderWindow& window){
    currentState = std::move(newState);
    pause->setLoopManager(this);
    if(currentState->LoadRessources(window) == false){
        std::cerr << "Error loading resources" << std::endl;
        exit(1);
    }
};

void LoopManagement::setSoundManager(std::shared_ptr<SoundManager> soundManager) {
    this->soundManager = soundManager;
}

void LoopManagement::callSoundManager(const std::string& soundName) {
    if (soundName == "music") {
        soundManager->playMusic();
    } else {
        soundManager->playSound(soundName);
    }
}