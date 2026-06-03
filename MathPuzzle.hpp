#ifndef MATH_PUZZLE_HPP
#define MATH_PUZZLE_HPP

#include "Puzzle.hpp"
#include <string>

class MathPuzzle : public Puzzle {
public:
    MathPuzzle(int a, int b, char op, int penaltyDamage = 10);
    
    std::string getPrompt() const override;
    bool checkAnswer(const std::string& answer) const override;
    std::string getTypeName() const override;
    int getPenaltyDamage() const override;

private:
    int m_a, m_b, m_result;
    char m_op;
    int m_penaltyDamage;
};

#endif