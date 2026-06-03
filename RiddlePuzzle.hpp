#ifndef RIDDLE_PUZZLE_HPP
#define RIDDLE_PUZZLE_HPP

#include "Puzzle.hpp"
#include <string>

class RiddlePuzzle : public Puzzle {
public:
    RiddlePuzzle(const std::string& prompt, const std::string& answer, int penaltyDamage = 10);
    
    std::string getPrompt() const override;
    bool checkAnswer(const std::string& answer) const override;
    std::string getTypeName() const override;
    int getPenaltyDamage() const override;

private:
    std::string m_prompt;
    std::string m_answer;
    int m_penaltyDamage;
};

#endif