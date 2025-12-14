#include "ResourceManager.hpp"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::loadTexture(const std::string& id, const std::string& filepath) {
    // Check if texture already exists
    if (hasTexture(id)) {
        std::cerr << "Warning: Texture with ID '" << id << "' already exists." << std::endl;
        return false;
    }

    // Create new texture
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filepath)) {
        std::cerr << "Error: Failed to load texture from '" << filepath << "'" << std::endl;
        return false;
    }

    textures_[id] = texture;
    return true;
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& id) const {
    auto it = textures_.find(id);
    if (it != textures_.end()) {
        return it->second;
    }
    return nullptr;
}

bool ResourceManager::loadFont(const std::string& id, const std::string& filepath) {
    // Check if font already exists
    if (hasFont(id)) {
        std::cerr << "Warning: Font with ID '" << id << "' already exists." << std::endl;
        return false;
    }

    // Create new font
    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile(filepath)) {
        std::cerr << "Error: Failed to load font from '" << filepath << "'" << std::endl;
        return false;
    }

    fonts_[id] = font;
    return true;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const std::string& id) const {
    auto it = fonts_.find(id);
    if (it != fonts_.end()) {
        return it->second;
    }
    return nullptr;
}

bool ResourceManager::hasTexture(const std::string& id) const {
    return textures_.find(id) != textures_.end();
}

bool ResourceManager::hasFont(const std::string& id) const {
    return fonts_.find(id) != fonts_.end();
}

