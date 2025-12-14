#ifndef NPC_HPP
#define NPC_HPP

#include "Entities/Entity.hpp"
#include <string>

// Forward declaration
class Player;

/**
 * NPC - Abstract base class for all Non-Player Characters
 * Provides polymorphic interface for different NPC types
 */
class NPC : public Entity {
public:
    NPC(const std::string& name, const std::string& dialogue);
    virtual ~NPC() = default;

    /**
     * Update NPC state (called every frame)
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime) override;

    /**
     * Interact with the player (pure virtual - must be implemented by derived classes)
     * @param player Reference to the player character
     */
    virtual void interact(Player& player) = 0;

    // Getters
    const std::string& getName() const { return m_name; }
    const std::string& getDialogue() const { return m_dialogue; }

    /**
     * Check if player is close enough to interact
     * @param playerPosition Player's current position
     * @param interactionDistance Maximum distance for interaction
     * @return true if player is within interaction distance
     */
    bool isPlayerNearby(const sf::Vector2f& playerPosition, float interactionDistance = 50.0f) const;

protected:
    std::string m_name;      // NPC's name
    std::string m_dialogue;  // NPC's dialogue text
};

#endif // NPC_HPP

