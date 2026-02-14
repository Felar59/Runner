#include "Settings.hpp"
#include <filesystem> 
namespace fs = std::filesystem;


Settings::Settings() {
    std::string folderPath = "data";
    std::string filePath = folderPath + "/settings.json";

    // Créer le dossier s’il n’existe pas
    if (!fs::exists(folderPath)) {
        fs::create_directories(folderPath);
        std::cout << "Dossier 'data' créé." << std::endl;
    }

    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cout << "Fichier introuvable. Création du fichier settings.json par défaut." << std::endl;

        // Valeurs par défaut
        data = {
            {"volume", 50},
            {"fullscreen", true},
            {"FPS", 120}
        };

        // Crée le fichier avec les données
        std::ofstream outFile(filePath);
        if (outFile.is_open()) {
            outFile << data.dump(4); // indenté
            outFile.close();
            std::cout << "Fichier settings.json créé avec succès." << std::endl;
        } else {
            std::cout << "Erreur : impossible de créer le fichier settings.json." << std::endl;
        }
    } else {
        std::cout << "Fichier settings.json trouvé. Lecture en cours..." << std::endl;
        file >> data;
        file.close();
    }

    // Affiche les données chargées
    std::cout << "Contenu du fichier JSON :" << std::endl;
    std::cout << data.dump(4) << std::endl;
}

Settings::~Settings() {
};

// Getter
int Settings::getVolume() const {
    return data.value("volume", 50); // Valeur par défaut de 50 si la clé n'existe pas
}
bool Settings::isFullscreen() const {
    return data.value("fullscreen", true); // Valeur par défaut de true si la clé n'existe pas
}
int Settings::getFPS() const {
    return data.value("FPS", 120); // Valeur par défaut de 120 si la clé n'existe pas
}

// Setter
void Settings::setVolume(int volume) {
    data["volume"] = volume;
    saveSettings();
}

void Settings::setFullscreen(bool fullscreen) {
    data["fullscreen"] = fullscreen;
    saveSettings();
}

void Settings::setFPS(int fps) {
    data["FPS"] = fps;
    saveSettings();
}

// Save settings to file

void Settings::saveSettings() {
    std::string folderPath = "data";
    std::string filePath = folderPath + "/settings.json";

    std::ofstream file(filePath);

    if (!file.is_open()) {
        std::cout << "Erreur : impossible d'ouvrir le fichier settings.json pour l'écriture." << std::endl;
        return;
    } else {
        file << data.dump(4);
        file.close();
        std::cout << "Paramètres sauvegardés dans settings.json." << std::endl;
    }
    std::cout << "Contenu du fichier JSON après sauvegarde :" << std::endl;
    std::cout << data.dump(4) << std::endl;
}

// Settings GUI
void Settings::LoadRessources(sf::RenderWindow& window,  sf::Font& font) {
    // This function would load textures for the settings GUI
    // For example, loading buttons or backgrounds
    // Here we just print a message for demonstration purposes
    std::cout << "Loading textures for settings GUI..." << std::endl;
}

void Settings::handleEvents(sf::RenderWindow& window, sf::Event& event) {
    return;
}

void Settings::draw(sf::RenderWindow& window, std::map<ButtonType, button> buttons) {
    this->buttons = buttons;
    buttonsName[0] = "FPS";
    buttonsName[1] = "Fullscreen";
    buttonsName[2] = "Volume";


    for (int i = 0; i < 3; i++){
        ButtonType newButton = static_cast<ButtonType>(i);
        buttons[newButton].text.setString(buttonsName[i]);
    }

    for (const auto& button : buttons) {
        window.draw(button.second.onOffSprite[button.second.hovered ? 0 : 1]);
        window.draw(button.second.text);
    }
}