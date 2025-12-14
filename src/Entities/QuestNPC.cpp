#include "Entities/QuestNPC.hpp"
#include "Entities/Player.hpp"
#include <iostream>

QuestNPC::QuestNPC()
    : NPC("Yotam \"The Tel-Avivian\"", "Excuse me? Do you have Oat Milk? I'm lost...") {
    // Set color to cyan for quest NPC
    sprite_.setColor(sf::Color::Cyan);
}

void QuestNPC::interact(Player& player) {
    std::cout << m_name << ": " << m_dialogue << std::endl;
    std::cout << "Quest Started: Escape from Periphery" << std::endl;
    // TODO: Implement quest system
}

