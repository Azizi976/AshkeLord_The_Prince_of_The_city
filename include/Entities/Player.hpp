#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entities/Entity.hpp"
#include <SFML/Window/Keyboard.hpp>

/**
 * Player - The protagonist "Ars" character
 * Handles player input and movement
 */
class Player : public Entity {
public:
    Player();
    ~Player() override = default;

    /**
     * Update player state based on input
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime) override;

    /**
     * Handle keyboard input for movement
     * Supports both WASD and Arrow keys
     */
    void handleInput();

    // Player stats (Ars-themed)
    int getStreetCred() const { return streetCred_; }
    void addStreetCred(int amount) { streetCred_ += amount; }
    
    int getNervimLevel() const { return nervimLevel_; }
    void setNervimLevel(int level) { nervimLevel_ = level; }
    
    // Player actions
    void heal(int amount);
    void increaseDamage(int amount);
    
    int getDamage() const { return damage_; }

private:
    sf::Vector2f velocity_; // Current movement velocity
    int streetCred_;        // Experience points (street credibility)
    int nervimLevel_;       // Energy/mana level (nervim = nerves/energy in Hebrew slang)
    int maxNervim_;         // Maximum nervim level
    int damage_;            // Player damage stat
};

#endif // PLAYER_HPP

