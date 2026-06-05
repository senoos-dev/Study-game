#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <string> 

enum class Action {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Interact, OpenInventory, EquipWeapon, Quit, None,
    Attack, UsePotion, Flee,
    SelectOne, SelectTwo, SelectThree, QuickSave, QuickLoad
};

class InputHandler {
public:
    static Action getAction();
    static Action getBattleAction();
    static int getNumberInput();
    static std::string getStringInput(); 
};

#endif