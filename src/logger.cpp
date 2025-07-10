#include "Logger.hpp"

Logger::Logger(const std::string& filename, bool logToConsole) : consoleOutput(logToConsole) {
    if (!filename.empty()) {
        logFile.open(filename, std::ios::app);
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message, LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::string fullMessage = "[" + getTimestamp() + "] [" + levelToString(level) + "] " + message;

    if (consoleOutput) {
        std::cout << fullMessage << std::endl;
    }

    if (logFile.is_open()) {
        logFile << fullMessage << std::endl;
    }
}

std::string Logger::getTimestamp() {
    time_t now = time(0);
    char buf[80];
    tm* timeinfo = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buf;
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
