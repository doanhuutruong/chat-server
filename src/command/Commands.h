#pragma once
#include <string>
#include "command/Command.h"

class LoginCommand : public Command
{
public:
    explicit LoginCommand(std::string name) : name_(std::move(name)) {}
    bool execute(CommandContext& ctx) override;
private:
    std::string name_;
};

class MessageCommand : public Command
{
public:
    explicit MessageCommand(std::string text) : text_(std::move(text)) {}
    bool execute(CommandContext& ctx) override;
private:
    std::string text_;
};

class QuitCommand : public Command
{
public:
    bool execute(CommandContext& ctx) override;
};

class HelpCommand : public Command
{
public:
    bool execute(CommandContext& ctx) override;
};

class UnknownCommand : public Command
{
public:
    explicit UnknownCommand(std::string name) : name_(std::move(name)) {}
    bool execute(CommandContext& ctx) override;
private:
    std::string name_;
};