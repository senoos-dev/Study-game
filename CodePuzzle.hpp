#ifndef CODE_PUZZLE_HPP
#define CODE_PUZZLE_HPP

#include "Puzzle.hpp"
#include <string>

class CodePuzzle : public Puzzle {
public:
    CodePuzzle(const std::string& prompt, const std::string& code, int penaltyDamage = 10);
    
    std::string getPrompt() const override;
    bool checkAnswer(const std::string& answer) const override;
    std::string getTypeName() const override;
    int getPenaltyDamage() const override;

private:
    std::string m_prompt;
    std::string m_code;
    int m_penaltyDamage;
};

#endif