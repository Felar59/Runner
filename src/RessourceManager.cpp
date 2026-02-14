#include "RessourceManager.hpp"
#include <iostream>

RessourceManager::RessourceManager() {}
RessourceManager::~RessourceManager() {}

void RessourceManager::loadAnimationFrame(std::string_view animation, int frameIndex, const sf::Image& image, const sf::IntRect& frameRect) {
    std::string keyName(animation);

    // On évite de recharger si déjà présent
    if (textures[keyName].find(frameIndex) != textures[keyName].end()) {
        std::cout << "Frame " << frameIndex << " de l'animation " << animation << " déjà chargée." << std::endl;
        return;
    }

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromImage(image, frameRect)) {
        std::cerr << "Échec du chargement de la frame " << frameIndex << " pour " << animation << std::endl;
        return;
    }

    textures[keyName][frameIndex] = texture;
    // std::cout << "Frame " << frameIndex << " chargée pour " << animation << std::endl;
}

std::shared_ptr<sf::Texture> RessourceManager::getTextureFrame(std::string_view animation, int frameIndex) {
    std::string key(animation);

    auto animIt = textures.find(key);
    if (animIt != textures.end()) {
        auto frameIt = animIt->second.find(frameIndex);
        if (frameIt != animIt->second.end()) {
            return frameIt->second;
        }
    }

    // std::cerr << "!! Frame " << frameIndex << " non trouvée pour animation " << animation << std::endl;
    return nullptr;
}
