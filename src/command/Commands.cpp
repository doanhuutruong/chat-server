#include "command/Commands.h"

#include "core/ClientManager.h"
#include "util/Logger.h"
#include "core/HistoryProcess.h"

bool LoginCommand::execute(CommandContext& ctx)
{
    if (!ctx.clients.nameOf(ctx.fd).empty())
    {
        ctx.clients.sendTo(ctx.fd, "ERR you are already logged in");
        return true;
    }
    if (name_.empty() || name_.find(' ') != std::string::npos)
    {
        ctx.clients.sendTo(ctx.fd, "ERR usage: /login <name> (no spaces)");
        return true;
    }
    if (!ctx.clients.login(ctx.fd, name_))
    {
        ctx.clients.sendTo(ctx.fd, "ERR name '" + name_ + "' is taken");
        return true;
    }
    Logger::Instance().log("User logged in: " + name_);
    ctx.clients.sendTo(ctx.fd, "OK welcome " + name_);
    ctx.clients.broadcast("* " + name_ + " joined the chat", ctx.fd);
    return true;
}

bool MessageCommand::execute(CommandContext& ctx)
{
    std::string name = ctx.clients.nameOf(ctx.fd);
    if (name.empty())
    {
        ctx.clients.sendTo(ctx.fd, "ERR please /login <name> first");
        return true;
    }
    if (text_.empty()) return true;
    std::string line = name + ": " + text_;
    ctx.clients.broadcast(line, ctx.fd);
    ctx.history.write(line);
    return true;
}

bool QuitCommand::execute(CommandContext& ctx)
{
    ctx.clients.sendTo(ctx.fd, "Bye!");
    return false;   // báo cho server ngắt kết nối
}

bool HelpCommand::execute(CommandContext& ctx)
{
    ctx.clients.sendTo(ctx.fd,
        "Commands:\n"
        "  /login <name>   log in\n"
        "  <text>          send a message to everyone\n"
        "  /quit           disconnect");
    return true;
}

bool UnknownCommand::execute(CommandContext& ctx)
{
    ctx.clients.sendTo(ctx.fd, "ERR unknown command " + name_ + ". Type /help");
    return true;
}