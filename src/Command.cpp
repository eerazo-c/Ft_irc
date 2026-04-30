#include "Command.hpp"

Pass::~Pass(){}
void Pass::execute(Client& client, std::vector<std::string> params, Server &server) const{
    std::ostringstream oss;
    std::string errorMsg;

    if (params.empty()){
        oss << ":localhost 461 * PASS :Not enough parameters\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (client.getState() >= Client::PASS_ACCEPTED){
        oss << ":localhost 462 * :You may not reregister\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (params[0] != server.getPass()){
        oss << ":localhost 464 * :Password incorrect\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    client.setState(Client::PASS_ACCEPTED);
    std::cout << "Pass execute: SUCCESS" << std::endl;
}

Nick::~Nick(){}
void Nick::execute(Client& client, std::vector<std::string> params, Server &server) const{
    std::ostringstream oss;
    std::string errorMsg;

    if (client.getState() < Client::PASS_ACCEPTED)
        return;

    if (params.empty()){
        oss << ":localhost 431 * :No nickname given\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::string nick = params[0];
    if (client.getNick() == nick)
        return;

    for (size_t i = 0; i < nick.size(); i++){
        char c = nick[i];
        if (i == 0 && isalpha(c))
            continue;
        if (c == '_' || c == '[' || c == ']' || c == '\\' || c == '^' || c == '{' || c == '}')
            continue;
        if (i != 0 && (isalnum(c) || c == '-'))
            continue;
        oss << ":localhost 432 * " << nick << " :Erroneus nickname\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    const std::map<int, Client>& clients = server.getClients();
    std::map<int, Client>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it){
        if (it->second.getNick() == nick){
            oss << ":localhost 433 * " << nick << " :Nickname is already in use\r\n";
            errorMsg = oss.str();
            send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
    }
    
    std::string oldNick = client.getNick(); 
    client.setNick(nick);

    if (client.getState() == Client::REGISTERED) {
        oss << ":" << oldNick << "!" << client.getUser() << "@127.0.0.1 NICK :" << nick << "\r\n";
        std::string nickMsg = oss.str();
        send(client.getFd(), nickMsg.c_str(), nickMsg.size(), 0);

        // (Nota para el futuro: Cuando tengas canales, también tendrás que enviarle 
        // este mismo mensaje a todas las personas que estén en los mismos canales 
        // que este usuario, ¡para que vean que se cambió el nombre!)
    }

    if (!client.getUser().empty() && !client.getRealName().empty() && client.getState() != Client::REGISTERED){
        client.setState(Client::REGISTERED);
        std::string nick = client.getNick();
        std::string user = client.getUser();
        std::string host = "127.0.0.1";
        
        oss << ":localhost 001 " << nick << " :Welcome to the Internet Relay Network " 
            << nick << "!" << user << "@" << host << "\r\n";
            
        std::string welcomeMsg = oss.str();
        send(client.getFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);
    }

    std::cout << "Nick execute: SUCCESS" << std::endl;
}

User::~User(){}
void User::execute(Client& client, std::vector<std::string> params, Server &server) const{
    (void)server;
    std::ostringstream oss;
    std::string errorMsg;

    if (client.getState() < Client::PASS_ACCEPTED)
        return;

    if (params.empty() || params.size() < 4 || params[0].empty() || params[3].empty()){
        oss << ":localhost 461 * USER :Not enough parameters\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (client.getState() == Client::REGISTERED){
        oss << ":localhost 462 * :You may not reregister\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    client.setUser(params[0]);
    client.setRealName(params[3]);

    if (!client.getNick().empty()){
        client.setState(Client::REGISTERED);
        std::string nick = client.getNick();
        std::string user = client.getUser();
        std::string host = "127.0.0.1";
        
        oss << ":localhost 001 " << nick << " :Welcome to the Internet Relay Network " 
            << nick << "!" << user << "@" << host << "\r\n";
            
        std::string welcomeMsg = oss.str();
        send(client.getFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);
    }

    std::cout << "User execute: SUCCESS" << std::endl;
}

Join::~Join(){}
void Join::execute(Client& client, std::vector<std::string> params, Server &server) const{
    (void)client;
    (void)params;
    (void)server;
    std::cout << "Join execute" << std::endl;
}

Part::~Part(){}
void Part::execute(Client& client, std::vector<std::string> params, Server &server) const{
    (void)client;
    (void)params;
    (void)server;
    std::cout << "Part execute" << std::endl;
}

Quit::~Quit(){}
void Quit::execute(Client& client, std::vector<std::string> params, Server &server) const{
    (void)client;
    (void)params;
    (void)server;
    std::cout << "Quit execute" << std::endl;
}