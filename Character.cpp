#include "Character.hpp"
#include "Weapon.hpp"
#include <iostream>

using namespace std;

Character::Character(string my_location) {
    location = my_location;
}

int Character::get_health() const {
    return health;
}

int Character::get_damage() const {
    return damage;
}

void Character::add_health(int x) {
    health = health + x;
    if (health > 100) {
        health = 100;
    }
}

void Character::set_damage(int x) {
    damage = x;
    if (damage < 1) {
        damage = 1;
    }
}

void Character::use_potion(int ID) {
    for (int i = 0; i < (int)inventory.size(); i++) {
        if (inventory[i]->get_ID() == ID) {
            add_health(inventory[i]->get_health_value());
            inventory.erase(inventory.begin() + i);
            cout << "Зелье использовано! Твоё HP: " << health << "\n";
            return;
        }
    }
    cout << "Зелье с ID " << ID << " не найдено!\n";
}

void Character::attack_me(int x) {
    health = health - x;
    if (health < 0) health = 0;
}

void Character::add_to_inventory(unique_ptr<Item> x) {
    inventory.push_back(move(x));
}

void Character::remove_from_inventory(int i) {
    if (i >= 0 && i < (int)inventory.size()) {
        inventory.erase(inventory.begin() + i);
    }
}

void Character::print() const {
    cout << "health: " << health << "\n";
    cout << "damage: " << damage << "\n";
    cout << "location: " << location << "\n";
    cout << "inventory: [ ";
    for (int i = 0; i < (int)inventory.size(); i++) {
        cout << inventory[i]->get_name();
        if (i < (int)inventory.size() - 1) cout << ", ";
    }
    cout << " ]" << "\n";
}

void Character::equipWeapon(int weaponId) {
    for (int i = 0; i < (int)inventory.size(); i++) {
        if (inventory[i]->get_ID() == weaponId) {
            m_equippedWeaponId = weaponId;
            break;
        }
    }
}

void Character::unequipWeapon() {
    m_equippedWeaponId = -1;
}

int Character::getTotalDamage() const {
    int bonus = 0;
    for (int i = 0; i < (int)inventory.size(); i++) {
        if (inventory[i]->get_ID() == m_equippedWeaponId) {
            Weapon* weapon = dynamic_cast<Weapon*>(inventory[i].get());
            if (weapon) {
                bonus = weapon->getDamageBonus();
            }
            break;
        }
    }
    return damage + bonus;
}
void Character::set_location(const std::string& newLoc) {
    location = newLoc;
}