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
#include "CodePuzzle.hpp"
#include "SaveManager.hpp"
#include "Logger.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>
#include <chrono>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, last - first + 1);
}

static random_device rd;
static mt19937 gen(rd());

GameEngine::GameEngine() 
    : m_currentLocationId(0), m_playerX(1), m_playerY(1), m_isRunning(true), m_victory(false) {}

GameEngine::~GameEngine() = default;

void GameEngine::resetGame() {
    m_player = make_unique<Character>("Стартовая локация");
    (*m_player).add_to_inventory(make_unique<Weapon>(3, "Меч", 8));
    
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
    
    vector<string> map1 = {
        "##########",
        "#.......D#",
        "#..C.....#",
        "#........#",
        "#.......D#",
        "#........#",
        "#........#",
        "##########"
    };
    auto loc1 = make_unique<Location>(1, "Стартовая комната", map1);
    loc1->addConnection(2);
    loc1->addConnection(3);
    loc1->open();
    m_locations.push_back(move(loc1));
    
    vector<string> map2 = {
        "##########",
        "#........#",
        "#..C.....#",
        "#........#",
        "#.......D#",
        "#....N...#",
        "#D.......#",
        "##########"
    };
    auto loc2 = make_unique<Location>(2, "Тёмный лес", map2);
    loc2->addConnection(1);
    loc2->addConnection(3);
    
    vector<pair<string, string>> riddles = {
        {"Зимой и летом одним цветом? (одно слово)", "elka"},
        {"Что можно сломать, не прикасаясь к нему? (одно слово)", "obeshaniye"},
        {"Что не имеет начала и конца? (одно слово)", "kolco"},
        {"Висит груша, нельзя скушать", "lampochka"},
        {"На березе росло 5 яблок. 2 упало, сколько осталось?", "0"}
    };
    uniform_int_distribution<> dis(0, riddles.size() - 1);
    int idx = dis(gen);
    loc2->setPuzzle(make_unique<RiddlePuzzle>(riddles[idx].first, riddles[idx].second, 15));
    m_locations.push_back(move(loc2));
    
    vector<string> map3 = {
        "##########",
        "#...C....#",
        "#D.......#",
        "#........#",
        "#.......D#",
        "#....N...#",
        "#.D......#",
        "##########"
    };
    auto loc3 = make_unique<Location>(3, "Подземелье", map3);
    loc3->addConnection(1);
    loc3->addConnection(2);
    loc3->addConnection(4);
    loc3->setPuzzle(make_unique<MathPuzzle>(2, 2, '*', 20));
    m_locations.push_back(move(loc3));
    
    vector<string> map4 = {
        "##########",
        "#........#",
        "#D.......#",
        "#........#",
        "#.......D#",
        "#....N...#",
        "#........#",
        "##########"
    };
    auto loc4 = make_unique<Location>(4, "Пещера стража", map4);
    loc4->addConnection(3);
    loc4->addConnection(5);
    
    vector<pair<string, string>> codePuzzles = {
        {"Код из 4 букв — время года с лютым холодом", "zima"},
        {"Код из 3 букв — краткое название учебного заведения", "pgu"},
        {"Код из 5 букв — имена 2 разработчиков (одинаковые)", "slava"}
    };
    uniform_int_distribution<> disCode(0, codePuzzles.size() - 1);
    int idxCode = disCode(gen);
    loc4->setPuzzle(make_unique<CodePuzzle>(codePuzzles[idxCode].first, codePuzzles[idxCode].second, 25));
    m_locations.push_back(move(loc4));
    
    vector<string> map5 = {
        "##########",
        "#........#",
        "#........#",
        "#........#",
        "#.......D#",
        "#..C.....#",
        "#........#",
        "##########"
    };
    auto loc5 = make_unique<Location>(5, "Сокровищница", map5);
    loc5->addConnection(4);
    
    vector<pair<string, string>> finalRiddles = {
        {"Продолжите: У лукоморья дуб зеленый, златая цепь на дубе ...", "tom"},
        {"Самый умный персонаж из Винни Пуха", "sova"},
        {"Что можно увидеть с закрытыми глазами?", "son"}
    };
    uniform_int_distribution<> disFinal(0, finalRiddles.size() - 1);
    int idxFinal = disFinal(gen);
    loc5->setPuzzle(make_unique<RiddlePuzzle>(finalRiddles[idxFinal].first, finalRiddles[idxFinal].second, 30));
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
        case Action::QuickSave:
            saveGame();
            break;
        case Action::QuickLoad:
            loadGame();
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
        cout << "Нажми Enter...";
        cin.ignore();
        cin.get();
        return;
    }
    
    m_playerX = newX;
    m_playerY = newY;
    
    if (target == 'D') {
        Location* currentLoc = m_locations[m_currentLocationId].get();
        
        vector<pair<int,int>> doors;
        for (int y = 0; y < (int)m_map.size(); y++) {
            for (int x = 0; x < (int)m_map[y].size(); x++) {
                if (m_map[y][x] == 'D') {
                    doors.push_back({x, y});
                }
            }
        }
        
        int doorIndex = -1;
        for (size_t i = 0; i < doors.size(); i++) {
            if (doors[i].first == newX && doors[i].second == newY) {
                doorIndex = i;
                break;
            }
        }
        
        const auto& connections = currentLoc->getConnections();
        int targetId = -1;
        if (doorIndex >= 0 && doorIndex < (int)connections.size()) {
            targetId = connections[doorIndex];
        }
        
        if (targetId == -1) {
            cout << "\n[!] Ошибка: нет связи для этой двери!\n";
            cout << "Нажми Enter...";
            cin.get();
            return;
        }
        
        Location* targetLoc = m_locations[targetId - 1].get();
        
        if (!targetLoc->isOpen()) {
            bool hasKey = (*m_player).getItemCount(10) > 0;
            
            if (hasKey) {
                cout << "\n[!] У тебя есть ключ! Использовать? (y/n): ";
                char useKey;
                cin >> useKey;
                if (useKey == 'y' || useKey == 'Y') {
                    (*m_player).remove_from_inventory_by_id(10);
                    targetLoc->open();
                    cout << "\n[!] Ты использовал ключ! Локация открыта!\n";
                    cout << "Нажми Enter...";
                    cin.get();
                }
            }
            
            if (!targetLoc->isOpen()) {
                int damage = 0;
                string answer;
                cout << "\n[!] Локация закрыта! Реши головоломку:\n";
                if (targetLoc->getPuzzle()) {
                    cout << targetLoc->getPuzzle()->getPrompt() << "\n";
                }
                cout << "Ответ: ";
                cin.ignore();
                getline(cin, answer);
                
                string trimmedAnswer = trim(answer);
                
                if (targetLoc->trySolvePuzzle(trimmedAnswer, damage)) {
                    cout << "\n[!] Правильно! Локация открыта!\n";
                    targetLoc->open();
                } else {
                    cout << "\n[!] Неправильно! Урон: " << damage << "\n";
                    (*m_player).attack_me(damage);
                    if ((*m_player).get_health() <= 0) {
                        cout << "\n[!] Ты погиб...\n";
                        cout << "Хотите начать заново? (y/n): ";
                        char again;
                        cin >> again;
                        if (again == 'y' || again == 'Y') resetGame();
                        else m_isRunning = false;
                        return;
                    }
                    cout << "Нажми Enter...";
                    cin.get();
                    return;
                }
            }
        }
        
        if (targetLoc->isOpen()) {
            m_currentLocationId = targetId - 1;
            m_map = m_locations[m_currentLocationId]->getAsciiArt();
            
            bool foundStart = false;
            for (int y = 0; y < (int)m_map.size() && !foundStart; y++) {
                for (int x = 0; x < (int)m_map[y].size(); x++) {
                    if (m_map[y][x] == 'D') {
                        m_playerX = x;
                        m_playerY = y;
                        foundStart = true;
                        break;
                    }
                }
            }
            if (!foundStart) {
                m_playerX = 1;
                m_playerY = 1;
            }
            
            cout << "\n[!] Ты перешёл в: " << targetLoc->getName() << "\n";
            if (m_currentLocationId == (int)m_locations.size() - 1) {
                cout << "[!] Это последняя локация! Найди сокровище!\n";
            }
            cout << "Нажми Enter...";
            cin.get();
        }
    }
}

