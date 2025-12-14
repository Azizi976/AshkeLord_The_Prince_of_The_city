#ifndef ENEMY_NPC_HPP
#define ENEMY_NPC_HPP

#include "Entities/NPC.hpp"

// Forward declaration
class Player;

/**
 * EnemyNPC - Avi "Bicep"
 * Mini-boss / Aggressive NPC
 */
class EnemyNPC : public NPC {
public:
    EnemyNPC();
    ~EnemyNPC() override = default;

    void interact(Player& player) override;
};

#endif // ENEMY_NPC_HPP

