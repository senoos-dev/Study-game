#include "InputHandler.hpp"
#include <iostream>

using namespace std;

Action InputHandler::getAction() {
    char input;
    cin >> input;
    
    switch (input) {
        case 'w': case 'W': return Action::MoveUp;
        case 's': case 'S': return Action::MoveDown;
        case 'a': case 'A': return Action::MoveLeft;
        case 'd': case 'D': return Action::MoveRight;
        case 'e': case 'E': return Action::Interact;
        case 'i': case 'I': return Action::OpenInventory;
        case 'r': case 'R': return Action::EquipWeapon;
        case 'z': case 'Z': return Action::QuickSave;
        case 'x': case 'X': return Action::QuickLoad;
        case 'q': case 'Q': return Action::Quit;
        default: return Action::None;
    }
}

Action InputHandler::getBattleAction() {
    int input;
    cin >> input;
    
    switch (input) {
        case 1: return Action::Attack;
        case 2: return Action::UsePotion;
        case 3: return Action::Flee;
        default: return Action::None;
    }
}

int InputHandler::getNumberInput() {
    int value;
    cin >> value;
    return value;
}

string InputHandler::getStringInput() {
    string value;
    cin >> value;
    return value;
}