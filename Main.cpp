#include "GameEngine.hpp"
#include "Logger.hpp"
#include <iostream>

int main() {
    system("chcp 65001 > nul");
    Logger::init("game.log");
    
    GameEngine engine;
    
    if (!engine.init()) {
        Logger::error("Failed to initialize game");
        std::cerr << "Ошибка инициализации игры!" << std::endl;
        return 1;
    }
    
    engine.run();
    
    Logger::close();
    return 0;
}