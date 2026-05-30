#include "Weapon.hpp"

Weapon::Weapon(int id, const std::string& name, int damageBonus)
    : Item(id, 0, name), m_damageBonus(damageBonus) {}

int Weapon::getDamageBonus() const {
    return m_damageBonus;
}