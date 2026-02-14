#ifndef LOOP_MANAGEMENT_HPP
#define LOOP_MANAGEMENT_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "PauseMenu.hpp"
#include "ManageState.hpp"
#include "SoundManager.hpp"

class PauseMenu;

class LoopManagement
{
private:
    std::unique_ptr<ManageState>  currentState;
    sf::RenderWindow* window;
    PauseMenu* pause;
    bool inPause = false;
    std::shared_ptr<SoundManager> soundManager;
    

public:
    LoopManagement();
    ~LoopManagement();

    void run(sf::RenderWindow& window);
    void changeState(std::unique_ptr<ManageState> newState, sf::RenderWindow& window);
    void setSoundManager(std::shared_ptr<SoundManager> soundManager);
    void callSoundManager(const std::string& soundName);
};

#endif