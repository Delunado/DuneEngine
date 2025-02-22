#ifndef LOGGER_H
#define LOGGER_H
#include <string>

class Logger {
public:
    static void Log(const std::string &message);

    static void Error(const std::string &message);

private:
    static void PrintFormatted(const std::string &color, const std::string &message);
};

#endif //LOGGER_H
