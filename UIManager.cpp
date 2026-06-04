#include "UIManager.hpp"
#include <iostream>

using namespace std;

void UIManager::showWelcomeScreen() {
    cout << "\n";
    cout << "========================================\n";
    cout << "     ПОДЗЕМНЫЙ ЛАБИРИНТ\n";
    cout << "     Текстовый квест\n";
    cout << "========================================\n";
    cout << "Найди выход из лабиринта,\n";
    cout << "решай головоломки и сражайся с врагами!\n";
    cout << "========================================\n";
    cout << "\nНажми Enter чтобы начать...";
    cin.get();
}

void UIManager::showGameOverScreen(bool victory) {
    cout << "\n========================================\n";
    if (victory) {
        cout << "              ПОБЕДА!\n";
        cout << "  Ты выбрался из лабиринта!\n";
    } else {
        cout << "             ИГРА ОКОНЧЕНА\n";
        cout << "  Ты не смог выбраться...\n";
    }
    cout << "========================================\n";
}

void UIManager::showPuzzlePrompt(const string& question) {
    cout << "\n========== ГОЛОВОЛОМКА ==========\n";
    cout << question << "\n";
    cout << "Ваш ответ: ";
}

void UIManager::showDialogueLine(const string& speaker, const string& line) {
    cout << "\n[" << speaker << "]: " << line << "\n";
}

void UIManager::showDialogueOptions(const vector<string>& options) {
    cout << "\nВарианты ответа:\n";
    for (int i = 0; i < (int)options.size(); i++) {
        cout << "  " << i + 1 << ") " << options[i] << "\n";
    }
    cout << "  0) Выйти из диалога\n";
    cout << "Выбор: ";
}

void UIManager::showMessage(const string& message) {
    cout << "\n[!] " << message << "\n";
}

void UIManager::waitForEnter() {
    cout << "\nНажми Enter чтобы продолжить...";
    cin.get();
    cin.get();
}