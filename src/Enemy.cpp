#include <Enemy.hpp>
#include <iostream>
#include <utility>
#include <random>
#include "Collision.hpp"

Enemy::Enemy(sf::Vector2f beginPos, std::string habit, RessourceManager& ressourceManager)
: pos(beginPos), enemyHabit(habit), ressourceManager(ressourceManager) {
};

Enemy::~Enemy() {
}

// Getter
sf::Vector2f Enemy::getPos() {
    return pos;
}

sf::RectangleShape Enemy::getHitbox() {
    return hitboxEnemy;
}

std::string Enemy::getHabit() {
    return enemyHabit;
}

// Setter
void Enemy::setPos(sf::Vector2f newPos) {
    pos = newPos;
}

// Others
void Enemy::LoadEnemy(sf::RenderWindow& window){
    const int sizeCatTiles = 32;
    const float scale = (window.getSize().x / 16.f) / sizeCatTiles; // Scale for enemy with tile size

    // Load the textures and create the sprites
    for (size_t i = 0; i < tileSetPath.size(); i++) {
        sf::Image currentTileset;
        if (!currentTileset.loadFromFile("assets/Enemy/" + tileSetPath[i].second + ".png")) {
            std::cerr << "Error loading " + tileSetPath[i].second + ".png" << std::endl;
        }
        AnimationType currentType = static_cast<AnimationType>(i);
        animations[currentType].speed = 0.8f / tileSetPath[i].first; // Proportion for 0.1f for 8 frames
        animations[currentType].sprites.resize(tileSetPath[i].first);
        // Apply the texture to the sprite
        for (int j = 0; j < tileSetPath[i].first; j++) {
            sf::Sprite tempSprite;
            ressourceManager.loadAnimationFrame((tileSetPath[i].second), j, currentTileset, sf::IntRect(j*sizeCatTiles, 0, sizeCatTiles, sizeCatTiles));
            tempSprite.setTexture(*ressourceManager.getTextureFrame(tileSetPath[i].second, j));
            tempSprite.setScale(-scale, scale);
            animations[currentType].sprites[j] = tempSprite;
        }
    }
    currentSkin = animations[AnimationType::IDLE].sprites[0];
    hitboxEnemy = sf::RectangleShape(sf::Vector2f(sizeCatTiles * scale, sizeCatTiles*scale)); // (window.getSize().y / 9)/16 to fit with the texture and get ur feet in the ground
    hitboxEnemy.setOutlineColor(sf::Color::Red);
    hitboxEnemy.setFillColor(sf::Color::Transparent);
    hitboxEnemy.setOutlineThickness(1.0f);
    hitboxEnemy.setPosition(pos.x, pos.y);
}

void Enemy::ApplyGravity(std::vector<std::vector <Tile>>& tiles, float GroundSpeed, float deltaTime){
    if (pos.x > -hitboxEnemy.getGlobalBounds().width*2) {
        pos.x -= GroundSpeed * deltaTime;
        if (enemyHabit == "Walker" || enemyHabit == "Jumper") {
            Velocity += gravity * deltaTime;
            pos.y += Velocity * deltaTime;
            if (enemyHabit == "Walker") {
                pos.x -= GroundSpeed * deltaTime / 2;
            }
        }
        
        hitboxEnemy.setPosition(pos.x, pos.y);
        
        if (enemyHabit != "Walker"){
            if (pos.y > floorLimit) {
                Velocity = 0;
                pos.y = floorLimit;
            }
        } else {
            for (size_t y = 0; y < tiles.size(); y++){
                for (size_t x = 0; x < tiles[y].size(); x++){
                    if (tiles[y][x].getRect().getSize().x == 0 || tiles[y][x].getRect().getSize().y == 0) continue;
                    
                    sf::RectangleShape tileShape = tiles[y][x].getRect();
                    if(RectRectCollision(hitboxEnemy, tileShape)){
                        Velocity = 0;
                        pos.y = tileShape.getPosition().y - hitboxEnemy.getGlobalBounds().height;
                        hitboxEnemy.setPosition(pos.x, pos.y); 
                    }
                } 
            }
        }
    }
}

void Enemy::changeSkin(AnimationType type, float deltaTime) {
    animations[type].elapsedTime += deltaTime;
    if (animations[type].elapsedTime >= animations[type].speed) {
        animations[type].currentFrame = (animations[type].currentFrame + 1) % tileSetPath[static_cast<int>(type)].first;
        currentSkin.setTexture(*ressourceManager.getTextureFrame(tileSetPath[static_cast<int>(type)].second, animations[type].currentFrame));
        animations[type].elapsedTime = 0.f;
    }
}

void Enemy::updateSkin(float deltaTime) {
    if(pos.x < 1920.f && HasBorn == false) {
        lastHabit = enemyHabit;
        changeSkin(AnimationType::BORN, deltaTime);
        if (animations[AnimationType::BORN].currentFrame == tileSetPath[static_cast<int>(AnimationType::BORN)].first - 1) {
            animations[AnimationType::BORN].currentFrame = 0;
            enemyHabit = lastHabit;
            HasBorn = true;
        }
    } else{
        if (enemyHabit == "Idler") {
            changeSkin(AnimationType::IDLE, deltaTime);
        } else if (enemyHabit == "Sleeper") {
            changeSkin(AnimationType::SLEEP, deltaTime);
        } else if (enemyHabit == "Jumper") {
            changeSkin(AnimationType::JUMP, deltaTime);
            if (animations[AnimationType::JUMP].currentFrame == 0 && Velocity == 0) {
                Velocity = -16.f/0.016f;
            }
        } else if (enemyHabit == "Walker"){
            changeSkin(AnimationType::WALK, deltaTime);
        } else if (enemyHabit == "Death") {
            if (animations[AnimationType::DEATH].currentFrame != tileSetPath[static_cast<int>(AnimationType::DEATH)].first - 1) {
                changeSkin(AnimationType::DEATH, deltaTime);
            }
        }
    }
    currentSkin.setPosition(pos.x + hitboxEnemy.getGlobalBounds().width, pos.y);
    hitboxEnemy.setPosition(pos.x, pos.y);
}

void Enemy::draw(sf::RenderWindow& window){
    window.draw(currentSkin);
    // window.draw(hitboxEnemy);
}

void Enemy::summonEnemy(sf::Vector2f summoningPos, float habitFactor){
    if(pos.x < -hitboxEnemy.getGlobalBounds().width*2) {
        if (rand() % 100 < habitFactor*habitFactor) {
            sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
            pos = {summoningPos.x, summoningPos.y + (desktopMode.height / 9.f / 16.f)}; // slightly in the ground (in the grass)

            // Chance of summoning stronger ennemy based on habitFactor
            size_t timeConsecuence = static_cast<size_t>(habitFactor * 2);

            if (timeConsecuence > 2){
                timeConsecuence = 2;
            }
            size_t index = timeConsecuence + (rand() % 2) % (possibleHabits.size());
            enemyHabit = possibleHabits[index];

            if (enemyHabit != "Walker") {
                floorLimit = pos.y + (desktopMode.height / 9.f / 16.f);
            }

            // Rest born animation and death one
            HasBorn = false;
            animations[AnimationType::DEATH].currentFrame = 0;
        }
    }
}

void Enemy::collid(const sf::RectangleShape& playerHitbox, const sf::RectangleShape& swordHitbox, const bool& isAttacking) {
    if (RectRectCollision(hitboxEnemy, swordHitbox) && isAttacking) {
        enemyHabit = "Death";
        floorLimit = pos.y;
    }
}