#ifndef MYSTIC_NPC_HPP
#define MYSTIC_NPC_HPP

#include "Entities/NPC.hpp"

// Forward declaration
class Player;

/**
 * MysticNPC - Baba-Rentgen
 * Provides buffs to the player
 */
class MysticNPC : public NPC {
public:
    MysticNPC();
    ~MysticNPC() override = default;

    void interact(Player& player) override;
};

#endif // MYSTIC_NPC_HPP

