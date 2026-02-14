#ifndef ENEMY_hpp
#define ENEMY_hpp

#include <SFML/Graphics.hpp>
#include <vector>
#include "Tiles.hpp"
#include "RessourceManager.hpp"

class Enemy
{
private:
    sf::Vector2f pos;
    float Velocity = 0.0f;
    float gravity = 50.0f/0.016f;
    float floorLimit = 0.f;
    sf::RectangleShape hitboxEnemy;
    std::string enemyHabit;
    
    struct Animation
    {
        std::vector<sf::Sprite> sprites;
        float speed;
        int currentFrame = 0;
        float elapsedTime = 0.0f;
    };
    enum class AnimationType
    {
        IDLE,
        BORN,
        SLEEP,
        JUMP,
        WALK,
        DEATH
    };
    const std::vector<std::pair<int, std::string>> tileSetPath = 
        {{8, "Idle"},
        {8, "Born"}, 
        {8, "Sleep"},
        {13, "Jump"},
        {4, "Walk"},
        {15, "Death"}};
    std::map<AnimationType, Animation> animations;
    sf::Sprite currentSkin;
    const std::vector<std::string_view> possibleHabits = {"Sleeper", "Idler", "Jumper", "Walker"};
    std::string lastHabit;
    bool HasBorn = false;
    RessourceManager& ressourceManager;

public:
    Enemy(sf::Vector2f beginPos, std::string enemyHabit, RessourceManager& ressourceManager);
    ~Enemy();

    // Getter
    sf::Vector2f getPos();
    sf::RectangleShape getHitbox();
    std::string getHabit();

    // Setter
    void setPos(sf::Vector2f newPos);

    // Others
    void LoadEnemy(sf::RenderWindow& window);
    void ApplyGravity(std::vector<std::vector <Tile>>& tiles, float GroundSpeed, float deltaTime);
    void changeSkin(AnimationType type, float deltaTime);
    void updateSkin(float deltaTime);
    void draw(sf::RenderWindow& window);
    void summonEnemy(sf::Vector2f summoningPos, float habitFactor);
    void collid(const sf::RectangleShape& playerHitbox, const sf::RectangleShape& swordHitbox, const bool& isAttacking);
};

#endif