#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <iostream>
#include <fstream>
#include "json.hpp"
#include <SFML/Graphics.hpp>
#include "UIbuttons.hpp"

using json = nlohmann::json;

class Settings
{
private:
    json data;
    std::map<ButtonType, button> buttons;

    std::unordered_map<int, std::string> buttonsName;

public:
    Settings();
    ~Settings();

    // Getters
    int getVolume() const;
    bool isFullscreen() const;
    int getFPS() const;

    // Setters
    void setVolume(int volume);
    void setFullscreen(bool fullscreen);
    void setFPS(int fps);

    // Save settings to file
    void saveSettings();

    // Settings GUI
    void LoadRessources(sf::RenderWindow& window, sf::Font& font);
    void handleEvents(sf::RenderWindow& window, sf::Event& event);
    void draw(sf::RenderWindow& window, std::map<ButtonType, button> buttons);
};

#endif