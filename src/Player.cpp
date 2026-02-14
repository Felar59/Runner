#include "Player.hpp"
#include "Collision.hpp"
#include <iostream>

Player::Player() : health(3), pos({0.0f, 0.0f}) {
    // Valeurs par défaut
}

Player::Player(int hp, sf::Vector2f beginPos){
    health = hp;
    pos = beginPos;
};

Player::~Player(){
};

// Getter
sf::Vector2f Player::getPos(){
    return pos;
};
int Player::getHeatlh(){
    return health;
};
sf::RectangleShape Player::getSwordHitbox(){
    return swordHitbox;
};
sf::RectangleShape Player::getHitbox(){
    return hitboxPlayer;
};

// Setter
void Player::setPos(sf::Vector2f newPos){
    pos = newPos;
};
void Player::setHealth(int newHealth){
    health = newHealth;
};

// Others
void Player::LoadPlayer(sf::RenderWindow& window){
    const std::vector<std::pair<std::vector<sf::Vector2f>, std::vector<sf::Vector2f>>> cordsAndPos = 
    {
        { // RUN
            {{ {0, 0}, {21, 0}, {43, 0}, {64, 0} }}, // cords
            {{ {20, 28}, {21, 28}, {20, 28}, {21, 28} }} // pos
        },
        { // JUMP
            {{ {0, 0}, {22, 0}, {45, 0}, {70, 0} }},
            {{ {22, 29}, {22, 29}, {24, 29}, {24, 29} }}
        },
        { // ATTACK
            {{ {0, 0}, {23, 0}, {58, 0}, {91, 0} }},
            {{ {22, 29}, {34, 29}, {32, 29}, {22, 29} }}
        },
        { // Idle
            {{ {0, 0}, {20, 0}, {40, 0}, {60, 0} }},
            {{ {19, 30}, {19, 30}, {19, 30}, {18, 30} }}
        },
        { // Death
            {{ {0, 0}, {20, 0}, {41, 0}, {61, 0}, {82, 0}, {103, 0} }},
            {{ {19, 31}, {20, 31}, {19, 31}, {21, 31}, {20, 31}, {19, 31} }}
        }, 
        { // Damaged
            {{ {0, 0}, {20, 0}, {40, 0} }},
            {{ {19, 29}, {19, 29}, {19, 29} }}
        }
    };
    
    const std::vector<std::pair<int, std::string>> tileSetPath = {{0, "Run"}, {1, "Jump"}, {2, "Attack"},{3, "Idle"},{4, "Death"},{5, "Damaged"}};
    
    for (size_t i = 0; i < tileSetPath.size(); i++) {
        sf::Image currentTileset;
        if (!currentTileset.loadFromFile("assets/Player/" + tileSetPath[i].second + ".png")) {
            std::cerr << "Error loading " + tileSetPath[i].second + ".png" << std::endl;
        }
        AnimationType currentType = static_cast<AnimationType>(i);
        animations[currentType].speed = 0.8f / cordsAndPos[tileSetPath[i].first].first.size(); // Proportion for 0.1f for 8 frames
        animations[currentType].textures.resize(cordsAndPos[tileSetPath[i].first].first.size());
        animations[currentType].sprites.resize(cordsAndPos[tileSetPath[i].first].first.size());
        // Apply the texture to the sprite
        for (size_t j = 0; j < cordsAndPos[tileSetPath[i].first].first.size(); j++) {
            sf::Texture tempTexture;
            if (!tempTexture.loadFromImage(currentTileset,
                sf::IntRect(cordsAndPos[tileSetPath[i].first].first[j].x, 
                    cordsAndPos[tileSetPath[i].first].first[j].y, 
                    cordsAndPos[tileSetPath[i].first].second[j].x, 
                    cordsAndPos[tileSetPath[i].first].second[j].y)))
            {
                std::cerr << "Erreur chargement texture frame " << j << std::endl;
            }
            animations[currentType].textures[j] = tempTexture;
            sf::Sprite tempSprite;
            tempSprite.setTexture(animations[currentType].textures[j]);
            float scale = window.getSize().x / 20.f / animations[currentType].textures[j].getSize().x;
            if (currentType == AnimationType::ATTACK){
                if (j == 1 or j == 2){
                    animations[currentType].speed = 0.1f;
                    tempSprite.setScale(scale*1.5, scale*1.5);    

                } else {
                    tempSprite.setScale(scale, scale);    
                }
            } else{
                tempSprite.setScale(scale, scale);    
            }
            animations[currentType].sprites[j] = tempSprite;
        }
    }
    currentSkin = animations[AnimationType::IDLE].sprites[0];
    hitboxPlayer = sf::RectangleShape(sf::Vector2f(animations[AnimationType::RUN].sprites[0].getGlobalBounds().width, animations[AnimationType::RUN].sprites[0].getGlobalBounds().height - ((window.getSize().y / 9)/16))); // (window.getSize().y / 9)/16 to fit with the texture and get ur feet in the ground
    hitboxPlayer.setFillColor(sf::Color::Transparent);
    hitboxPlayer.setOutlineColor(sf::Color::Red);
    hitboxPlayer.setOutlineThickness(1.0f);
    hitboxPlayer.setPosition(pos.x, pos.y);
    swordHitbox.setSize(sf::Vector2f(animations[AnimationType::ATTACK].sprites[1].getGlobalBounds().width - animations[AnimationType::ATTACK].sprites[0].getGlobalBounds().width, animations[AnimationType::ATTACK].sprites[1].getGlobalBounds().height));
    swordHitbox.setPosition(pos.x + animations[AnimationType::ATTACK].sprites[0].getGlobalBounds().width, pos.y);
    swordHitbox.setFillColor(sf::Color::Transparent);
    swordHitbox.setOutlineColor(sf::Color::Magenta);
    swordHitbox.setOutlineThickness(1.0f);
}
        
