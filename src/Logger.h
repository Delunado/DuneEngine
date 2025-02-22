#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <vector>

enum LogType {
    INFO,
    WARNING,
    ERROR
};

struct LogEntry {
    LogType type;
    std::string message;

    LogEntry(LogType type, const std::string &message) : type(type), message(message) {
    }
};

class Logger {
public:
    static void Log(const std::string &message);

    static void Warning(const std::string &message);

    static void Error(const std::string &message);

private:
    static void PrintFormatted(const std::string &color, const std::string &message);

    static void SaveMessage(const std::string &message, LogType type);

    static std::vector<LogEntry> _messages;
};

#endif //LOGGER_H
