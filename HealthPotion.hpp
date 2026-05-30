#ifndef HEALTH_POTION_HPP
#define HEALTH_POTION_HPP

#include "Item.hpp"

class HealthPotion : public Item {
public:
    HealthPotion(int id, const std::string& name, int healAmount);
    int getHealAmount() const;
    
private:
    int m_healAmount;
};

#endif