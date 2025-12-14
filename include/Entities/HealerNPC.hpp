#ifndef HEALER_NPC_HPP
#define HEALER_NPC_HPP

#include "Entities/NPC.hpp"

// Forward declaration
class Player;

/**
 * HealerNPC - Miri "Kapara"
 * Restores player's Nervim (HP)
 */
class HealerNPC : public NPC {
public:
    HealerNPC();
    ~HealerNPC() override = default;

    void interact(Player& player) override;
};

#endif // HEALER_NPC_HPP

