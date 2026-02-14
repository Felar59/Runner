#include "LoopManagement.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "SoundManager.hpp"
#include "Settings.hpp"
#include <SFML/Graphics.hpp>

int main() {
    LoopManagement mainLoop;
    auto soundManager = std::make_shared<SoundManager>();
    soundManager->LoadResources();

    Settings settings;
    sf::Uint32 screen = settings.isFullscreen() ? sf::Style::Fullscreen : sf::Style::Default;

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktopMode.width, desktopMode.height), "Runner", screen);

    std::unique_ptr<MenuState> menuState = std::make_unique<MenuState>();
    menuState->setLoopManager(&mainLoop);
    mainLoop.setSoundManager(soundManager);
    mainLoop.changeState(std::move(menuState), window);
    mainLoop.run(window);

    return 0;
}