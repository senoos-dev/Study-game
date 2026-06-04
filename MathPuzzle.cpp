#include "MathPuzzle.hpp"
#include <string>

MathPuzzle::MathPuzzle(int a, int b, char op, int penaltyDamage)
    : m_a(a), m_b(b), m_op(op), m_penaltyDamage(penaltyDamage) {
    if (op == '+') m_result = a + b;
    else if (op == '-') m_result = a - b;
    else if (op == '*') m_result = a * b;
    else m_result = a + b;
}

std::string MathPuzzle::getPrompt() const {
    return std::to_string(m_a) + " " + m_op + " " + std::to_string(m_b) + " = ?";
}

bool MathPuzzle::checkAnswer(const string& answer) const {
    if (answer.empty()) return false;
    try {
        return stoi(answer) == m_result;
    } catch (...) {
        return false;
    }
}

std::string MathPuzzle::getTypeName() const {
    return "MathPuzzle";
}

int MathPuzzle::getPenaltyDamage() const {
    return m_penaltyDamage;
}