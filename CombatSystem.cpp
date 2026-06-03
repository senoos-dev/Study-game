#include "CombatSystem.hpp"

#include "Logger.hpp"

using namespace std;

void CombatSystem::playerAttack(Character& player, NPC& enemy) {
    int dmg = player.get_damage();
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
    while (player.is_alive() && enemy.isAlive()) {
        out << "Раунд " << round++ << "\n";
        playerAttack(player, enemy);
        if (!enemy.isAlive()) {
            out << enemy.getName() << " повержен!\n";
            Logger::log("Combat auto: player won");
            return CombatState::PlayerWon;
        }
        enemyAttack(player, enemy);
        if (!player.is_alive()) {
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

    while (player.is_alive() && enemy.isAlive()) {
        out << "\nВаше HP: " << player.get_health()
            << " | " << enemy.getName() << " HP: " << enemy.getHealth()
            << " | урон: " << player.get_damage() << "\n";
        out << "1) Удар  2) Зелье (ID)  3) Побег\nВыбор: ";

        int cmd = 0;
        in >> cmd;
        if (!in) {
            return CombatState::PlayerFled;
        }

        if (cmd == 1) {
            playerAttack(player, enemy);
            if (!enemy.isAlive()) {
                out << "Победа!\n";
                Logger::log("Combat: player won");
                return CombatState::PlayerWon;
            }
            enemyAttack(player, enemy);
            if (!player.is_alive()) {
                out << "Поражение...\n";
                Logger::log("Combat: player lost");
                return CombatState::PlayerLost;
            }
        } else if (cmd == 2) {
            out << "ID зелья: ";
            int potionId = 0;
            in >> potionId;
            player.use_poison(potionId);
            Logger::log("Combat: player used potion id " + to_string(potionId));
            enemyAttack(player, enemy);
            if (!player.is_alive()) {
                Logger::log("Combat: player lost");
                return CombatState::PlayerLost;
            }
        } else if (cmd == 3) {
            out << "Вы сбежали из боя.\n";
            Logger::log("Combat: player fled");
            return CombatState::PlayerFled;
        } else {
            out << "Неизвестная команда.\n";
        }
    }

    return player.is_alive() ? CombatState::PlayerWon : CombatState::PlayerLost;
}
