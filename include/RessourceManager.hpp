#ifndef RESSOURCE_MANAGER_HPP
#define RESSOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class RessourceManager
{
private:
    std::unordered_map<std::string, std::unordered_map<int, std::shared_ptr<sf::Texture>>> textures;

public:
    RessourceManager();
    ~RessourceManager();

    void loadAnimationFrame(std::string_view animation, int frameIndex, const sf::Image& image, const sf::IntRect& frameRect); 
    std::shared_ptr<sf::Texture> getTextureFrame(std::string_view animation, int frameIndex);
};

#endif