#include "GameRules.hpp"

#include "Logger.hpp"

using namespace std;

bool GameRules::canMoveTo(const Character& player, const Location& from, const Location& to) {
    if (!player.is_alive()) {
        return false;
    }
    if (!from.canReach(to.getId())) {
        return false;
    }
    if (!to.isOpen()) {
        return false;
    }
    return true;
}

bool GameRules::checkVictory(const Character& player, const Location& current) {
    if (!player.is_alive()) {
        return false;
    }
    return current.isFinal() && player.get_location() == current.getId();
}

MoveResult GameRules::tryMove(Character& player, Location& from, Location& to) {
    if (!from.canReach(to.getId())) {
        Logger::log("Move blocked: no connection " + from.getId() + " -> " + to.getId());
        return MoveResult::Blocked;
    }

    if (!to.isOpen()) {
        if (to.hasPuzzle()) {
            Logger::log("Move needs puzzle at " + to.getId());
            return MoveResult::NeedPuzzle;
        }
        Logger::log("Move blocked: location closed " + to.getId());
        return MoveResult::Blocked;
    }

    player.set_location(to.getId());
    Logger::log("Player moved: " + from.getId() + " -> " + to.getId());

    if (checkVictory(player, to)) {
        Logger::log("Victory at " + to.getId());
        return MoveResult::Victory;
    }

    return MoveResult::Moved;
}

MoveResult GameRules::trySolvePuzzle(Character& player, Location& lockedLocation, const string& answer) {
    if (lockedLocation.isOpen()) {
        return MoveResult::Moved;
    }

    Puzzle* puzzle = lockedLocation.getPuzzle();
    if (puzzle == nullptr) {
        Logger::log("Puzzle missing at " + lockedLocation.getId());
        return MoveResult::Blocked;
    }

    if (puzzle->checkAnswer(answer)) {
        lockedLocation.setOpen(true);
        Logger::log("Puzzle solved (" + puzzle->getTypeName() + ") at " + lockedLocation.getId());
        return MoveResult::PuzzleSolved;
    }

    applyPuzzleFailure(player, lockedLocation);
    Logger::log("Puzzle failed (" + puzzle->getTypeName() + ") at " + lockedLocation.getId());
    if (!player.is_alive()) {
        return MoveResult::PlayerDead;
    }
    return MoveResult::PuzzleFailed;
}

void GameRules::applyPuzzleFailure(Character& player, const Location& loc) {
    int dmg = loc.getPuzzleFailDamage();
    player.attack_me(dmg);
    Logger::log("Puzzle damage " + to_string(dmg) + " to player");
}
