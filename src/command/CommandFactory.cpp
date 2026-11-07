#include "command/CommandFactory.h"
#include "command/Commands.h"

CommandFactory::CommandFactory()
{
    creators["/login"] = [](const std::string& args) { return std::make_unique<LoginCommand>(args); };
    creators["/quit"]  = [](const std::string&)      { return std::make_unique<QuitCommand>(); };
    creators["/help"]  = [](const std::string&)      { return std::make_unique<HelpCommand>(); };
}
std::unique_ptr<Command> CommandFactory::create(const std::string& line) const
{
    if(line.empty() || line[0] != '/')
    {
        return std::make_unique<MessageCommand>(line);
    }
    size_t space = line.find(' ');
    std::string name = line.substr(0, space);
    std::string args = (space == std::string::npos) ? "" : line.substr(space + 1);

    auto it = creators.find(name);
    if (it == creators.end())
    {
        return std::make_unique<UnknownCommand>(name);
    }
    return it->second(args);
}
