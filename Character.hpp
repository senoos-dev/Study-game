#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <memory>
#include "Item.hpp"

using namespace std;

class Character {
public:
    Character(string my_location);

    int get_health() const;
    void add_health(int x);
    int get_damage() const;
    void set_damage(int x);
    void use_potion(int ID);
    void attack_me(int x);
    void add_to_inventory(unique_ptr<Item> x);
    void remove_from_inventory(int i);
    void print() const;

    void equipWeapon(int weaponId);
    void unequipWeapon();
    int getTotalDamage() const;

private:
    int health = 100;
    int damage = 1;
    string location = "";
    vector<unique_ptr<Item>> inventory;
    int m_equippedWeaponId = -1;
};

#endif