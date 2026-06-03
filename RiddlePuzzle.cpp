#include "RiddlePuzzle.hpp"
#include <algorithm>
#include <cctype>

RiddlePuzzle::RiddlePuzzle(const std::string& prompt, const std::string& answer, int penaltyDamage)
    : m_prompt(prompt), m_answer(answer), m_penaltyDamage(penaltyDamage) {}

std::string RiddlePuzzle::getPrompt() const {
    return m_prompt;
}

bool RiddlePuzzle::checkAnswer(const std::string& answer) const {
    std::string a = answer;
    std::string correct = m_answer;
    for (char& c : a) c = std::tolower(c);
    for (char& c : correct) c = std::tolower(c);
    return a == correct;
}

std::string RiddlePuzzle::getTypeName() const {
    return "RiddlePuzzle";
}

int RiddlePuzzle::getPenaltyDamage() const {
    return m_penaltyDamage;
}