#include "Logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"

std::vector<LogEntry> Logger::_messages;

void Logger::Log(const std::string &message) {
    PrintFormatted(GREEN, message);
    SaveMessage(message, INFO);
}

void Logger::Warning(const std::string &message) {
    PrintFormatted(YELLOW, message);
    SaveMessage(message, WARNING);
}

void Logger::Error(const std::string &message) {
    PrintFormatted(RED, message);
    SaveMessage(message, ERROR);
}

void Logger::PrintFormatted(const std::string &color, const std::string &message) {
    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);

    std::cout << color << "LOG " << "[" << std::put_time(std::localtime(&time), "%Y/%m/%d %H:%M:%S") << "] " << message
            << RESET << std::endl;
}

void Logger::SaveMessage(const std::string &message, LogType type) {
    const LogEntry entry(type, message);
    _messages.push_back(entry);
}
