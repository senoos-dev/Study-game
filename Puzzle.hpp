#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include <string>

using namespace std;

class Puzzle {
public:
    virtual ~Puzzle() = default;

    virtual string getPrompt() const = 0;
    virtual bool checkAnswer(const string& answer) const = 0;
    virtual string getTypeName() const = 0;
};

#endif
