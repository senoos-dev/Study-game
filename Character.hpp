#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <memory>
#include "Item.hpp"
#include "Weapon.hpp"

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
    int getEquippedWeaponId() const { return m_equippedWeaponId; }

    bool is_alive() const;
    string get_location() const;
    void set_location(const string& newLoc);
    
    bool hasItem(int ID) const {
        for (const auto& item : inventory) {
            if (item->get_ID() == ID) return true;
        }
        return false;
    }
    
    
    int getItemCount(int ID) const {
        int count = 0;
        for (const auto& item : inventory) {
            if (item->get_ID() == ID) count++;
        }
        return count;
    }

    void remove_from_inventory_by_id(int ID) {
        for (int i = 0; i < (int)inventory.size(); i++) {
            if (inventory[i]->get_ID() == ID) {
                inventory.erase(inventory.begin() + i);
                return;
            }
        }
    }
    

private:
    int health = 100;
    int damage = 1;
    string location = "";
    vector<unique_ptr<Item>> inventory;
    int m_equippedWeaponId = -1;
};

#endif