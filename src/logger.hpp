#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const std::string& filename = "", bool logToConsole = true);
    ~Logger();

    void log(const std::string& message, LogLevel level = INFO);

private:
    std::ofstream logFile;
    bool consoleOutput;
    std::mutex logMutex;

    std::string getTimestamp();
    std::string levelToString(LogLevel level);
};

#endif
