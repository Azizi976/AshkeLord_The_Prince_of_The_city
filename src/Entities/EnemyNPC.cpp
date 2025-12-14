#include "Entities/EnemyNPC.hpp"
#include "Entities/Player.hpp"
#include <iostream>

EnemyNPC::EnemyNPC()
    : NPC("Avi \"Bicep\"", "Bro, turn off the music? Are you crazy? FEEL THE PUMP!") {
    // Set color to yellow for enemy
    sprite_.setColor(sf::Color::Yellow);
}

void EnemyNPC::interact(Player& player) {
    std::cout << m_name << ": " << m_dialogue << std::endl;
    std::cout << "Battle Started!" << std::endl;
    // TODO: Change GameState to Battle when battle system is implemented
    // if (m_game) {
    //     m_game->setState(GameState::Battle);
    // }
}

