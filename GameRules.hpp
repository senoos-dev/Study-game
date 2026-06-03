#ifndef GAME_RULES_HPP
#define GAME_RULES_HPP

#include <string>

#include "../Character.hpp"
#include "../Location.hpp"

using namespace std;

enum class MoveResult {
    Moved,
    Victory,
    Blocked,
    NeedPuzzle,
    PuzzleSolved,
    PuzzleFailed,
    PlayerDead
};

class GameRules {
public:
    static bool canMoveTo(const Character& player, const Location& from, const Location& to);
    static bool checkVictory(const Character& player, const Location& current);

    // Переход в соседнюю открытую локацию
    static MoveResult tryMove(Character& player, Location& from, Location& to);

    // Закрытая локация: проверка ответа головоломки
    static MoveResult trySolvePuzzle(Character& player, Location& lockedLocation, const string& answer);

    static void applyPuzzleFailure(Character& player, const Location& loc);
};

#endif
