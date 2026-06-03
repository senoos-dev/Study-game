#ifndef MATH_PUZZLE_HPP
#define MATH_PUZZLE_HPP

#include "Puzzle.hpp"

class MathPuzzle : public Puzzle {
public:
    MathPuzzle(int a, int b, char op);

    string getPrompt() const override;
    bool checkAnswer(const string& answer) const override;
    string getTypeName() const override;

private:
    int m_a;
    int m_b;
    char m_op;
    int m_result;
};

#endif
