#pragma once

#include "header.hpp"

class Client;
class Command;
class Server;

struct IrcMessage{
        std::string prefix;
        std::string command;
        std::vector<std::string> params;
};

class Parser{
    public:
        Parser();
        ~Parser();

		void lTrim(std::string& str) const;
        void toUpper(std::string& str) const;
        IrcMessage tokenize(std::string& message) const;
        void parseMessage(Client &client, std::string& message, Server &server) const;
};
