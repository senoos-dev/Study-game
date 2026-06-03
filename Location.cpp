#include "Location.hpp"
#include "Puzzle.hpp"
#include "Chest.hpp"
#include "NPC.hpp"

Location::Location(int id, const std::string& name, const std::vector<std::string>& asciiArt)
    : m_id(id), m_name(name), m_asciiArt(asciiArt), m_isOpen(false) {}

void Location::addConnection(int locationId) {
    m_connections.push_back(locationId);
}

void Location::addChest(std::unique_ptr<Chest> chest) {
    m_chests.push_back(std::move(chest));
}

void Location::addNPC(std::unique_ptr<NPC> npc) {
    m_npcs.push_back(std::move(npc));
}

void Location::setPuzzle(std::unique_ptr<Puzzle> puzzle) {
    m_puzzle = std::move(puzzle);
}

bool Location::isOpen() const {
    return m_isOpen;
}

bool Location::trySolvePuzzle(const std::string& answer, int& damageDealt) {
    if (m_isOpen) return true;
    if (!m_puzzle) return false;
    
    if (m_puzzle->checkAnswer(answer)) {
        m_isOpen = true;
        damageDealt = 0;
        return true;
    } else {
        damageDealt = m_puzzle->getPenaltyDamage();
        return false;
    }
}

void Location::open() {
    m_isOpen = true;
}

int Location::getId() const {
    return m_id;
}

const std::string& Location::getName() const {
    return m_name;
}

const std::vector<std::string>& Location::getAsciiArt() const {
    return m_asciiArt;
}

const std::vector<int>& Location::getConnections() const {
    return m_connections;
}
bool Location::canReach(int targetId) const {
    for (int id : m_connections) {
        if (id == targetId) return true;
    }
    return false;
}

bool Location::hasPuzzle() const {
    return m_puzzle != nullptr;
}

Puzzle* Location::getPuzzle() const {
    return m_puzzle.get();
}

void Location::setOpen(bool open) {
    m_isOpen = open;
}

int Location::getPuzzleFailDamage() const {
    if (m_puzzle) return m_puzzle->getPenaltyDamage();
    return 10;
}
Location::~Location() = default;