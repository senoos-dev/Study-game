#include "CombatSystem.hpp"
#include "Character.hpp"
#include "NPC.hpp"
#include "Logger.hpp"

using namespace std;

void CombatSystem::playerAttack(Character& player, NPC& enemy) {
    int dmg = player.getTotalDamage();
    enemy.takeDamage(dmg);
    Logger::log("Combat: player hit " + enemy.getName() + " for " + to_string(dmg));
}

void CombatSystem::enemyAttack(Character& player, const NPC& enemy) {
    int dmg = enemy.getDamage();
    player.attack_me(dmg);
    Logger::log("Combat: " + enemy.getName() + " hit player for " + to_string(dmg));
}

CombatState CombatSystem::runAuto(Character& player, NPC& enemy, ostream& out) {
    out << "\n=== Автобой с " << enemy.getName() << " ===\n";
    Logger::log("Combat auto started vs " + enemy.getName());

    int round = 1;
    while (player.get_health() > 0 && enemy.getHealth() > 0) {
        out << "Раунд " << round++ << "\n";
        playerAttack(player, enemy);
        if (enemy.getHealth() <= 0) {
            out << enemy.getName() << " повержен!\n";
            Logger::log("Combat auto: player won");
            return CombatState::PlayerWon;
        }
        enemyAttack(player, enemy);
        if (player.get_health() <= 0) {
            out << "Вы погибли...\n";
            Logger::log("Combat auto: player lost");
            return CombatState::PlayerLost;
        }
        out << "HP игрока: " << player.get_health() << ", HP врага: " << enemy.getHealth() << "\n";
    }
    return CombatState::PlayerLost;
}

CombatState CombatSystem::runTurnBased(Character& player, NPC& enemy, istream& in, ostream& out) {
    out << "\n=== Бой с " << enemy.getName() << " ===\n";
    Logger::log("Combat turn-based started vs " + enemy.getName());

    while (player.get_health() > 0 && enemy.getHealth() > 0) {
        out << "\nВаше HP: " << player.get_health()
            << " | " << enemy.getName() << " HP: " << enemy.getHealth()
            << " | урон: " << player.getTotalDamage() << "\n";
        out << "1) Удар  2) Зелье (ID)  3) Побег\nВыбор: ";

        int cmd = 0;
        in >> cmd;
        if (!in) return CombatState::PlayerFled;

        if (cmd == 1) {
            playerAttack(player, enemy);
            if (enemy.getHealth() <= 0) {
                out << "Победа!\n";
                return CombatState::PlayerWon;
            }
            enemyAttack(player, enemy);
            if (player.get_health() <= 0) {
                out << "Поражение...\n";
                return CombatState::PlayerLost;
            }
        } else if (cmd == 2) {
            out << "ID зелья: ";
            int potionId = 0;
            in >> potionId;
            player.use_potion(potionId);
            if (enemy.getHealth() <= 0) {
                out << "Победа!\n";
                return CombatState::PlayerWon;
            }
            enemyAttack(player, enemy);
            if (player.get_health() <= 0) {
                return CombatState::PlayerLost;
            }
        } else if (cmd == 3) {
            out << "Вы сбежали.\n";
            return CombatState::PlayerFled;
        }
    }
    return (player.get_health() > 0) ? CombatState::PlayerWon : CombatState::PlayerLost;
}