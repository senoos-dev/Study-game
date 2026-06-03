#ifndef CHEST_HPP
#define CHEST_HPP

#include <vector>
#include <memory>
#include "Item.hpp"

class Puzzle;

class Chest {
public:
    Chest(int id, bool isLocked = false);
    void setPuzzle(std::unique_ptr<Puzzle> puzzle);
    void addItem(std::unique_ptr<Item> item);
    bool open(std::vector<std::unique_ptr<Item>>& inventory, int& damageDealt);
    bool isLocked() const;
    int getId() const;

private:
    int m_id;
    bool m_isLocked;
    std::unique_ptr<Puzzle> m_puzzle;
    std::vector<std::unique_ptr<Item>> m_items;
};

#endif