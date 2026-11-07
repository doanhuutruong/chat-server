#pragma once
#include <functional>
#include <map>
#include <string>
#include "memory"
#include "command/Command.h"

class CommandFactory
{
public:
    CommandFactory();
    std::unique_ptr<Command> create(const std::string& line) const;

private:
    using Creator = std::function<std::unique_ptr<Command>(const std::string& args)>;
    std::map<std::string, Creator> creators;
};