#ifndef COMBAT_SYSTEM_HPP
#define COMBAT_SYSTEM_HPP

#include <iostream>

#include "../Character.hpp"
#include "../NPC.hpp"

using namespace std;

enum class CombatAction {
    Attack,
    UsePotion,
    Flee
};

enum class CombatState {
    Ongoing,
    PlayerWon,
    PlayerLost,
    PlayerFled
};

class CombatSystem {
public:
    // Пошаговый бой (меню в консоли)
    static CombatState runTurnBased(Character& player,
                                    NPC& enemy,
                                    istream& in = cin,
                                    ostream& out = cout);

    // Автоматический бой до конца
    static CombatState runAuto(Character& player, NPC& enemy, ostream& out = cout);

    static void playerAttack(Character& player, NPC& enemy);
    static void enemyAttack(Character& player, const NPC& enemy);
};

#endif
