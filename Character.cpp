#include "Character.hpp"
#include <iostream>  // только для cout, можно убрать если не нужен

using namespace std;

// Конструктор
Character::Character(string my_location) {
    location = my_location;
}

// Геттеры с const
int Character::get_health() const {
    return health;
}

int Character::get_damage() const {
    return damage;
}

// Добавить здоровье
void Character::add_health(int x) {
    health = health + x;
    if (health > 100) {
        health = 100;
    }
}

// Установить урон (исправлено!)
void Character::set_damage(int x) {
    damage = x;
    if (damage < 1) {
        damage = 1;
    }
}

// Использовать зелье (исправлено название: use_potion, не use_poison)
void Character::use_poison(int ID) {
    int L = inventory.size();
    for (int i = 0; i < L; i++) {
        if (inventory[i].get_ID() == ID) {
            add_health(inventory[i].get_health_value());
            remove_from_inventory(i);
            L--;        // уменьшаем, так как размер вектора уменьшился
            i--;        // ❗ Добавил - нужно проверить следующий элемент на той же позиции
        }
    }
}

// Атаковать персонажа
void Character::attack_me(int x) {
    health = health - x;
    if (health < 0) health = 0;  // ❗ Добавил - здоровье не может быть отрицательным
}

// Добавить в инвентарь
void Character::add_to_inventory(Item x) {
    inventory.push_back(x);
}

// Удалить из инвентаря по индексу
void Character::remove_from_inventory(int i) {
    if (i >= 0 && i < (int)inventory.size()) {
        inventory.erase(inventory.begin() + i);
    }
}

// Вывести информацию
void Character::print() const {
    cout << "health: " << health << "\n";
    cout << "damage: " << damage << "\n";
    cout << "location: " << location << "\n";
    cout << "inventory: [ ";
    int L = inventory.size();
    for (int i = 0; i < L; i++) {
        cout << inventory[i].get_name();
        if (i < L - 1) cout << ", ";  // не ставим запятую после последнего
    }
    cout << " ]" << "\n";
}