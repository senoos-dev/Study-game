#include "Logger.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;

static ofstream logFile;
static string logFilename;

void Logger::init(const string& filename) {
    logFilename = filename;
    logFile.open(filename, ios::app);
    if (logFile.is_open()) {
        log("=== GAME START ===");
    }
}

void Logger::log(const string& message) {
    auto now = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(now);
    string timeStr = ctime(&time);
    timeStr.pop_back(); // убираем \n
    
    if (logFile.is_open()) {
        logFile << "[" << timeStr << "] " << message << endl;
        logFile.flush();
    }
    // Не выводим в консоль, чтобы не засорять
}

void Logger::error(const string& message) {
    auto now = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(now);
    string timeStr = ctime(&time);
    timeStr.pop_back();
    
    if (logFile.is_open()) {
        logFile << "[" << timeStr << "] ERROR: " << message << endl;
        logFile.flush();
    }
    cerr << "[ERROR] " << message << endl;
}

void Logger::close() {
    if (logFile.is_open()) {
        log("=== GAME END ===");
        logFile.close();
    }
}