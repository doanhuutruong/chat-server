#pragma once
#include <sys/types.h>
#include <mutex>
#include <string>

class HistoryProcess
{
public:
    explicit HistoryProcess(const std::string& filePath) : filePath_(filePath) {};
    bool start();
    void write(const std::string& line);
private:
    void childMain(int readFD);

    std::string filePath_;
    int writeFd_ = -1;
    std::mutex mutex_;
};