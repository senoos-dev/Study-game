#include "HealthPotion.hpp"

HealthPotion::HealthPotion(int id, const std::string& name, int healAmount)
    : Item(id, healAmount, name), m_healAmount(healAmount) {}

int HealthPotion::getHealAmount() const {
    return m_healAmount;
}