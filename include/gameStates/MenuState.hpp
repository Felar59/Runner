#ifndef MenuState_hpp
#define MenuState_hpp

#include <SFML/Graphics.hpp>
#include "ManageState.hpp"
#include "LoopManagement.hpp"
#include <memory>
#include <ParalaxBG.hpp>
#include "SoundManager.hpp"
#include "Settings.hpp"
#include "UIbuttons.hpp"

class LoopManagement;

class MenuState : public ManageState
{
private:
    sf::Font font;
    ParalaxBG paralaxBG;
    sf::Texture layoutTexture;
    sf::Sprite layoutSprite;
    std::vector<sf::Texture> buttonTextures;
    std::map<ButtonType, button> buttons;

    sf::Vector2f mousePos;    
    LoopManagement* loopMain = nullptr;
    float deltaTime = 0.0f;
    sf::Clock clock;
    sf::Time lastFrameTime;
    bool isPause = false;

    Settings settings;
    bool settingsOpen = false;

public:
    MenuState();
    ~MenuState();

    std::string toString(ButtonType type);
    bool LoadRessources(sf::RenderWindow& window) override;

    // Events Logic
    bool ButtonPressed(ButtonType type, sf::Event& event, sf::Vector2f& mousePos);
    void PlayEvent(sf::RenderWindow& window, sf::Event& event, sf::Vector2f& mousePos);
    void SettingsEvent(sf::RenderWindow& window, sf::Event& event, sf::Vector2f& mousePos);
    void ExitEvent(sf::RenderWindow& window, sf::Event& event, sf::Vector2f& mousePos);
    void handleEvents(sf::RenderWindow& window, sf::Event& event) override;

    void update() override;
    void draw(sf::RenderWindow& window) override;
    void setLoopManager(LoopManagement* manager);
    void isPaused(bool pausedState) override;

};

#endif