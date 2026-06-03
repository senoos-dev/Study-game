#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <mutex>
#include <string>

using namespace std;

class Logger {
public:
    static void init(const string& filePath = "game.log");
    static void log(const string& event);
    static void close();

private:
    static ofstream s_file;
    static mutex s_mutex;
    static bool s_initialized;
    static string timestamp();
};

#endif
