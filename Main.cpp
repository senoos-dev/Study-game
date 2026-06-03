#include <iostream>
#include <memory>
#include "Character.hpp"
#include "Weapon.hpp"
#include "HealthPotion.hpp"

using namespace std;

int main() {
    Character hero("Тёмная комната");
    
    cout << "=== СОЗДАЁМ ПРЕДМЕТЫ ===" << endl;
    
    hero.add_to_inventory(make_unique<HealthPotion>(1, "Volt - Виноград", 25));
    hero.add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
    hero.add_to_inventory(make_unique<Weapon>(3, "Дрын", 9999));
    hero.add_to_inventory(make_unique<Weapon>(4, "Меч", 10));
    
    hero.print();
    cout << "Текущий урон: " << hero.get_damage() << endl;
    cout << "Общий урон с оружием: " << hero.getTotalDamage() << endl;
    
    cout << "\n=== ЭКИПИРУЕМ МЕЧ (ID=4) ===" << endl;
    hero.equipWeapon(4);
    hero.print();
    cout << "Общий урон с мечом: " << hero.getTotalDamage() << endl;
    
    cout << "\n=== ЭКИПИРУЕМ ДРЫН (ID=3) ===" << endl;
    hero.equipWeapon(3);
    hero.print();
    cout << "Общий урон с дрыном: " << hero.getTotalDamage() << endl;
    
    cout << "\n=== СНИМАЕМ ОРУЖИЕ ===" << endl;
    hero.unequipWeapon();
    hero.print();
    cout << "Общий урон без оружия: " << hero.getTotalDamage() << endl;
    
    cout << "\n=== ИСПОЛЬЗУЕМ ЗЕЛЬЕ VOLT (ID=1) ===" << endl;
    hero.use_potion(1);
    hero.print();
    
    cout << "\n=== АТАКА НА 30 ===" << endl;
    hero.attack_me(30);
    hero.print();
    
    cout << "\n=== ЛЕЧИМСЯ НА 50 ===" << endl;
    hero.add_health(50);
    hero.print();
    
    return 0;
}