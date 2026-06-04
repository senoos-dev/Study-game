#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <string>
#include <vector>

class UIManager {
public:
    static void showWelcomeScreen();
    static void showGameOverScreen(bool victory);
    static void showPuzzlePrompt(const std::string& question);
    static void showDialogueLine(const std::string& speaker, const std::string& line);
    static void showDialogueOptions(const std::vector<std::string>& options);
    static void showMessage(const std::string& message);
    static void waitForEnter();
};

#endif