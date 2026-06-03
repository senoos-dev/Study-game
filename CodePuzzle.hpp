#ifndef CODE_PUZZLE_HPP
#define CODE_PUZZLE_HPP

#include "Puzzle.hpp"

// Третий тип: ввод секретного кода (регистр не важен)
class CodePuzzle : public Puzzle {
public:
    CodePuzzle(const string& hint, const string& code);

    string getPrompt() const override;
    bool checkAnswer(const string& answer) const override;
    string getTypeName() const override;

private:
    string m_hint;
    string m_code;
};

#endif
