#include "util/Logger.h"
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h> 
#include <thread>
#include <map>
#include <mutex>

std::mutex clients_mutex;
std::map<int, std::string> clients;
void broadcast(const std::string& message, int exclude_sockfd)
{
    std::lock_guard<std::mutex> lock(clients_mutex);
    for(const auto& [fd, name] : clients)
    {
        if(fd != exclude_sockfd)
        {
            send(fd, message.c_str(), message.size(), MSG_NOSIGNAL);
        }
    }
}

void handle_client(int client_sockfd)
{
    char buffer[1024];
    while(true) 
    {
        ssize_t n = recv(client_sockfd, buffer, sizeof(buffer), 0);
        if(n <= 0) 
        {
            break;
        }
        std::string message = "[client " + std::to_string(client_sockfd) + "] "
                            + std::string(buffer, n);
        broadcast(message, client_sockfd); 
    }
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(client_sockfd);
    }
    Logger::Instance().log("Client disconnected");
    broadcast("* client " + std::to_string(client_sockfd) + " left\n", client_sockfd);
    close(client_sockfd);
}
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);   
    if(sockfd < 0)
    {
        perror("socket");
        return 1;

    }
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        
        perror("bind");
        return 1; 
    
    }
    if(listen(sockfd, 10) < 0)
    {
        perror("listen");
        return false;
    }
    Logger::Instance().log("Server is listening on port 8080");
    
    while(true)
    {
        int client_sockfd = accept(sockfd, nullptr, nullptr);
        if(client_sockfd < 0)
        {
            perror("accept");
            continue;
        }
         Logger::Instance().log("Client connected, fd " + std::to_string(client_sockfd));
        {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients[client_sockfd] = ""; 
        }
        broadcast("* client " + std::to_string(client_sockfd) + " joined\n", client_sockfd);
        std::thread(handle_client, client_sockfd).detach();
    }  
    close(sockfd);
    return 0; 
        
}