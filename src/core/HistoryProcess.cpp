#include "core/HistoryProcess.h"
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <fstream>
#include "util/Logger.h"

bool HistoryProcess::start()
{
    int fds[2];
    if(pipe(fds) == -1)
    {
        perror("pipe");
        return false;   
    }
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return false;
    }
    if(pid == 0)
    {
        close(fds[1]); 
        childMain(fds[0]);
        _exit(0);
    }
    close(fds[0]);
    writeFd_ = fds[1];
    Logger::Instance().log("History process started, pid " + std::to_string(pid));
    return true;
    
}
void HistoryProcess::childMain(int readFD)
{
    signal(SIGPIPE, SIG_IGN);
    std::ofstream file(filePath_, std::ios::app);
    char buffer[1024];
    while(true)
    {
        ssize_t n = read(readFD, buffer, sizeof(buffer));
        if(n <= 0)
        {
            break;
        }
        file.write(buffer, n);
        file.flush();
    }
    close(readFD);
}
void HistoryProcess::write(const std::string& line)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::string data = line + "\n";
    ::write(writeFd_, data.c_str(), data.size());
}