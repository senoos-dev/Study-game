#include "CodePuzzle.hpp"
#include <algorithm>
#include <cctype>

CodePuzzle::CodePuzzle(const std::string& prompt, const std::string& code, int penaltyDamage)
    : m_prompt(prompt), m_code(code), m_penaltyDamage(penaltyDamage) {}

std::string CodePuzzle::getPrompt() const {
    return m_prompt;
}

bool CodePuzzle::checkAnswer(const std::string& answer) const {
    std::string a = answer;
    std::string b = m_code;
    for (char& c : a) c = std::tolower(c);
    for (char& c : b) c = std::tolower(c);
    return a == b;
}

std::string CodePuzzle::getTypeName() const {
    return "CodePuzzle";
}

int CodePuzzle::getPenaltyDamage() const {
    return m_penaltyDamage;
}