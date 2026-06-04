#include "GameEngine.hpp"
#include "AsciiRenderer.hpp"
#include "InputHandler.hpp"
#include "UIManager.hpp"
#include "CombatSystem.hpp"
#include "Location.hpp"
#include "NPC.hpp"
#include "Weapon.hpp"
#include "HealthPotion.hpp"
#include <iostream>

using namespace std;

GameEngine::GameEngine() 
    : m_currentLocationId(0), m_playerX(1), m_playerY(1), m_isRunning(true), m_victory(false) {}

GameEngine::~GameEngine() = default;

bool GameEngine::init() {
    UIManager::showWelcomeScreen();
    
    m_player = make_unique<Character>("Стартовая локация");
    
    (*m_player).add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
    (*m_player).add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
    (*m_player).add_to_inventory(make_unique<Weapon>(3, "Меч", 8));
    (*m_player).add_to_inventory(make_unique<Weapon>(4, "Топор", 15));
    
    setupLocations();
    
    return true;
}

void GameEngine::setupLocations() {
    vector<string> map1 = {
        "##########",
        "#........#",
        "#..C.....#",
        "#........#",
        "#.......D#",
        "#....N...#",
        "#........#",
        "##########"
    };
    
    auto loc1 = make_unique<Location>(1, "Стартовая комната", map1);
    loc1->addConnection(2);
    m_locations.push_back(move(loc1));
    
    vector<string> map2 = {
        "##########",
        "#........#",
        "#........#",
        "#........#",
        "#........#",
        "#........#",
        "#........#",
        "##########"
    };
    
    auto loc2 = make_unique<Location>(2, "Тёмный зал", map2);
    m_locations.push_back(move(loc2));
    
    m_map = m_locations[0]->getAsciiArt();
}

void GameEngine::run() {
    while (m_isRunning) {
        render();
        processInput();
        update();
        
        if (m_victory) {
            UIManager::showGameOverScreen(true);
            break;
        }
    }
}

void GameEngine::render() {
    AsciiRenderer::clearScreen();
    AsciiRenderer::render(m_map, m_playerX, m_playerY);
    AsciiRenderer::drawHUD(
        (*m_player).get_health(), 
        (*m_player).getTotalDamage()
    );
}

void GameEngine::processInput() {
    Action action = InputHandler::getAction();
    
    switch (action) {
        case Action::MoveUp:
            movePlayer(m_playerX, m_playerY - 1);
            break;
        case Action::MoveDown:
            movePlayer(m_playerX, m_playerY + 1);
            break;
        case Action::MoveLeft:
            movePlayer(m_playerX - 1, m_playerY);
            break;
        case Action::MoveRight:
            movePlayer(m_playerX + 1, m_playerY);
            break;
        case Action::Interact:
            interact();
            break;
        case Action::OpenInventory:
            showInventory();
            break;
        case Action::EquipWeapon:
            showEquipMenu();
            break;
        case Action::Quit:
            m_isRunning = false;
            break;
        default:
            break;
    }
}

void GameEngine::update() {
    // Для будущего обновления логики
}

void GameEngine::movePlayer(int newX, int newY) {
    if (newX < 0 || newY < 0 || newY >= (int)m_map.size() || newX >= (int)m_map[0].size()) {
        return;
    }
    
    char target = m_map[newY][newX];
    
    if (target == '#') {
        UIManager::showMessage("Там стена, не пройти!");
        UIManager::waitForEnter();
        return;
    }
    
    m_playerX = newX;
    m_playerY = newY;
    
    if (target == 'D') {
        if (m_currentLocationId + 1 < (int)m_locations.size()) {
            m_currentLocationId++;
            m_map = m_locations[m_currentLocationId]->getAsciiArt();
            m_playerX = 1;
            m_playerY = 1;
            UIManager::showMessage("Ты перешёл в новую локацию!");
            UIManager::waitForEnter();
        } else {
            m_victory = true;
        }
    }
}

void GameEngine::interact() {
    char target = m_map[m_playerY][m_playerX];
    
    if (target == 'C') {
        m_map[m_playerY][m_playerX] = '.';
        
        UIManager::showMessage("Ты открыл сундук и нашёл зелье!");
        (*m_player).add_to_inventory(make_unique<HealthPotion>(5, "Зелье из сундука", 25));
        (*m_player).add_health(25);
        UIManager::waitForEnter();
    } else if (target == 'N') {
        UIManager::showMessage("Ты встретил странника. Он дал тебе совет.");
        UIManager::waitForEnter();
    } else {
        UIManager::showMessage("Здесь не с чем взаимодействовать");
        UIManager::waitForEnter();
    }
}

void GameEngine::showInventory() {
    cout << "\n========== ИНВЕНТАРЬ ==========\n";
    
    // Проверяем, какие зелья есть
    bool hasSmallPotion = false;
    bool hasBigPotion = false;
    
    // Здесь нужно проверить реальный инвентарь
    // Простой способ: попробовать использовать и увидеть ошибку "не найдено"
    
    cout << "1 - Малое зелье (ID: 1, +30 HP) - " << (hasSmallPotion ? "есть" : "нет") << "\n";
    cout << "2 - Большое зелье (ID: 2, +60 HP) - " << (hasBigPotion ? "есть" : "нет") << "\n";
    cout << "3 - Меч (ID: 3, +8 урона)\n";
    cout << "4 - Топор (ID: 4, +15 урона)\n";
    cout << "0 - Назад\n";
    cout << "===============================\n";
    cout << "Выбор: ";
    
    int id;
    cin >> id;
    
    if (id == 1) {
        (*m_player).use_potion(1);
        // После использования покажем инвентарь заново
        showInventory();
    } else if (id == 2) {
        (*m_player).use_potion(2);
        showInventory();
    } else if (id == 3) {
        (*m_player).equipWeapon(3);
        cout << "[!] Меч экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
    } else if (id == 4) {
        (*m_player).equipWeapon(4);
        cout << "[!] Топор экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
    }
}

void GameEngine::showEquipMenu() {
    cout << "\n=== ЭКИПИРОВКА ОРУЖИЯ ===\n";
    
    // Проверяем, какое оружие есть в инвентаре
    bool hasSword = (*m_player).hasItem(3);
    bool hasAxe = (*m_player).hasItem(4);
    
    if (hasSword) {
        cout << "1 - Меч (ID: 3, +8 урона)\n";
    }
    if (hasAxe) {
        cout << "2 - Топор (ID: 4, +15 урона)\n";
    }
    if (!hasSword && !hasAxe) {
        cout << "У тебя нет оружия для экипировки!\n";
        UIManager::waitForEnter();
        return;
    }
    
    cout << "0 - Отмена\n";
    cout << "Выбор: ";
    
    int choice;
    cin >> choice;
    
    if (choice == 1 && hasSword) {
        (*m_player).equipWeapon(3);
        cout << "[!] Меч экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
        UIManager::waitForEnter();
    } else if (choice == 2 && hasAxe) {
        (*m_player).equipWeapon(4);
        cout << "[!] Топор экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
        UIManager::waitForEnter();
    }
}