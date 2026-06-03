#include "GameRules.hpp"
#include "Character.hpp"
#include "Location.hpp"
#include "Puzzle.hpp"
#include "Logger.hpp"

using namespace std;

bool GameRules::canMoveTo(const Character& player, const Location& from, const Location& to) {
    if (player.get_health() <= 0) return false;
    
    bool connected = false;
    for (int id : from.getConnections()) {
        if (id == to.getId()) { connected = true; break; }
    }
    if (!connected) return false;
    if (!to.isOpen()) return false;
    return true;
}

bool GameRules::checkVictory(const Character& player, const Location& current) {
    if (player.get_health() <= 0) return false;
    return current.isOpen() && current.getConnections().empty();
}

MoveResult GameRules::tryMove(Character& player, Location& from, Location& to) {
    if (!canMoveTo(player, from, to)) return MoveResult::Blocked;
    
    player.set_location(to_string(to.getId()));
    if (checkVictory(player, to)) return MoveResult::Victory;
    return MoveResult::Moved;
}

MoveResult GameRules::trySolvePuzzle(Character& player, Location& lockedLocation, const string& answer) {
    if (lockedLocation.isOpen()) return MoveResult::Moved;
    
    Puzzle* puzzle = lockedLocation.getPuzzle();
    if (!puzzle) return MoveResult::Blocked;
    
    if (puzzle->checkAnswer(answer)) {
        lockedLocation.open();
        return MoveResult::PuzzleSolved;
    } else {
        applyPuzzleFailure(player, lockedLocation);
        if (player.get_health() <= 0) return MoveResult::PlayerDead;
        return MoveResult::PuzzleFailed;
    }
}

void GameRules::applyPuzzleFailure(Character& player, const Location& loc) {
    int dmg = loc.getPuzzleFailDamage();
    player.attack_me(dmg);
}