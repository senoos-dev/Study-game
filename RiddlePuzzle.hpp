#ifndef RIDDLE_PUZZLE_HPP
#define RIDDLE_PUZZLE_HPP

#include "Puzzle.hpp"

class RiddlePuzzle : public Puzzle {
public:
    RiddlePuzzle(const string& question, const string& correctAnswer);

    string getPrompt() const override;
    bool checkAnswer(const string& answer) const override;
    string getTypeName() const override;

private:
    string m_question;
    string m_answer;
};

#endif
