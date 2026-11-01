#include "util/Logger.h"
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h> 
#include <thread>

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
        send(client_sockfd, buffer, n, 0);    
    }
    Logger::Instance().log("Client disconnected");
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
        std::thread(handle_client, client_sockfd).detach();
        Logger::Instance().log("Client connected, fd " + std::to_string(client_sockfd));
    }   
        
}