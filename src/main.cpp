#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h> 
#include <thread>
#include <string>

#include "core/ClientManager.h"
#include "command/CommandFactory.h"
#include "util/Logger.h"

ClientManager clients;
CommandFactory factory;

void handle_client(int client_sockfd)
{
    std::string pending;
    char buffer[1024];
    bool connected  = true;
    while(connected ) 
    {
        ssize_t n = recv(client_sockfd, buffer, sizeof(buffer), 0);
        if(n <= 0) 
        {
            break;
        }
        pending.append(buffer, n);
        size_t pos;
        while(connected  && (pos = pending.find('\n')) != std::string::npos)
         {
            std::string line = pending.substr(0, pos);
            pending.erase(0, pos + 1);
            if(!line.empty() && line.back() == '\r') 
            {
                line.pop_back();
            }
            if(line.empty())
            {
                continue;
            }
            auto command = factory.create(line);
            CommandContext context{client_sockfd, clients};
            connected  = command->execute(context);

         }
    }
    std::string name = clients.nameOf(client_sockfd);
    clients.remove(client_sockfd);
    if(!name.empty())
    {
        clients.broadcast("* " + name + " left the chat", client_sockfd);   
    }
    Logger::Instance().log("Client disconnected, fd " + std::to_string(client_sockfd));
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
        return 1;
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
        
        clients.add(client_sockfd);
        clients.sendTo(client_sockfd, "* Welcome to the chat server!\n");
        std::thread(handle_client, client_sockfd).detach();
    }  
    close(sockfd);
    return 0; 
        
}