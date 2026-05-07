/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:48:43 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/22 17:48:58 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Command.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include <iterator>
#include <locale>
#include <cmath>

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
    
    const std::map<int, Client *>& clients = server.getClients();
    std::map<int, Client *>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it){
        if (it->second->getNick() == nick){
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

Join::~Join()
{

}

void Join::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.empty())
		return ;
	if (client.getState() != Client::REGISTERED)
		return;

	if (args.size() == 1 && args[0] == "0")
	{
		std::map<std::string, Channel> &channels = server.getChannels();

		for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (it->second.isMember(client))
				it->second.removeClient(client);
		}
		return;
	}
	
	std::map<std::string, Channel> &channels = server.getChannels();
	
	std::stringstream ss(args[0]);
	std::string chan_name;

	while (std::getline(ss, chan_name, ','))
	{
		//std::map<std::string, Channel> &channels = server.getChannels();
		std::cout << "CANAL " << chan_name << " CLIENTES:" << std::endl;
//comienza aqui el test
		for (std::map<int, Client*>::iterator it = server.getClients().begin();
		it != server.getClients().end(); ++it)
		{
			std::cout << it->first << " nick=" << it->second->getNick() << std::endl;
		}
		//termina aqui

		if (chan_name.empty() || chan_name[0] != '#')
		{
			// opcional: enviar error IRC
			 client.WritePrefix(ERR_NOCREATEHAS(client.getNick(), chan_name));
			continue;
		}

		if (channels.find(chan_name) == channels.end())
			channels.insert(std::make_pair(chan_name, Channel(chan_name)));

		Channel &chan = channels[chan_name];

		//  Si ya está dentro, skip
		if (chan.isMember(client))
			continue;
		std::cout << "Nick: [" << client.getNick() << "]\n";
		std::cout << "User: [" << client.getUser() << "]\n";
		//  Añadir cliente
		chan.addClient(client);

		// JOIN msg
		std::string joinMsg = ":" + client.getNick() + "!" +
			client.getUser() + "@localhost JOIN " + chan_name + "\r\n";

		chan.broadcast(joinMsg);

		//  NAMES
		std::string users = chan.getUsersList();

		std::string namesMsg = ":localhost 353 " + client.getNick() +
			" = " + chan_name + " :" + users + "\r\n";

		std::string endMsg = ":localhost 366 " + client.getNick() +
			" " + chan_name + " :End of /NAMES list\r\n";

		send(client.getFd(), namesMsg.c_str(), namesMsg.size(), 0);
		send(client.getFd(), endMsg.c_str(), endMsg.size(), 0);
		// aquí luego meter Write()
		std::cout << "execute join" << std::endl;
	}
}

Part::~Part()
{

}

void Part::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.empty() || args[0].empty())
		return (client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "Part"))); //aqui modifique _name x args

	std::string message;
	if (args.size() > 1)
		message = args[1];
	else 
		message = "";

	std::string chan;
	std::stringstream ss(args[0]);

	std::map<std::string, Channel> &channels = server.getChannels();

	while (std::getline(ss, chan, ','))
	{
		std::map<std::string, Channel>::iterator it = channels.find(chan);

		if (it == channels.end())
			continue;

		Channel &channel = it->second;

		if (!channel.isMember(client))
			continue;

		std::string partMsg = ":" + client.getNick() + "!" +
			client.getUser() + "@localhost PART " + chan +
			" :" + message + "\r\n";

		channel.broadcast(partMsg);
		channel.removeClient(client);
	}
}

Quit::~Quit()
{

}

void Quit::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	std::string message;

	if (!args.empty())
		message = args[0];
	else 
		message = "";

	std::string quitMsg = ":" + client.getNick() +
		" QUIT :" + message + "\r\n";

	std::map<std::string, Channel> &channels = server.getChannels();

	for (std::map<std::string, Channel>::iterator it = channels.begin();
		 it != channels.end(); ++it)
	{
		Channel &chan = it->second;

		if (chan.isMember(client))
		{
			chan.broadcast(quitMsg);
			chan.removeClient(client);
		}
	}

	/*std::string message;

	if (!args.empty())
		message = args[0];
	else 
		message = "";
	
	std::cout << client.getNick() << ": " << message << std::endl;
	
	client.CloseClient(server);

	std::cout << "Quit execute" << std::endl;
*/
}

PrivMsg::~PrivMsg(void)
{
}

//PRIVMSG nick :msg
void PrivMsg::execute(Client& client, std::vector<std::string> args, Server &server) const
{
    if (args.empty())
	{
        client.WritePrefix(ERR_NORECIPIENT(client.getNick(), "PRIVMSG"));
		return ;
	}

    if (args.size() == 1)
	{
        client.WritePrefix(ERR_NOTEXTTOSEND(client.getNick()));
		return ;
	}

    std::string target = args[0];

    std::string message;
    for (size_t i = 1; i < args.size(); i++)
    {
        if (i > 1)
            message += " ";
        message += args[i];
    }
   	
		std::cout << "target:" << target << std::endl;
		std::cout << "message:" << message << std::endl;

		std::string fullMsg = ":" + client.getNick() + "!" + 
		client.getUser() + "@localhost PRIVMSG " + target + " " + message + "\r\n";

//	std::string fullMsg = ":" + client.getNick() + "!" + 
//		client.getUser() + "@localhost PRIVMSG " + target + " " + message + "\r\n";
	std::cout << "fullMsg: " << fullMsg << std::endl;
    //buscar cliente por nick
 //   std::map<int, Client *>& clients = server.getClients();

	std::cout << "---- BROADCAST ----" << std::endl;
    for (std::map<int, Client *>::iterator it = server.getClients().begin(); it != server.getClients().end(); ++it)
    {
		std::cout << "FD en canal: " << it->first << std::endl;
        if (it->second->getNick() == target)
        {
			send(it->second->getFd(), fullMsg.c_str(), fullMsg.size(), 0);

            // opcional pero correcto en IRC: eco al emisor
    //        send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
			return;
            //dest = it->second;
            //break;
        }
    }

   /* if (dest)
    {
		std::string msg = ":" + client.getNick() + "!" +
			client.getUser() + "@localhost PRIVMSG " +
			dest->getNick() + " :" + message + "\r\n";

		send(dest->getFd(), msg.c_str(), msg.size(), 0);
		return;
		
        std::cout << client.getNick() << " -> " << dest->getNick()
                  << " : " << message << std::endl;
        return;
    }
*/
    // buscar canal
    std::map<std::string, Channel>& channels = server.getChannels();
    std::map<std::string, Channel>::iterator it = channels.find(target);

    if (it != channels.end())
    {
		Channel& channel = it->second;

        if (!channel.isMember(client))
        {
            client.WritePrefix(ERR_CANNOTSENDTOCHAN(client.getNick(), target));
            return;
        }

        channel.broadcastExcept(client, fullMsg);
        return;
	}

    // no existe nada
    client.WritePrefix(ERR_NOSUCHNICK(client.getNick(), target));
	std::cout << "Broadcast to FD: " << client.getFd() << std::endl;
	std::cout << "execute PrivMsg" << std::endl;
}
