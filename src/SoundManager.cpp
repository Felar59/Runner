#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager() : volume(70.0f) {
    LoadResources();
}
SoundManager::~SoundManager() {
    soundBuffers.clear();
    sounds.clear();
}
void SoundManager::LoadResources() {
    // Load sound buffers and initialize sounds
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile("assets/Player/hurt.wav")) {
        soundBuffers["hurt"] = buffer;
        sounds["hurt"].setBuffer(soundBuffers["hurt"]);
    }
    if (buffer.loadFromFile("assets/Player/jump.wav")) {
        soundBuffers["jump"] = buffer;
        sounds["jump"].setBuffer(soundBuffers["jump"]);
    }
    if (buffer.loadFromFile("assets/Player/slash.wav")) {
        soundBuffers["slash"] = buffer;
        sounds["slash"].setBuffer(soundBuffers["slash"]);
    }
    if (buffer.loadFromFile("assets/Sounds/buttonPressed.wav")) {
        soundBuffers["button_click"] = buffer;
        sounds["button_click"].setBuffer(soundBuffers["button_click"]);
    }
    // Load music
    if (!music.openFromFile("assets/Sounds/BloodLeafFalling.mp3")) {
        std::cerr << "Error loading music file" << std::endl;
    } 
}

void SoundManager::playMusic() {
    music.setVolume(volume);
    music.setLoop(true);
    music.play();
}

void SoundManager::playSound(const std::string& soundName) {
    if (sounds.find(soundName) != sounds.end()) {
        sounds[soundName].setVolume(volume);
        sounds[soundName].play();
    } else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}