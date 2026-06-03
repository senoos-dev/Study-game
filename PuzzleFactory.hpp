#ifndef PUZZLE_FACTORY_HPP
#define PUZZLE_FACTORY_HPP

#include <memory>
#include <string>

#include "Puzzle.hpp"

using namespace std;

class PuzzleFactory {
public:
    // id из данных игры: "riddle_crypt", "math_gate", "code_vault", ...
    static unique_ptr<Puzzle> createById(const string& puzzleId);
};

#endif
