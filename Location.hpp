#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <memory>

class Puzzle;
class Chest;
class NPC;

class Location {
public:
    Location(int id, const std::string& name, const std::vector<std::string>& asciiArt);
    ~Location();
    
    void addConnection(int locationId);
    void addChest(std::unique_ptr<Chest> chest);
    void addNPC(std::unique_ptr<NPC> npc);
    void setPuzzle(std::unique_ptr<Puzzle> puzzle);
    
    bool isOpen() const;
    bool trySolvePuzzle(const std::string& answer, int& damageDealt);
    void open();
    
    // Геттеры
    int getId() const;
    const std::string& getName() const;
    const std::vector<std::string>& getAsciiArt() const;
    const std::vector<int>& getConnections() const;
    
    // Дополнительные методы (без дубликатов!)
    bool canReach(int targetId) const;
    bool hasPuzzle() const;
    Puzzle* getPuzzle() const;
    void setOpen(bool open);
    int getPuzzleFailDamage() const;

private:
    int m_id;
    std::string m_name;
    std::vector<std::string> m_asciiArt;
    std::vector<int> m_connections;
    std::vector<std::unique_ptr<Chest>> m_chests;
    std::vector<std::unique_ptr<NPC>> m_npcs;
    std::unique_ptr<Puzzle> m_puzzle;
    bool m_isOpen;
};

#endif