#include "Chest.hpp"
#include "Puzzle.hpp"

Chest::Chest(int id, bool isLocked) : m_id(id), m_isLocked(isLocked) {}

void Chest::setPuzzle(std::unique_ptr<Puzzle> puzzle) {
    m_puzzle = std::move(puzzle);
}

void Chest::addItem(std::unique_ptr<Item> item) {
    m_items.push_back(std::move(item));
}

bool Chest::open(std::vector<std::unique_ptr<Item>>& inventory, int& damageDealt) {
    if (!m_isLocked) {
        for (auto& item : m_items) {
            inventory.push_back(std::move(item));
        }
        m_items.clear();
        return true;
    }
    
    if (m_puzzle) {
        // Головоломка будет решаться отдельно
        return false;
    }
    
    return false;
}

bool Chest::isLocked() const {
    return m_isLocked;
}

int Chest::getId() const {
    return m_id;
}