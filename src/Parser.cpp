#include "Parser.hpp"

Parser::Parser(){}

Parser::~Parser(){}

static void printElement(std::string str){
    std::cout << "argument: " << str << std::endl;
}

void Parser::lTrim(std::string& str) const{
    size_t start = str.find_first_not_of(" \t");

    if (start == std::string::npos)
        str.clear();
    else if (start > 0)
        str.erase(0, start);
}

void Parser::toUpper(std::string& str) const{
    for (size_t i = 0; i < str.size(); i++){
        char lower = str[i];
        str[i] = toupper(lower);
    }
}

IrcMessage Parser::tokenize(std::string& message) const{
    IrcMessage ircMessage;
    size_t pos = std::string::npos;

    if (message[0] == ':'){
        if ((pos = message.find(" ")) != std::string::npos ){
            ircMessage.prefix = message.substr(1, pos - 1);
            message.erase(0, pos);
            lTrim(message);
        }
        else{
            ircMessage.prefix = message;
            message.clear();
        }
    }
    if (!ircMessage.prefix.empty())
        std::cout << "prefix: " << ircMessage.prefix << std::endl;

    if ((pos = message.find(" ")) != std::string::npos ){
        ircMessage.command = message.substr(0, pos);
        message.erase(0, pos);
        lTrim(message);
    }
    else{
        ircMessage.command = message;
        message.clear();
    }
    toUpper(ircMessage.command);
    std::cout << "command: " << ircMessage.command << std::endl;

    while(!message.empty()){
        if (message[0] == ':'){
            ircMessage.params.push_back(message.substr(1));
            break;
        }

        if ((pos = message.find(" ")) != std::string::npos){
            ircMessage.params.push_back(message.substr(0, pos));
            message.erase(0, pos);
            lTrim(message);
        }
        else{
            ircMessage.params.push_back(message);
            message.clear();
        }
    }

    std::for_each(ircMessage.params.begin(), ircMessage.params.end(), printElement);

    return ircMessage;
}

void Parser::parseMessage(Client& client, std::string& message, Server& server) const{
    if (message.empty())
        return;

    lTrim(message);

    IrcMessage ircMessage = tokenize(message);
    
    /*if (!message.empty()){
        params.push_back(message);
        std::for_each(params.begin(), params.end(), printElement);
    }

    std::map<std::string, Command*>::const_iterator it = _commands.find(command);
    if (it != _commands.end())
        it->second->execute(client, params, server);
}*/
    server.executeCommand(client, ircMessage);
}
