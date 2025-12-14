#include "Entities/NPC.hpp"
#include <cmath>

NPC::NPC(const std::string& name, const std::string& dialogue)
    : m_name(name), m_dialogue(dialogue) {
    // NPCs don't move, so set speed to 0
    speed_ = 0.0f;
    
    // Create a simple colored rectangle as sprite (will be colored by derived classes)
    static sf::Texture npcTexture;
    if (npcTexture.getSize().x == 0) {
        npcTexture.create(32, 32);
        sf::Image img;
        img.create(32, 32, sf::Color::White);
        npcTexture.update(img);
    }
    sprite_.setTexture(npcTexture);
    auto bounds = sprite_.getLocalBounds();
    sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

void NPC::update(float deltaTime) {
    // NPCs are static, so no movement update needed
    // But we still need to update sprite position
    sprite_.setPosition(position_);
}

bool NPC::isPlayerNearby(const sf::Vector2f& playerPosition, float interactionDistance) const {
    float dx = position_.x - playerPosition.x;
    float dy = position_.y - playerPosition.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= interactionDistance;
}

