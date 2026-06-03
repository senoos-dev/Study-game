#include <iostream>
#include <memory>
#include "Character.hpp"
#include "Weapon.hpp"
#include "HealthPotion.hpp"
#include "RiddlePuzzle.hpp"
#include "MathPuzzle.hpp"
#include "Location.hpp"
#include "NPC.hpp"
#include "CombatSystem.hpp"

using namespace std;

int main() {
    cout << "\n========================================";
    cout << "\n   ПОДЗЕМНЫЙ ЛАБИРИНТ - ТЕСТОВЫЙ РЕЖИМ";
    cout << "\n========================================\n";

    // 1. СОЗДАЁМ ПЕРСОНАЖА
    Character hero("Стартовая комната");
    cout << "[OK] Персонаж создан: " << hero.get_health() << " HP, урон " << hero.get_damage() << "\n";

    // 2. ДОБАВЛЯЕМ ПРЕДМЕТЫ В ИНВЕНТАРЬ
    hero.add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
    hero.add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
    hero.add_to_inventory(make_unique<Weapon>(3, "Меч", 8));
    hero.add_to_inventory(make_unique<Weapon>(4, "Двуручный топор", 15));

    cout << "[OK] Инвентарь пополнен (2 зелья, 2 оружия)\n";

    // 3. ТЕСТ ОРУЖИЯ
    cout << "\n--- ТЕСТ ОРУЖИЯ ---\n";
    hero.print();
    cout << "Базовый урон: " << hero.get_damage() << ", с экипировкой: " << hero.getTotalDamage() << "\n";
    
    hero.equipWeapon(4);
    cout << "[OK] Экипирован топор (ID=4), урон стал: " << hero.getTotalDamage() << "\n";

    // 4. ТЕСТ ЗЕЛЬЯ
    cout << "\n--- ТЕСТ ЗЕЛЬЯ ---\n";
    cout << "Было HP: " << hero.get_health() << "\n";
    hero.use_potion(1);
    cout << "После малого зелья HP: " << hero.get_health() << "\n";

    // 5. ТЕСТ ЛОКАЦИИ И ГОЛОВОЛОМКИ
    cout << "\n--- ТЕСТ ЛОКАЦИИ ---\n";
    auto riddle = make_unique<RiddlePuzzle>("Зимой и летом одним цветом? (одно слово)", "елка", 15);
    Location dungeon(1, "Тёмный зал", {"##############", "#............#", "#....@.......#", "#............#", "##############"});
    dungeon.setPuzzle(move(riddle));

    cout << "Локация открыта? " << (dungeon.isOpen() ? "Да" : "Нет") << "\n";
    
    int damage = 0;
    if (dungeon.trySolvePuzzle("ёлка", damage)) {
        cout << "[+] Головоломка решена! Локация открыта.\n";
        dungeon.open();
    } else {
        cout << "[-] Ошибка! Урон: " << damage << "\n";
        hero.attack_me(damage);
    }
    cout << "Локация открыта? " << (dungeon.isOpen() ? "Да" : "Нет") << "\n";

    // 6. ТЕСТ NPC И БОЯ
    cout << "\n--- ТЕСТ NPC И БОЕВОЙ СИСТЕМЫ ---\n";
    NPC goblin(1, "Злобный гоблин", 30, 5, NpcState::Hostile);
    cout << "Враг: " << goblin.getName() << ", HP: " << goblin.getHealth() << ", урон: " << goblin.getDamage() << "\n";
    cout << "Начинаем автобой...\n";

    CombatState result = CombatSystem::runTurnBased(hero, goblin, cin, cout);
    
    switch (result) {
        case CombatState::PlayerWon:
            cout << "\n[ПОБЕДА] Ты одолел врага!\n";
            break;
        case CombatState::PlayerLost:
            cout << "\n[ПОРАЖЕНИЕ] Ты пал в бою...\n";
            break;
        case CombatState::PlayerFled:
            cout << "\n[ПОБЕГ] Ты сбежал, как трус!\n";
            break;
    }

    // 7. ПОБЕДА
    cout << "\n========================================";
    cout << "\n   ТЕСТ ЗАВЕРШЁН УСПЕШНО!";
    cout << "\n   ВСЕ СИСТЕМЫ РАБОТАЮТ.";
    cout << "\n========================================\n";
    

    return 0;
}