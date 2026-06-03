#include "PuzzleFactory.hpp"

#include "CodePuzzle.hpp"
#include "MathPuzzle.hpp"
#include "RiddlePuzzle.hpp"

using namespace std;

unique_ptr<Puzzle> PuzzleFactory::createById(const string& puzzleId) {
    if (puzzleId == "riddle_crypt") {
        return make_unique<RiddlePuzzle>(
            "Загадка: что всегда уходит, но остаётся на месте?",
             "время");
    }
    if (puzzleId == "math_gate") {
        return make_unique<MathPuzzle>(17, 5, '*');
    }
    if (puzzleId == "code_vault") {
        return make_unique<CodePuzzle>(
            "На стене выцарапано: «Код из 4 букв — имя стража лабиринта»",
            "орк");
    }
    if (puzzleId == "riddle_hall") {
        return make_unique<RiddlePuzzle>(
            "Чем больше забираешь, тем больше становится?",
            "яма");
    }
    if (puzzleId == "math_chest") {
        return make_unique<MathPuzzle>(144, 12, '/');
    }
    return nullptr;
}
