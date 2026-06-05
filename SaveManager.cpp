#include "SaveManager.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

using namespace std;

bool SaveManager::save(const string& filename, const SaveData& data) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return false;
    
    file.write(reinterpret_cast<const char*>(&data.health), sizeof(data.health));
    file.write(reinterpret_cast<const char*>(&data.damage), sizeof(data.damage));
    file.write(reinterpret_cast<const char*>(&data.equippedWeaponId), sizeof(data.equippedWeaponId));
    
    size_t len = data.locationName.size();
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(data.locationName.c_str(), len);
    
    file.write(reinterpret_cast<const char*>(&data.currentLocationId), sizeof(data.currentLocationId));
    file.write(reinterpret_cast<const char*>(&data.playerX), sizeof(data.playerX));
    file.write(reinterpret_cast<const char*>(&data.playerY), sizeof(data.playerY));
    
    size_t invSize = data.inventoryIds.size();
    file.write(reinterpret_cast<const char*>(&invSize), sizeof(invSize));
    for (int id : data.inventoryIds) {
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
    }
    
    size_t locSize = data.locationStates.size();
    file.write(reinterpret_cast<const char*>(&locSize), sizeof(locSize));
    // Для vector<bool> нужно особое обращение
    for (size_t i = 0; i < locSize; i++) {
        bool val = data.locationStates[i];
        file.write(reinterpret_cast<const char*>(&val), sizeof(bool));
    }
    
    size_t chestSize = data.openedChests.size();
    file.write(reinterpret_cast<const char*>(&chestSize), sizeof(chestSize));
    for (int id : data.openedChests) {
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
    }
    
    file.write(reinterpret_cast<const char*>(&data.victory), sizeof(data.victory));
    file.write(reinterpret_cast<const char*>(&data.isRunning), sizeof(data.isRunning));
    
    file.close();
    return true;
}

bool SaveManager::load(const string& filename, SaveData& data) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return false;
    
    file.read(reinterpret_cast<char*>(&data.health), sizeof(data.health));
    file.read(reinterpret_cast<char*>(&data.damage), sizeof(data.damage));
    file.read(reinterpret_cast<char*>(&data.equippedWeaponId), sizeof(data.equippedWeaponId));
    
    size_t len;
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    data.locationName.resize(len);
    file.read(&data.locationName[0], len);
    
    file.read(reinterpret_cast<char*>(&data.currentLocationId), sizeof(data.currentLocationId));
    file.read(reinterpret_cast<char*>(&data.playerX), sizeof(data.playerX));
    file.read(reinterpret_cast<char*>(&data.playerY), sizeof(data.playerY));
    
    size_t invSize;
    file.read(reinterpret_cast<char*>(&invSize), sizeof(invSize));
    data.inventoryIds.resize(invSize);
    for (size_t i = 0; i < invSize; i++) {
        file.read(reinterpret_cast<char*>(&data.inventoryIds[i]), sizeof(int));
    }
    
    size_t locSize;
    file.read(reinterpret_cast<char*>(&locSize), sizeof(locSize));
    data.locationStates.resize(locSize);
    for (size_t i = 0; i < locSize; i++) {
        bool val;
        file.read(reinterpret_cast<char*>(&val), sizeof(bool));
        data.locationStates[i] = val;
    }
    
    size_t chestSize;
    file.read(reinterpret_cast<char*>(&chestSize), sizeof(chestSize));
    data.openedChests.resize(chestSize);
    for (size_t i = 0; i < chestSize; i++) {
        file.read(reinterpret_cast<char*>(&data.openedChests[i]), sizeof(int));
    }
    
    file.read(reinterpret_cast<char*>(&data.victory), sizeof(data.victory));
    file.read(reinterpret_cast<char*>(&data.isRunning), sizeof(data.isRunning));
    
    file.close();
    return true;
}

bool SaveManager::saveExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

void SaveManager::listSaves() {
    for (const auto& entry : filesystem::directory_iterator(".")) {
        string path = entry.path().string();
        if (path.find(".save") != string::npos) {
            cout << "  - " << path << endl;
        }
    }
}