#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

class Logger {
public:
    static void init(const std::string& filename);
    static void log(const std::string& message);
    static void error(const std::string& message);
    static void close();
};

#endif