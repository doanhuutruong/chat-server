#pragma once
#include <iostream> 
#include <string>   

class Logger
{
private:
    Logger() = default;
    ~Logger() = default;
public:
    static Logger& Instance()
    {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message)
    {
        // Implementation for logging the message
        std::cout << message << std::endl;
    }

    // Delete copy constructor and assignment operator to prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
