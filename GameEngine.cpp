#include "GameEngine.hpp"
#include "AsciiRenderer.hpp"
#include "InputHandler.hpp"
#include "UIManager.hpp"
#include "CombatSystem.hpp"
#include "Location.hpp"
#include "NPC.hpp"
#include "Weapon.hpp"
#include "HealthPotion.hpp"
#include "RiddlePuzzle.hpp"
#include "MathPuzzle.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, last - first + 1);
}

GameEngine::GameEngine() 
    : m_currentLocationId(0), m_playerX(1), m_playerY(1), m_isRunning(true), m_victory(false) {}

GameEngine::~GameEngine() = default;

void GameEngine::resetGame() {
    m_player = make_unique<Character>("Стартовая локация");
    (*m_player).add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
    (*m_player).add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
    (*m_player).add_to_inventory(make_unique<Weapon>(3, "Меч", 8));
    (*m_player).add_to_inventory(make_unique<Weapon>(4, "Топор", 15));
    
    m_currentLocationId = 0;
    m_playerX = 1;
    m_playerY = 1;
    m_isRunning = true;
    m_victory = false;
    
    setupLocations();
    m_map = m_locations[0]->getAsciiArt();
}

bool GameEngine::init() {
    UIManager::showWelcomeScreen();
    resetGame();
    return true;
}

void GameEngine::setupLocations() {
    m_locations.clear();
    
    // Локация 1: Стартовая (открыта)
    vector<string> map1 = {
        "##########",
        "#........#",
        "#..C.....#",
        "#........#",
        "#.......D#",
        "#........#",
        "#........#",
        "##########"
    };
    auto loc1 = make_unique<Location>(1, "Стартовая комната", map1);
    loc1->addConnection(2);
    loc1->open();
    m_locations.push_back(move(loc1));
    
    // Локация 2: Тёмный лес (Мудрец)
    vector<string> map2 = {
        "##########",
        "#........#",
        "#..C.....#",
        "#........#",
        "#.......D#",
        "#....N...#",
        "#........#",
        "##########"
    };
    auto loc2 = make_unique<Location>(2, "Тёмный лес", map2);
    loc2->addConnection(1);
    loc2->addConnection(3);
    loc2->setPuzzle(make_unique<RiddlePuzzle>("Зимой и летом одним цветом? (одно слово)", "елка", 15));
    m_locations.push_back(move(loc2));
    
    // Локация 3: Подземелье (Торговец)
    vector<string> map3 = {
        "##########",
        "#...C....#",
        "#........#",
        "#........#",
        "#.......D#",
        "#....N...#",
        "#........#",
        "##########"
    };
    auto loc3 = make_unique<Location>(3, "Подземелье", map3);
    loc3->addConnection(2);
    loc3->addConnection(4);
    loc3->setPuzzle(make_unique<MathPuzzle>(2, 2, '*', 20));
    m_locations.push_back(move(loc3));
    
    // Локация 4: Пещера стража (Стражник)
    vector<string> map4 = {
        "##########",
        "#........#",
        "#........#",
        "#........#",
        "#.......D#",
        "#....N...#",
        "#........#",
        "##########"
    };
    auto loc4 = make_unique<Location>(4, "Пещера стража", map4);
    loc4->addConnection(3);
    loc4->addConnection(5);
    loc4->setPuzzle(make_unique<RiddlePuzzle>("Что можно сломать, не прикасаясь к нему? (одно слово)", "обещание", 25));
    m_locations.push_back(move(loc4));
    
    // Локация 5: Финальная сокровищница
    vector<string> map5 = {
        "##########",
        "#........#",
        "#........#",
        "#........#",
        "#.......D#",
        "#........#",
        "#........#",
        "##########"
    };
    auto loc5 = make_unique<Location>(5, "Сокровищница", map5);
    loc5->addConnection(4);
    loc5->setPuzzle(make_unique<RiddlePuzzle>("Что не имеет начала и конца? (одно слово)", "кольцо", 30));
    m_locations.push_back(move(loc5));
    
    m_map = m_locations[0]->getAsciiArt();
}

