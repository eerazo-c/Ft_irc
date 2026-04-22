#pragma once

#include "header.hpp"

class Client;
class Command;
class Server;

class Parser{
    private:
        std::map<std::string, Command*> _commands;
        Parser(const Parser &other);
        Parser &operator=(const Parser &other);

    public:
        Parser();       
        ~Parser();
        void parseMessage(Client &client, std::string message, Server &server) const;
};