void GameEngine::interact() {
    char target = m_map[m_playerY][m_playerX];
    
    if (target == 'C') {
        m_map[m_playerY][m_playerX] = '.';
        static int chestNumber = 0;
        chestNumber++;

        if (m_currentLocationId == 4) {
            cout << "\n========================================\n";
            cout << "  Ты открыл древний сундук!\n";
            cout << "  Внутри сиял огромный АЛМАЗ!\n";
            cout << "  Ты стал богат и славен!\n";
            cout << "  Это ПОБЕДА!\n";
            cout << "========================================\n";
            m_victory = true;
            return;
        }
        
        if (chestNumber % 2 == 1) {
            (*m_player).add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
            cout << "\n[!] Ты открыл сундук и нашёл Малое зелье!\n";
        } else {
            (*m_player).add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
            cout << "\n[!] Ты открыл сундук и нашёл Большое зелье!\n";
        }
        cout << "Нажми Enter...";
        cin.ignore();
        cin.get();
    } 
    else if (target == 'N') {
        // Мудрец в локации 2 (индекс 1)
        if (m_currentLocationId == 1) {
            cout << "\n[Мудрец]: Приветствую, путник!\n";
            cout << "1 - Привет, ищу выход. Можешь помочь?\n";
            cout << "2 - Отойди, старик!\n";
            cout << "Выбор: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                cout << "\n[Мудрец]: Помогу, но сначала ответь на вопрос.\n";
                cout << "Что можно держать, но нельзя потрогать?\n";
                cout << "Ответ: ";
                
                string answer;
                cin.ignore();
                getline(cin, answer);
                
                if (trim(answer) == "слово" || trim(answer) == "Слово" || 
                    trim(answer) == "slovo" || trim(answer) == "Slovo" ||
                    trim(answer) == "обещание" || trim(answer) == "obeshaniye") {
                    cout << "\n[Мудрец]: Верно! Держи ключ.\n";
                    (*m_player).add_to_inventory(make_unique<Item>(10, 0, "Ключ"));
                    cout << "\n[!] Ты получил КЛЮЧ!\n";
                    m_map[m_playerY][m_playerX] = '.';
                } else {
                    cout << "\n[Мудрец]: Неправильно! Получи урон!\n";
                    (*m_player).attack_me(15);
                    cout << "\n[!] Ты получил 15 урона! HP: " << (*m_player).get_health() << "\n";
                }
            } else {
                cout << "\n[Мудрец]: Грубиян! Уходи!\n";
            }
        } 
        // Торговец в локации 3 (индекс 2)
        else if (m_currentLocationId == 2) {
            cout << "\n[Торговец]: Заходи, путник! Хорошие товары!\n";
            cout << "1 - Что у тебя есть?\n";
            cout << "2 - Мне ничего не нужно.\n";
            cout << "Выбор: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                cout << "\n[Торговец]: У меня есть отличный ТОПОР! Отдам бесплатно!\n";
                cout << "1 - Взять топор\n";
                cout << "2 - Не надо\n";
                cout << "Выбор: ";
                
                cin >> choice;
                if (choice == 1) {
                    (*m_player).add_to_inventory(make_unique<Weapon>(4, "Топор", 15));
                    cout << "\n[!] Ты получил ТОПОР!\n";
                    m_map[m_playerY][m_playerX] = '.';
                }
            }
        } 
        // Стражник в локации 4 (индекс 3)
        else if (m_currentLocationId == 3) {
            cout << "\n[Стражник]: Стой! Прохода нет!\n";
            cout << "1 - Пропусти, я ищу выход\n";
            cout << "2 - Убирайся с дороги!\n";
            cout << "Выбор: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 1) {
                cout << "\n[Стражник]: Докажи свою силу в бою, и я пропущу!\n";
                cout << "1 - Хорошо, сражусь\n";
                cout << "2 - Пожалуй, уйду\n";
                cout << "Выбор: ";
                
                cin >> choice;
                if (choice == 1) {
                    NPC guard(100, "Стражник", 50, 12, NpcState::Hostile);
                    CombatState result = CombatSystem::runTurnBased((*m_player), guard);
                    
                    if (result == CombatState::PlayerWon) {
                        cout << "\n[Стражник]: Ты силён! Проходи!\n";
                        (*m_player).add_to_inventory(make_unique<Item>(10, 0, "Ключ"));
                        cout << "\n[!] Ты получил КЛЮЧ!\n";
                        m_map[m_playerY][m_playerX] = '.';
                    } else if (result == CombatState::PlayerLost) {
                        cout << "\n[!] Ты погиб в бою...\n";
                        cout << "Хотите начать заново? (y/n): ";
                        char again;
                        cin >> again;
                        if (again == 'y' || again == 'Y') resetGame();
                        else m_isRunning = false;
                        return;
                    }
                }
            } else {
                cout << "\n[Стражник]: Тогда пеняй на себя!\n";
                NPC guard(100, "Стражник", 50, 12, NpcState::Hostile);
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
                    if (again == 'y' || again == 'Y') resetGame();
                    else m_isRunning = false;
                    return;
                }
            }
        }
        cout << "Нажми Enter...";
        cin.ignore();
        cin.get();
    } 
    else {
        cout << "\n[!] Здесь не с чем взаимодействовать\n";
        cout << "Нажми Enter...";
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
        
        if (smallCount > 0) cout << "1 - Малое зелье x" << smallCount << "\n";
        if (bigCount > 0) cout << "2 - Большое зелье x" << bigCount << "\n";
        if (hasSword) cout << "3 - Меч\n";
        if (hasAxe) cout << "4 - Топор\n";
        if (keyCount > 0) cout << "5 - Ключ x" << keyCount << "\n";
        
        if (smallCount == 0 && bigCount == 0 && !hasSword && !hasAxe && keyCount == 0) {
            cout << "Инвентарь пуст!\n";
            cout << "Нажми Enter...";
            cin.ignore();
            cin.get();
            return;
        }
        
        cout << "0 - Выход\n===============================\nВыбор: ";
        int id;
        cin >> id;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        
        if (id == 0) return;
        
        if (id == 1 && smallCount > 0) {
            (*m_player).use_potion(1);
            cout << "HP: " << (*m_player).get_health() << "\n";
        } else if (id == 2 && bigCount > 0) {
            (*m_player).use_potion(2);
            cout << "HP: " << (*m_player).get_health() << "\n";
        } else if (id == 3 && hasSword) {
            (*m_player).equipWeapon(3);
            cout << "Меч экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
        } else if (id == 4 && hasAxe) {
            (*m_player).equipWeapon(4);
            cout << "Топор экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
        } else if (id == 5 && keyCount > 0) {
            cout << "Ключ открывает закрытые локации.\n";
        }
        cout << "Нажми Enter...";
        cin.ignore();
        cin.get();
    }
}

