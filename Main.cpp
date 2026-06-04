#include "GameEngine.hpp"
#include <iostream>


int main() {
    system("chcp 65001 > nul");
    GameEngine engine;
    
    if (!engine.init()) {
        std::cerr << "Ошибка инициализации игры!" << std::endl;
        return 1;
    }
    
    engine.run();
    
    return 0;
}