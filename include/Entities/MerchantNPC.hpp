#ifndef MERCHANT_NPC_HPP
#define MERCHANT_NPC_HPP

#include "Entities/NPC.hpp"

// Forward declaration
class Player;

/**
 * MerchantNPC - Zion "Scissors"
 * Shopkeeper / Info broker
 */
class MerchantNPC : public NPC {
public:
    MerchantNPC();
    ~MerchantNPC() override = default;

    void interact(Player& player) override;
};

#endif // MERCHANT_NPC_HPP