bool Player::ApplyGravity(std::vector<std::vector <Tile>>& tiles, float deltaTime){
    Velocity += gravity * deltaTime;
    pos.y += Velocity * deltaTime;

    hitboxPlayer.setPosition(pos.x, pos.y);

    for (size_t y = 0; y < tiles.size(); y++){
        for (size_t x = 3; x < 5; x++){ // Block around the player
            if (tiles[y][x].getRect().getSize().x == 0 || tiles[y][x].getRect().getSize().y == 0) continue;

            sf::RectangleShape tileShape = tiles[y][x].getRect();
            if(RectRectCollision(hitboxPlayer, tileShape)){
                Velocity = 0;
                pos.y = tileShape.getPosition().y - hitboxPlayer.getGlobalBounds().height + 1.f; // +1 for issue with windows to be sure of being on the tile and not slightly over which make us fall in a while.
                hitboxPlayer.setPosition(pos.x, pos.y); 
                return true;
            }
        } 
    }
    return false;
}


bool Player::jump(bool doubleJump, bool onFloor){
    Velocity = -17.f/0.016f;
    if(!doubleJump){
        return true;
    } else if (!onFloor)
    {
        return false;
    }
    return true;
}
 
void Player::draw(sf::RenderWindow& window){
    window.draw(currentSkin);
    // window.draw(hitboxPlayer);
    // window.draw(swordHitbox);
}

void Player::changeSkinFlying(float deltaTime) {
    if (Velocity < -10/0.016f){
        currentSkin.setTexture(animations[AnimationType::JUMP].textures[0], true);
        currentSkin.setScale(animations[AnimationType::JUMP].sprites[0].getScale());
    } else if (Velocity < -4/0.016f) {
        currentSkin.setTexture(animations[AnimationType::JUMP].textures[1], true);
        currentSkin.setScale(animations[AnimationType::JUMP].sprites[1].getScale());
    } else if (Velocity < 6/0.016f) {
        currentSkin.setTexture(animations[AnimationType::JUMP].textures[2], true);
        currentSkin.setScale(animations[AnimationType::JUMP].sprites[2].getScale());
    } else if (Velocity > 0){
        currentSkin.setTexture(animations[AnimationType::JUMP].textures[3], true);
        currentSkin.setScale(animations[AnimationType::JUMP].sprites[3].getScale());
    }
}

void Player::changeSkin(AnimationType type, float deltaTime) {
    animations[type].elapsedTime += deltaTime;
    if (animations[type].elapsedTime >= animations[type].speed) {
        animations[type].currentFrame = (animations[type].currentFrame + 1) % animations[type].textures.size();
        currentSkin.setTexture(animations[type].textures[animations[type].currentFrame], true);
        currentSkin.setScale(animations[type].sprites[animations[type].currentFrame].getScale());
        animations[type].elapsedTime = 0.f;
    } else {
        currentSkin.setTexture(animations[type].textures[animations[type].currentFrame], true);
        currentSkin.setScale(animations[type].sprites[animations[type].currentFrame].getScale());
    }
}

void Player::updateSkin(bool& isAttacking, float deltaTime){
    if (damaged){ // Damaged
        changeSkin(AnimationType::DAMAGED, deltaTime);
        if (animations[AnimationType::DAMAGED].currentFrame == static_cast<int>(animations[AnimationType::DAMAGED].textures.size()) - 1){
            damaged = false;
            animations[AnimationType::DAMAGED].currentFrame = 0;
        }
    } else if (isAttacking){ // Attacking
        changeSkin(AnimationType::ATTACK, deltaTime);
        if (animations[AnimationType::ATTACK].currentFrame == static_cast<int>(animations[AnimationType::ATTACK].textures.size()) - 1){
            isAttacking = false;
            animations[AnimationType::ATTACK].currentFrame = 0;
        }
    } else if (Velocity != 0){ // Not on ground
        changeSkinFlying(deltaTime);
    } else{ // Running
        changeSkin(AnimationType::RUN, deltaTime);
    }
    hitboxPlayer.setPosition(pos.x, pos.y);
    swordHitbox.setPosition(pos.x + animations[AnimationType::ATTACK].sprites[0].getGlobalBounds().width, pos.y);
    currentSkin.setPosition(pos.x, pos.y);
}

bool Player::collid(const std::vector<std::pair<std::string ,sf::RectangleShape>>& HitboxAndHabit){
    for(auto& pair : HitboxAndHabit){
        if(RectRectCollision(hitboxPlayer, pair.second) && pair.first != "Death" && damaged != true){
            health--;
            damaged = true;
            return true;
        }
    }
    return false;
}