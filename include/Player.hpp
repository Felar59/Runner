#ifndef Player_hpp
#define Player_hpp

#include <SFML/Graphics.hpp>
#include <vector>
#include "Tiles.hpp"
#include "Tile.hpp"

class Player
{
private:
    int health;
    sf::Vector2f pos;
    float Velocity = 0.0f;
    float gravity = 60.0f/0.016f;
    sf::RectangleShape hitboxPlayer;
    struct Animation {
        std::vector<sf::Texture> textures;
        std::vector<sf::Sprite> sprites;
        float speed;
        int currentFrame = 0;
        float elapsedTime = 0.0f;
    };
    enum class AnimationType {
        RUN,
        JUMP,
        ATTACK,
        IDLE,
        DEATH,
        DAMAGED
    };
    std::map<AnimationType, Animation> animations;
    
    sf::Sprite currentSkin;
    sf::RectangleShape swordHitbox;
    bool damaged = false;

public:
    Player(); 
    Player(int health, sf::Vector2f pos);
    ~Player();
    // Getter
    sf::Vector2f getPos();
    int getHeatlh();
    sf::RectangleShape getSwordHitbox();
    sf::RectangleShape getHitbox();

    // Setter
    void setPos(sf::Vector2f newPos);
    void setHealth(int newHealth);

    // Others
    void LoadPlayer(sf::RenderWindow& window);
    bool ApplyGravity(std::vector<std::vector <Tile>>& tiles, float deltaTime);
    bool jump(bool doubleJump, bool onFloor);
    void draw(sf::RenderWindow& window);
    void updateSkin(bool& isAttacking, float deltaTime);
    void changeSkin(AnimationType type, float deltaTime);
    void changeSkinFlying(float deltaTime);
    bool collid(const std::vector<std::pair<std::string ,sf::RectangleShape>>& HitboxAndHabit);
};

#endif