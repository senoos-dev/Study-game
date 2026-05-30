#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>      // ❗ Добавил - для string
#include <vector>      // ❗ Добавил - для vector
#include "Item.hpp"

using namespace std;   // ❗ Добавил - чтобы не писать std:: везде

class Character {
public:
    Character(string my_location);  // Конструктор теперь только объявлен

    int get_health() const;         // ❗ Добавил const
    void add_health(int x);
    int get_damage() const;         // ❗ Добавил const
    void set_damage(int x);
    void use_poison(int ID);
    void attack_me(int x);
    void add_to_inventory(Item x);
    void remove_from_inventory(int i);
    void print() const;             // ❗ Добавил const (не меняет объект)

private:
    int health = 100;
    int damage = 1;
    string location = "";
    vector<Item> inventory = {};
};

#endif