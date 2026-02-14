#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <unordered_map>

class SoundManager {
private:
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    sf::Music music;
    float volume;

public:
    SoundManager();
    ~SoundManager();

    void LoadResources();
    void playSound(const std::string& soundName);
    void playMusic();
    void setVolume(float volume);

};

#endif // SOUND_MANAGER_HPP