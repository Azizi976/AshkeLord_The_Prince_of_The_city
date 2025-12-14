#ifndef QUEST_NPC_HPP
#define QUEST_NPC_HPP

#include "Entities/NPC.hpp"

// Forward declaration
class Player;

/**
 * QuestNPC - Yotam "The Tel-Avivian"
 * Distress NPC that starts quests
 */
class QuestNPC : public NPC {
public:
    QuestNPC();
    ~QuestNPC() override = default;

    void interact(Player& player) override;
};

#endif // QUEST_NPC_HPP

