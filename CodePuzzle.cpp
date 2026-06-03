#include "CodePuzzle.hpp"

#include <algorithm>
#include <cctype>

using namespace std;

CodePuzzle::CodePuzzle(const string& hint, const string& code) : m_hint(hint), m_code(code) {}

string CodePuzzle::getPrompt() const {
    return m_hint;
}

bool CodePuzzle::checkAnswer(const string& answer) const {
    string a = answer;
    string c = m_code;
    transform(a.begin(), a.end(), a.begin(), [](unsigned char ch) { return (char)tolower(ch); });
    transform(c.begin(), c.end(), c.begin(), [](unsigned char ch) { return (char)tolower(ch); });
    return a == c;
}

string CodePuzzle::getTypeName() const {
    return "code";
}
