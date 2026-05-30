#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item.hpp"

class Weapon : public Item {
public:
    Weapon(int id, const std::string& name, int damageBonus);
    int getDamageBonus() const;
    
private:
    int m_damageBonus;
};

#endif