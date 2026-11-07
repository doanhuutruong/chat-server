#pragma once
class ClientManager;

// Những gì một lệnh cần để làm việc.
struct CommandContext
{
    int fd;                  // client gửi lệnh
    ClientManager& clients;
};

// Lớp cơ sở cho mọi lệnh (Command pattern).
class Command
{
public:
    virtual ~Command() = default;
    // Trả về false nếu cần ngắt kết nối client.
    virtual bool execute(CommandContext& ctx) = 0;
};