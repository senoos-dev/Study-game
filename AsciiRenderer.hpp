#ifndef ASCII_RENDERER_HPP
#define ASCII_RENDERER_HPP

#include <string>
#include <vector>

class AsciiRenderer {
public:
    static void clearScreen();
    static void render(const std::vector<std::string>& map, int playerX, int playerY);
    static void drawHUD(int health, int damage);
    static void drawInventory(const std::vector<std::string>& items);
};

#endif