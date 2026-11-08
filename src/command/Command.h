#pragma once
class ClientManager;
class HistoryProcess;


struct CommandContext
{
    int fd;                  
    ClientManager& clients;
    HistoryProcess& history;
};

class Command
{
public:
    virtual ~Command() = default;
    // Trả về false nếu cần ngắt kết nối client.
    virtual bool execute(CommandContext& ctx) = 0;
};