#include "MathPuzzle.hpp"

#include <sstream>
#include <stdexcept>

using namespace std;

MathPuzzle::MathPuzzle(int a, int b, char op) : m_a(a), m_b(b), m_op(op) {
    switch (m_op) {
    case '+':
        m_result = a + b;
        break;
    case '-':
        m_result = a - b;
        break;
    case '*':
        m_result = a * b;
        break;
    case '/':
        if (b == 0) {
            throw invalid_argument("MathPuzzle: division by zero");
        }
        m_result = a / b;
        break;
    default:
        throw invalid_argument("MathPuzzle: unsupported operator");
    }
}

string MathPuzzle::getPrompt() const {
    ostringstream oss;
    oss << "Решите: " << m_a << " " << m_op << " " << m_b << " = ?";
    return oss.str();
}

bool MathPuzzle::checkAnswer(const string& answer) const {
    try {
        size_t idx = 0;
        int value = stoi(answer, &idx);
        if (idx != answer.size()) {
            return false;
        }
        return value == m_result;
    } catch (...) {
        return false;
    }
}

string MathPuzzle::getTypeName() const {
    return "math";
}