void GameEngine::showEquipMenu() {
    cout << "\n=== ЭКИПИРОВКА ОРУЖИЯ ===\n";
    if ((*m_player).hasItem(3)) cout << "1 - Меч\n";
    if ((*m_player).hasItem(4)) cout << "2 - Топор\n";
    cout << "0 - Отмена\nВыбор: ";
    
    int choice;
    cin >> choice;
    
    if (choice == 1 && (*m_player).hasItem(3)) {
        (*m_player).equipWeapon(3);
        cout << "Меч экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
    } else if (choice == 2 && (*m_player).hasItem(4)) {
        (*m_player).equipWeapon(4);
        cout << "Топор экипирован! Урон: " << (*m_player).getTotalDamage() << "\n";
    }
    cout << "Нажми Enter...";
    cin.ignore();
    cin.get();
}

void GameEngine::saveGame() {
    SaveData data;
    data.health = (*m_player).get_health();
    data.damage = (*m_player).get_damage();
    data.equippedWeaponId = (*m_player).getEquippedWeaponId();
    data.locationName = (*m_player).get_location();
    data.currentLocationId = m_currentLocationId;
    data.playerX = m_playerX;
    data.playerY = m_playerY;
    data.victory = m_victory;
    data.isRunning = m_isRunning;
    
    for (int i = 1; i <= 10; i++) {
        if ((*m_player).hasItem(i)) data.inventoryIds.push_back(i);
    }
    for (auto& loc : m_locations) data.locationStates.push_back(loc->isOpen());
    
    if (SaveManager::save("savegame.save", data)) {
        cout << "\n[!] Игра сохранена!\n";
        Logger::log("Game saved");
    }
    cout << "Нажми Enter...";
    cin.ignore();
    cin.get();
}

