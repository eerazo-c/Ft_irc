#include "Parser.hpp"

Parser::Parser(){
    _commands["PASS"] = new Pass();
	//_commands["INVITE"] = new Invite();
	_commands["JOIN"] = new Join();
	//_commands["KICK"] = new Kick();
	//_commands["MODE"] = new Mode();
	_commands["NICK"] = new Nick();
	_commands["PART"] = new Part();
	//_commands["PRIVMSG"] = new PrivMsg();
	_commands["QUIT"] = new Quit();
	//_commands["TOPIC"] = new Topic();
	_commands["USER"] = new User();
}

Parser::~Parser(){
    std::map<std::string, Command*>::iterator it;
    for (it = _commands.begin(); it != _commands.end(); ++it){
        delete it->second;
    }
}

static void printElement(std::string str){
    std::cout << "argument: " << str << std::endl;
}

void Parser::parseMessage(Client& client, std::string message, Server& server) const{
    (void)client;
    std::string command;
    std::vector<std::string> params;
    std::string trailing_param;
    size_t pos = std::string::npos;
    size_t trailing_pos = std::string::npos;

    if ((pos = message.find(" ")) != std::string::npos )
        command = message.substr(0, pos);
    else{
        command = message;
        message.clear();
    }
    std::cout << "command: " << command << std::endl;

    if ((trailing_pos = message.find(" :")) != std::string::npos){
        trailing_param = message.substr(trailing_pos + 2);
        message.erase(trailing_pos);
        message.erase(0, pos);
        if (!trailing_param.empty()){
            std::cout << "trailing argument: " << trailing_param << std::endl;
        }
    }
    else
        message.erase(0, pos + 1);

    while((pos = message.find(' ')) != std::string::npos){
        std::string token = message.substr(0, pos);
        if (!token.empty())
            params.push_back(token);
        message.erase(0, pos + 1);
    }
    
    if (!message.empty()){
        params.push_back(message);
        std::for_each(params.begin(), params.end(), printElement);
    }

    std::map<std::string, Command*>::const_iterator it = _commands.find(command);
    if (it != _commands.end())
        it->second->execute(client, params, server);
}