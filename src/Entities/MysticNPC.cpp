#include "Entities/MysticNPC.hpp"
#include "Entities/Player.hpp"
#include <iostream>

MysticNPC::MysticNPC()
    : NPC("Baba-Rentgen", "The Arak speaks to me... Take this red string against Ayin Hara.") {
    // Set color to magenta/purple for mystic
    sprite_.setColor(sf::Color::Magenta);
}

void MysticNPC::interact(Player& player) {
    std::cout << m_name << ": " << m_dialogue << std::endl;
    player.increaseDamage(10);
    std::cout << "You feel stronger! Damage increased." << std::endl;
}

