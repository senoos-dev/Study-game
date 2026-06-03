#include "Logger.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

ofstream Logger::s_file;
mutex Logger::s_mutex;
bool Logger::s_initialized = false;

string Logger::timestamp() {
    auto now = chrono::system_clock::now();
    auto t = chrono::system_clock::to_time_t(now);
    tm localTm{};
#ifdef _WIN32
    localtime_s(&localTm, &t);
#else
    localtime_r(&t, &localTm);
#endif
    ostringstream oss;
    oss << put_time(&localTm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Logger::init(const string& filePath) {
    lock_guard<mutex> lock(s_mutex);
    if (s_initialized) {
        return;
    }
    s_file.open(filePath, ios::app);
    if (!s_file.is_open()) {
        cerr << "Logger: cannot open " << filePath << endl;
        return;
    }
    s_initialized = true;
    s_file << "[" << timestamp() << "] Logger started" << endl;
}

void Logger::log(const string& event) {
    lock_guard<mutex> lock(s_mutex);
    string line = "[" + timestamp() + "] " + event;
    if (s_initialized && s_file.is_open()) {
        s_file << line << endl;
    }
    cout << "[LOG] " << event << endl;
}

void Logger::close() {
    lock_guard<mutex> lock(s_mutex);
    if (s_initialized && s_file.is_open()) {
        s_file << "[" << timestamp() << "] Logger stopped" << endl;
        s_file.close();
        s_initialized = false;
    }
}
