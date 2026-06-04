#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <vector>
#include <memory>
#include <string>
#include "Character.hpp"
#include "Location.hpp"

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    
    bool init();
    void run();
    
private:
    void processInput();
    void update();
    void render();
    
    void movePlayer(int newX, int newY);
    void interact();
    void showInventory();
    void showEquipMenu();
    
    void setupLocations();
    
    std::unique_ptr<Character> m_player;
    std::vector<std::unique_ptr<Location>> m_locations;
    int m_currentLocationId;
    int m_playerX;
    int m_playerY;
    bool m_isRunning;
    bool m_victory;
    
    std::vector<std::string> m_map;
};

#endif