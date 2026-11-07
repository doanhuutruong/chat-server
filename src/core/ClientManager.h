#pragma once
#include  <map>
#include  <mutex>
#include  <string>

class ClientManager
{   
public:
    void add(int fd);
    void remove(int fd);
    bool login(int fd, const std::string& name);
    std::string nameOf(int fd) const;
    
    void  sendTo(int fd, const std::string& message);
    void broadcast(const std::string& message, int exceptFd = -1);
private:
    struct Client
    {
        std::string name;
    };
    static void sendAll(int fd, const std::string& data);

    mutable std::mutex clients_mutex;
    std::map<int, Client> clients_;
};