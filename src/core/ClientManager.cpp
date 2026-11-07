#include "core/ClientManager.h"
#include <sys/socket.h>

void ClientManager::add(int fd)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients_[fd] = Client{""};
}
void ClientManager::remove(int fd)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients_.erase(fd);
}
bool ClientManager::login(int fd, const std::string& name)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    for(const auto& [otherFd, client] : clients_)
    {
        if(client.name == name)
        {
            return false; // Name already taken
        }
    }
    clients_[fd] = {name};
    return true;
}
std::string ClientManager::nameOf(int fd) const
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    auto it = clients_.find(fd);
    return it == clients_.end() ? "" : it->second.name;
}
void ClientManager::sendTo(int fd, const std::string& message)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    sendAll(fd, message + "\n" );
}
void ClientManager::broadcast(const std::string& message, int exceptFd)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    for(const auto& [fd, client] : clients_)
    {
        if(fd != exceptFd && !client.name.empty())
        {
            sendAll(fd, message + "\n");
        }
    }
}
void ClientManager::sendAll(int fd, const std::string& data)
{
    size_t sent = 0;
    while (sent < data.size())
    {
        ssize_t n = send(fd, data.data() + sent, data.size() - sent, MSG_NOSIGNAL);
        if (n <= 0)
        {
            return;
        }
        sent += n;
       
    }
}