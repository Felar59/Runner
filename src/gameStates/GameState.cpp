#include "GameState.hpp"
#include "LoopManagement.hpp"
#include "Settings.hpp"
#include <iostream>

GameState::GameState(sf::RenderWindow& window) : tiles(window){
    std::cout << "Création de l'état Game" << std::endl;
};
GameState::~GameState(){
    std::cout << "Création de l'état Game" << std::endl;
};

bool GameState::LoadRessources(sf::RenderWindow& window) {
    // Load Player
    player = Player(3, {(400.0f/1920.0f) * window.getSize().x, 0.0f});
    player.LoadPlayer(window);
    
    //Load Enemy
    enemies.emplace_back(sf::Vector2f(-300.f, 500.f), "Idler", ressourceManager);
    enemies.emplace_back(sf::Vector2f(-300.f, 500.f), "Idler", ressourceManager);
    enemies.emplace_back(sf::Vector2f(-300.f, 500.f), "Idler", ressourceManager);
    enemies.emplace_back(sf::Vector2f(-300.f, 500.f), "Idler", ressourceManager);

    for(auto& enemy : enemies){
        enemy.LoadEnemy(window);
    }
    // Load tiles
    Tiles tiles(window);
    
    // Load paralax background
    paralaxBG.LoadRessources(window, false);
    
    // Load HUD
    hud.LoadRessources(window);


    // Start Music
    loopMain->callSoundManager("music");

    Settings datas;
    
    // start Clock
    clock.restart();
    lastFrameTime = clock.getElapsedTime();
    return true;
};

void GameState::handleEvents(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && (onFloor || doubleJump)) {
            // Jump
            loopMain->callSoundManager("jump");
            doubleJump = player.jump(doubleJump, onFloor);
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Attack
            loopMain->callSoundManager("slash");
            isAttacking = true;
        }
    }
};

void GameState::update() {
    // Set deltaTime
    sf::Time currentTime = clock.getElapsedTime();
    deltaTime = (currentTime - lastFrameTime).asSeconds();
    lastFrameTime = currentTime;

    // update paralax background
    paralaxBG.Update(deltaTime);

    // Move floor for runner
    tiles.moves(deltaTime, enemies);  
    
    // Gravity on player and collision
    onFloor = player.ApplyGravity(tiles.getVector(), deltaTime);
    if (onFloor) {
        doubleJump = true;
    }

    // Gravity on enemy and collision
    std::vector<std::pair<std::string ,sf::RectangleShape>> enemiesHitbox;
    enemiesHitbox.reserve(enemies.size());
    for(auto& enemy : enemies){
        enemy.ApplyGravity(tiles.getVector(), tiles.getSpeed(), deltaTime);
        // Enemy collision with player or sword
        enemy.collid(player.getHitbox(), player.getSwordHitbox(), isAttacking);
        
        enemiesHitbox.emplace_back(enemy.getHabit(), enemy.getHitbox());
    }
    
    // Player collision with enemy or wall
    if (player.collid(enemiesHitbox))
    {
        loopMain->callSoundManager("hurt");
    }   
    
    difficulty += deltaTime*10;
    hud.update(static_cast<int>(difficulty), static_cast<int>(difficulty/100), player.getHeatlh(), isPause);
};

void GameState::draw(sf::RenderWindow& window) {
    // Paralax background
    paralaxBG.Draw(window);
    
    // Change Skin enemy
    for(auto& enemy : enemies){
        enemy.updateSkin(deltaTime);
    }
    
    // Change Skin player
    player.updateSkin(isAttacking, deltaTime);

    // Enemy
    for(auto& enemy : enemies){
        enemy.draw(window);
    }
    
    // Player
    player.draw(window);
    
    // Tiles
    tiles.draw(window);    

    // HUD
    hud.draw(window);
};

void GameState::setLoopManager(LoopManagement* manager) {
    loopMain = manager;
}

void GameState::isPaused(bool pausedState) {
    if (isPause && !pausedState){
        // Reset the clock when unpausing
        clock.restart();
        lastFrameTime = clock.getElapsedTime();
    }
    isPause = pausedState;
}