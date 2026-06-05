#ifndef SAVE_MANAGER_HPP
#define SAVE_MANAGER_HPP

#include <string>
#include <vector>

struct SaveData {
    int health;
    int damage;
    int equippedWeaponId;
    std::string locationName;
    int currentLocationId;
    int playerX;
    int playerY;
    std::vector<int> inventoryIds;
    std::vector<bool> locationStates;  // открыта ли каждая локация
    std::vector<int> openedChests;      // ID открытых сундуков
    bool victory;
    bool isRunning;
};

class SaveManager {
public:
    static bool save(const std::string& filename, const SaveData& data);
    static bool load(const std::string& filename, SaveData& data);
    static bool saveExists(const std::string& filename);
    static void listSaves();
};

#endif