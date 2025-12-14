#include "Entities/HealerNPC.hpp"
#include "Entities/Player.hpp"
#include <iostream>

HealerNPC::HealerNPC() 
    : NPC("Miri \"Kapara\"", "Kapara alai'ch, take some Cuscus. You look like a skeleton.") {
    // Set color to red for healer
    sprite_.setColor(sf::Color::Red);
}

void HealerNPC::interact(Player& player) {
    std::cout << m_name << ": " << m_dialogue << std::endl;
    player.heal(100);
    std::cout << "You feel refreshed! Nervim restored." << std::endl;
}

