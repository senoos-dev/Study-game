#include <iostream>
#include "Character.hpp"
#include "Item.hpp"

using namespace std;

int main() {
    // Создаём персонажа
    Character hero("Тёмная комната");
    
    // Создаём зелья
    Item smallPotion(1, 30, "Малое зелье");
    Item bigPotion(2, 60, "Большое зелье");
    
    // Добавляем в инвентарь
    hero.add_to_inventory(smallPotion);
    hero.add_to_inventory(bigPotion);
    
    // Показываем состояние
    cout << "=== НАЧАЛО ===" << endl;
    hero.print();
    
    // Используем зелье
    cout << "\n=== ИСПОЛЬЗУЕМ МАЛОЕ ЗЕЛЬЕ ===" << endl;
    hero.use_poison(1);
    hero.print();
    
    // Атакуем
    cout << "\n=== АТАКА НА 25 УРОНА ===" << endl;
    hero.attack_me(25);
    hero.print();
    
    // Лечимся
    cout << "\n=== ЛЕЧИМСЯ НА 50 ===" << endl;
    hero.add_health(50);
    hero.print();
    
    return 0;
}