#ifndef GameState_hpp
#define GameState_hpp

#include <SFML/Graphics.hpp>
#include "ManageState.hpp"
#include "LoopManagement.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Tiles.hpp"
#include "HUD.hpp"
#include "ParalaxBG.hpp"
#include "RessourceManager.hpp"

class LoopManagement;

class GameState : public ManageState
{
private:
    LoopManagement* loopMain = nullptr;
    Player player;
    RessourceManager ressourceManager;
    std::vector<Enemy> enemies;
    HUD hud;
    sf::CircleShape drawPlayer;
    Tiles tiles;
    bool onFloor = false;
    bool doubleJump = false;
    bool isAttacking = false;
    sf::Clock clock;
    sf::Time lastFrameTime;
    ParalaxBG paralaxBG;
    float deltaTime = 0.0f;
    bool isPause = false;
    float difficulty = 0;

public:
    GameState(sf::RenderWindow& window);
    ~GameState();

    bool LoadRessources(sf::RenderWindow& window) override;
    void handleEvents(sf::RenderWindow& window, sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void setLoopManager(LoopManagement* manager);
    void isPaused(bool pausedState) override;
};

#endif