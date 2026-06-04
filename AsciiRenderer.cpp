#include "AsciiRenderer.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

void AsciiRenderer::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void AsciiRenderer::render(const vector<string>& map, int playerX, int playerY) {
    for (int y = 0; y < (int)map.size(); y++) {
        for (int x = 0; x < (int)map[y].size(); x++) {
            if (x == playerX && y == playerY) {
                cout << '@';
            } else {
                cout << map[y][x];
            }
        }
        cout << "\n";
    }
}

void AsciiRenderer::drawHUD(int health, int totalDamage) {
    cout << "❤️ HP: " << health << "/100";
    cout << " | ⚔️ Урон: " << totalDamage << "\n";
    cout << "========================================\n";
    cout << "WASD - движение | E - действие | I - инвентарь | R - экипировка | Q - выход\n";
}

void AsciiRenderer::drawInventory(const vector<string>& items) {
    cout << "\n=== ИНВЕНТАРЬ ===\n";
    for (const auto& item : items) {
        cout << "- " << item << "\n";
    }
    cout << "================\n";
}