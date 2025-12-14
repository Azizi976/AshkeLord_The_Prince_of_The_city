#include "Entities/MerchantNPC.hpp"
#include "Entities/Player.hpp"
#include <iostream>

MerchantNPC::MerchantNPC()
    : NPC("Zion \"Scissors\"", "Achi, I know where Liran is hiding. But first, 50 shekels for a fade cut.") {
    // Set color to green for merchant
    sprite_.setColor(sf::Color::Green);
}

void MerchantNPC::interact(Player& player) {
    std::cout << m_name << ": " << m_dialogue << std::endl;
    std::cout << "Shop Opened" << std::endl;
    // TODO: Implement shop UI
}

