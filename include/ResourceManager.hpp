#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

/**
 * ResourceManager - Singleton class for managing game resources (textures, fonts, etc.)
 * Uses RAII principles with smart pointers to avoid memory leaks
 */
class ResourceManager {
public:
    // Get the singleton instance
    static ResourceManager& getInstance();

    // Delete copy constructor and assignment operator (singleton pattern)
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /**
     * Load a texture from file and store it with an ID
     * @param id Unique identifier for the texture
     * @param filepath Path to the texture file
     * @return true if loaded successfully, false otherwise
     */
    bool loadTexture(const std::string& id, const std::string& filepath);

    /**
     * Get a texture by ID
     * @param id The texture identifier
     * @return Shared pointer to the texture, or nullptr if not found
     */
    std::shared_ptr<sf::Texture> getTexture(const std::string& id) const;

    /**
     * Load a font from file and store it with an ID
     * @param id Unique identifier for the font
     * @param filepath Path to the font file
     * @return true if loaded successfully, false otherwise
     */
    bool loadFont(const std::string& id, const std::string& filepath);

    /**
     * Get a font by ID
     * @param id The font identifier
     * @return Shared pointer to the font, or nullptr if not found
     */
    std::shared_ptr<sf::Font> getFont(const std::string& id) const;

    /**
     * Check if a texture exists
     */
    bool hasTexture(const std::string& id) const;

    /**
     * Check if a font exists
     */
    bool hasFont(const std::string& id) const;

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures_;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts_;
};

#endif // RESOURCE_MANAGER_HPP