void GameEngine::run() {
    while (m_isRunning) {
        render();
        processInput();
        update();
        
        if (m_victory) {
            UIManager::showGameOverScreen(true);
            cout << "\nХотите сыграть ещё раз? (y/n): ";
            char again;
            cin >> again;
            if (again == 'y' || again == 'Y') {
                resetGame();
            } else {
                m_isRunning = false;
            }
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

void GameEngine::update() {}

void GameEngine::movePlayer(int newX, int newY) {
    if (newX < 0 || newY < 0 || newY >= (int)m_map.size() || newX >= (int)m_map[0].size()) {
        return;
    }
    
    char target = m_map[newY][newX];
    
    if (target == '#') {
        cout << "\n[!] Там стена, не пройти!\n";
        cout << "Нажми Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
        return;
    }
    
    m_playerX = newX;
    m_playerY = newY;
    
    if (target == 'D') {
        Location* currentLoc = m_locations[m_currentLocationId].get();
        
        if (!currentLoc->isOpen()) {
            bool hasKey = (*m_player).getItemCount(10) > 0;
            
            if (hasKey) {
                cout << "\n[!] У тебя есть ключ! Использовать его, чтобы открыть дверь? (y/n): ";
                char useKey;
                cin >> useKey;
                if (useKey == 'y' || useKey == 'Y') {
                    (*m_player).remove_from_inventory_by_id(10);
                    currentLoc->open();
                    cout << "\n[!] Ты использовал ключ! Локация открыта!\n";
                    cout << "Нажми Enter чтобы продолжить...";
                    cin.ignore();
                    cin.get();
                } else {
                    int damage = 0;
                    string answer;
                    cout << "\n[!] Локация закрыта! Решите головоломку:\n";
                    if (currentLoc->getPuzzle()) {
                        cout << currentLoc->getPuzzle()->getPrompt() << "\n";
                    }
                    cout << "Ответ: ";
                    cin.ignore();
                    getline(cin, answer);
                    
                    string trimmedAnswer = trim(answer);
                    
                    if (currentLoc->trySolvePuzzle(trimmedAnswer, damage)) {
                        cout << "\n[!] Правильно! Локация открыта!\n";
                        currentLoc->open();
                        cout << "Нажми Enter чтобы продолжить...";
                        cin.get();
                    } else {
                        cout << "\n[!] Неправильно! Вы получаете " << damage << " урона!\n";
                        (*m_player).attack_me(damage);
                        if ((*m_player).get_health() <= 0) {
                            cout << "\n[!] Ты погиб... Игра окончена.\n";
                            cout << "Хотите начать заново? (y/n): ";
                            char again;
                            cin >> again;
                            if (again == 'y' || again == 'Y') {
                                resetGame();
                            } else {
                                m_isRunning = false;
                            }
                            return;
                        }
                        cout << "Нажми Enter чтобы продолжить...";
                        cin.get();
                        return;
                    }
                }
            } else {
                int damage = 0;
                string answer;
                cout << "\n[!] Локация закрыта! Решите головоломку:\n";
                if (currentLoc->getPuzzle()) {
                    cout << currentLoc->getPuzzle()->getPrompt() << "\n";
                }
                cout << "Ответ: ";
                cin.ignore();
                getline(cin, answer);
                
                string trimmedAnswer = trim(answer);
                
                if (currentLoc->trySolvePuzzle(trimmedAnswer, damage)) {
                    cout << "\n[!] Правильно! Локация открыта!\n";
                    currentLoc->open();
                    cout << "Нажми Enter чтобы продолжить...";
                    cin.get();
                } else {
                    cout << "\n[!] Неправильно! Вы получаете " << damage << " урона!\n";
                    (*m_player).attack_me(damage);
                    if ((*m_player).get_health() <= 0) {
                        cout << "\n[!] Ты погиб... Игра окончена.\n";
                        cout << "Хотите начать заново? (y/n): ";
                        char again;
                        cin >> again;
                        if (again == 'y' || again == 'Y') {
                            resetGame();
                        } else {
                            m_isRunning = false;
                        }
                        return;
                    }
                    cout << "Нажми Enter чтобы продолжить...";
                    cin.get();
                    return;
                }
            }
        }
        
        if (currentLoc->isOpen()) {
            if (m_currentLocationId + 1 < (int)m_locations.size()) {
                m_currentLocationId++;
                m_map = m_locations[m_currentLocationId]->getAsciiArt();
                m_playerX = 1;
                m_playerY = 1;
                cout << "\n[!] Ты перешёл в новую локацию!\n";
                if (m_currentLocationId == (int)m_locations.size() - 1) {
                    cout << "[!] Это последняя локация! Найди выход!\n";
                }
                cout << "Нажми Enter чтобы продолжить...";
                cin.get();
            } else {
                m_victory = true;
            }
        }
    }
}

void GameEngine::interact() {
    char target = m_map[m_playerY][m_playerX];
    
    if (target == 'C') {
        m_map[m_playerY][m_playerX] = '.';
        
        static int chestNumber = 0;
        chestNumber++;
        
        if (chestNumber % 2 == 1) {
            (*m_player).add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
            cout << "\n[!] Ты открыл сундук и нашёл Малое зелье!\n";
        } else {
            (*m_player).add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
            cout << "\n[!] Ты открыл сундук и нашёл Большое зелье!\n";
        }
        cout << "Нажми Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
    } 
    else if (target == 'N') {
        // Определяем тип NPC по локации
        if (m_currentLocationId == 2) {
            // Мудрец в локации 2 (Тёмный лес)
            cout << "\n[Мудрец]: Приветствую, путник! Я хранитель знаний.\n";
            cout << "[Мудрец]: Ответь на мою загадку, и я дам тебе ключ.\n";
            cout << "Загадка: Что можно держать, но нельзя потрогать?\n";
            cout << "Ответ: ";
            
            string answer;
            cin.ignore();
            getline(cin, answer);
            
            string trimmedAnswer = trim(answer);
            
            if (trimmedAnswer == "слово" || trimmedAnswer == "Слово" || 
                trimmedAnswer == "обещание" || trimmedAnswer == "Обещание") {
                cout << "[Мудрец]: Верно! Держи ключ.\n";
                (*m_player).add_to_inventory(make_unique<Item>(10, 0, "Ключ"));
                m_map[m_playerY][m_playerX] = '.';
                cout << "\n[!] Ты получил ключ!\n";
            } else {
                cout << "[Мудрец]: Неправильно! Получи урон за невнимательность!\n";
                (*m_player).attack_me(15);
                cout << "[!] Ты получил 15 урона! Твоё HP: " << (*m_player).get_health() << "\n";
                if ((*m_player).get_health() <= 0) {
                    cout << "\n[!] Ты погиб...\n";
                    cout << "Хотите начать заново? (y/n): ";
                    char again;
                    cin >> again;
                    if (again == 'y' || again == 'Y') {
                        resetGame();
                    } else {
                        m_isRunning = false;
                    }
                    return;
                }
            }
        } 
        else if (m_currentLocationId == 3) {
            // Торговец в локации 3 (Подземелье)
            cout << "\n[Торговец]: Путник, хочешь купить зелье?\n";
            cout << "Малое зелье - 30 HP (бесплатно для тебя!)\n";
            cout << "1 - Взять зелье\n";
            cout << "2 - Уйти\n";
            cout << "Выбор: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                (*m_player).add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
                cout << "\n[!] Ты получил Малое зелье!\n";
                m_map[m_playerY][m_playerX] = '.';
            }
        } 
        else {
            // Стражник в локации 4 (Пещера стража) и других
            cout << "\n[Стражник]: Стой! Кто идёт?\n";
            cout << "1 - Я ищу выход, помоги мне\n";
            cout << "2 - Отойди или будешь драться!\n";
            cout << "Выбор: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                cout << "\n[Стражник]: Хорошо. Я дам тебе ключ, если докажешь свою силу.\n";
                cout << "[Стражник]: Сразишься со мной?\n";
                cout << "1 - Да, сражусь\n";
                cout << "2 - Нет, уйду\n";
                cout << "Выбор: ";
                
                int fightChoice;
                cin >> fightChoice;
                
                if (fightChoice == 1) {
                    NPC guard(100, "Стражник", 40, 8, NpcState::Hostile);
                    CombatState result = CombatSystem::runTurnBased((*m_player), guard);
                    
                    if (result == CombatState::PlayerWon) {
                        cout << "\n[!] Ты победил стражника!\n";
                        cout << "[!] Он выронил ключ!\n";
                        (*m_player).add_to_inventory(make_unique<Item>(10, 0, "Ключ"));
                        m_map[m_playerY][m_playerX] = '.';
                    } else if (result == CombatState::PlayerLost) {
                        cout << "\n[!] Ты погиб в бою...\n";
                        cout << "Хотите начать заново? (y/n): ";
                        char again;
                        cin >> again;
                        if (again == 'y' || again == 'Y') {
                            resetGame();
                        } else {
                            m_isRunning = false;
                        }
                        return;
                    }
                }
            } 
            else if (choice == 2) {
                cout << "\n[Стражник]: Тогда умри!\n";
                NPC guard(100, "Стражник", 40, 8, NpcState::Hostile);
                CombatState result = CombatSystem::runTurnBased((*m_player), guard);
                
                if (result == CombatState::PlayerWon) {
                    cout << "\n[!] Ты победил стражника!\n";
                    cout << "[!] Он выронил зелье!\n";
                    (*m_player).add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
                    m_map[m_playerY][m_playerX] = '.';
                } else if (result == CombatState::PlayerLost) {
                    cout << "\n[!] Ты погиб в бою...\n";
                    cout << "Хотите начать заново? (y/n): ";
                    char again;
                    cin >> again;
                    if (again == 'y' || again == 'Y') {
                        resetGame();
                    } else {
                        m_isRunning = false;
                    }
                    return;
                }
            }
        }
        cout << "Нажми Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
    } 
    else {
        cout << "\n[!] Здесь не с чем взаимодействовать\n";
        cout << "Нажми Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
    }
}

void GameEngine::showInventory() {
    while (true) {
        cout << "\n========== ИНВЕНТАРЬ ==========\n";
        
        int smallCount = (*m_player).getItemCount(1);
        int bigCount = (*m_player).getItemCount(2);
        int keyCount = (*m_player).getItemCount(10);
        bool hasSword = (*m_player).hasItem(3);
        bool hasAxe = (*m_player).hasItem(4);
        
        if (smallCount > 0) {
            cout << "1 - Малое зелье (ID: 1, +30 HP) x" << smallCount << "\n";
        }
        if (bigCount > 0) {
            cout << "2 - Большое зелье (ID: 2, +60 HP) x" << bigCount << "\n";
        }
        if (hasSword) {
            cout << "3 - Меч (ID: 3, +8 урона)\n";
        }
        if (hasAxe) {
            cout << "4 - Топор (ID: 4, +15 урона)\n";
        }
        if (keyCount > 0) {
            cout << "5 - Ключ (ID: 10) x" << keyCount << "\n";
        }
        
        if (smallCount == 0 && bigCount == 0 && !hasSword && !hasAxe && keyCount == 0) {
            cout << "Инвентарь пуст!\n";
            cout << "Нажми Enter чтобы продолжить...";
            cin.ignore();
            cin.get();
            return;
        }
        
        cout << "0 - Выход\n";
        cout << "===============================\n";
        cout << "Выбор: ";
        
        int id;
        cin >> id;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Неверный ввод!\n";
            continue;
        }
        
        if (id == 0) return;
        
        if (id == 1 && smallCount > 0) {
            (*m_player).use_potion(1);
            cout << "\n[!] Малое зелье использовано! Твоё HP: " << (*m_player).get_health() << "\n";
            cout << "Нажми Enter чтобы продолжить...";
            cin.ignore();
            cin.get();
        } else if (id == 2 && bigCount > 0) {
            (*m_player).use_potion(2);
            cout << "\n[!] Большое зелье использовано! HP: " << (*m_player).get_health() << "\n";
            cout << "Нажми Enter чтобы продолжить...";
            cin.ignore();
            cin.get();
        } else if (id == 3 && hasSword) {
            (*m_player).equipWeapon(3);
            cout << "\n[!] Меч экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
            cout << "Нажми Enter чтобы продолжить...";
            cin.ignore();
            cin.get();
        } else if (id == 4 && hasAxe) {
            (*m_player).equipWeapon(4);
            cout << "\n[!] Топор экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
            cout << "Нажми Enter чтобы продолжить...";
            cin.ignore();
            cin.get();
        } else if (id == 5 && keyCount > 0) {
            cout << "\n[!] Это ключ. Он открывает закрытые локации.\n";
            cout << "Нажми Enter чтобы продолжить...";
            cin.ignore();
            cin.get();
        } else {
            cout << "Неверный выбор!\n";
        }
    }
}

void GameEngine::showEquipMenu() {
    cout << "\n=== ЭКИПИРОВКА ОРУЖИЯ ===\n";
    
    if (!(*m_player).hasItem(3) && !(*m_player).hasItem(4)) {
        cout << "У тебя нет оружия для экипировки!\n";
        cout << "Нажми Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
        return;
    }
    
    if ((*m_player).hasItem(3)) cout << "1 - Меч (ID: 3, +8 урона)\n";
    if ((*m_player).hasItem(4)) cout << "2 - Топор (ID: 4, +15 урона)\n";
    cout << "0 - Отмена\n";
    cout << "Выбор: ";
    
    int choice;
    cin >> choice;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный ввод!\n";
        return;
    }
    
    if (choice == 1 && (*m_player).hasItem(3)) {
        (*m_player).equipWeapon(3);
        cout << "\n[!] Меч экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
        cout << "Нажми Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
    } else if (choice == 2 && (*m_player).hasItem(4)) {
        (*m_player).equipWeapon(4);
        cout << "\n[!] Топор экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
        cout << "Нажми Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
    }
}