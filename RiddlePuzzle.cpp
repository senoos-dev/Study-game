#include "RiddlePuzzle.hpp"

#include <algorithm>
#include <cctype>

using namespace std;

static string trimAndLower(string s) {
    while (!s.empty() && isspace((unsigned char)s.front())) {
        s.erase(s.begin());
    }
    while (!s.empty() && isspace((unsigned char)s.back())) {
        s.pop_back();
    }
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
    return s;
}

RiddlePuzzle::RiddlePuzzle(const string& question, const string& correctAnswer)
    : m_question(question), m_answer(correctAnswer) {}

string RiddlePuzzle::getPrompt() const {
    return m_question;
}

bool RiddlePuzzle::checkAnswer(const string& answer) const {
    return trimAndLower(answer) == trimAndLower(m_answer);
}

string RiddlePuzzle::getTypeName() const {
    return "riddle";
}