bool GameEngine::loadGame() {
    if (!SaveManager::saveExists("savegame.save")) {
        cout << "\n[!] Нет сохранения!\n";
        cout << "Нажми Enter...";
        cin.ignore();
        cin.get();
        return false;
    }
    
    SaveData data;
    if (!SaveManager::load("savegame.save", data)) {
        cout << "[!] Ошибка загрузки!\n";
        return false;
    }
    
    m_player = make_unique<Character>(data.locationName);
    (*m_player).add_health(data.health - 100);
    (*m_player).set_damage(data.damage);
    if (data.equippedWeaponId != -1) (*m_player).equipWeapon(data.equippedWeaponId);
    
    for (int id : data.inventoryIds) {
        if (id == 1) (*m_player).add_to_inventory(make_unique<HealthPotion>(1, "Малое зелье", 30));
        if (id == 2) (*m_player).add_to_inventory(make_unique<HealthPotion>(2, "Большое зелье", 60));
        if (id == 3) (*m_player).add_to_inventory(make_unique<Weapon>(3, "Меч", 8));
        if (id == 4) (*m_player).add_to_inventory(make_unique<Weapon>(4, "Топор", 15));
        if (id == 10) (*m_player).add_to_inventory(make_unique<Item>(10, 0, "Ключ"));
    }
    
    m_currentLocationId = data.currentLocationId;
    m_playerX = data.playerX;
    m_playerY = data.playerY;
    m_victory = data.victory;
    m_isRunning = data.isRunning;
    
    setupLocations();
    for (size_t i = 0; i < data.locationStates.size() && i < m_locations.size(); i++) {
        if (data.locationStates[i]) m_locations[i]->open();
    }
    
    m_map = m_locations[m_currentLocationId]->getAsciiArt();
    cout << "\n[!] Игра загружена!\n";
    Logger::log("Game loaded");
    cout << "Нажми Enter...";
    cin.ignore();
    cin.get();
    return true;
}