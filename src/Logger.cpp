#include "Logger.h"

#include <ctime>
#include <iostream>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"

void Logger::PrintFormatted(const std::string &color, const std::string &message) {
    time_t now = time(0);
    tm *localTime = localtime(&now);

    std::cout << color << "LOG " << "[" << localTime->tm_year + 1900 << "/" << localTime->tm_mon + 1 << "/" <<
            localTime->
            tm_mday << " " << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << "] "
            << message << RESET << std::endl;
}

void Logger::Log(const std::string &message) {
    PrintFormatted(GREEN, message);
}

void Logger::Error(const std::string &message) {
    PrintFormatted(RED, message);
}
