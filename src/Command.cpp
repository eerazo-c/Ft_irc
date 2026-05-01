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

Pass::~Pass(){}
void Pass::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "Pass execute" << std::endl;
}

Nick::~Nick(){}
void Nick::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "Nick execute" << std::endl;
}

User::~User(){}
void User::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "User execute" << std::endl;
}

Join::~Join()
{

}

void Join::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.size() == 1 && args[0] == "0")
	{
		std::map<std::string, Channel> &channels = server.getChannels();

		for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			it->second.removeClient(client);
		}
		return;
	}

	// Parsear canales (#a,#b,#c)
	std::stringstream ss(args[0]);
	std::string chan_name;

	while (std::getline(ss, chan_name, ','))
	{
		std::map<std::string, Channel> &channels = server.getChannels();

		if (channels.find(chan_name) == channels.end())
			channels.insert(std::make_pair(chan_name, Channel(chan_name)));

		Channel &chan = channels[chan_name];

		//  Si ya está dentro, skip
		if (chan.isMember(client))
			continue;

		//  Añadir cliente
		chan.addClient(client);

		// aquí luego meter Write()
	}
}

Part::~Part()
{

}

void Part::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.empty() || args[0].empty())
		return (client.WritePrefix(ERR_NEEDMOREPARAMS(client._nick(), _name)));

	std::string message;
	if (args.size() > 1)
		messages = args[1];
	else 
		messages = "";

	std::string chan;
	std::stringstream ss(args[0]);

	while (std::getline(ss, chan, ','))
		server._channels().Part(client, chan, message);
    std::cout << "Part execute" << std::endl;
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
	
	Log() << client->_nick() << ": " << message;
	
	server._channels().CloseClient(client.socket(), "Quit: " + message);

	std::cout << "Quit execute" << std::endl;
}

PrivMsg::PrivMsg(void)
{
}

void PrivMsg::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.empty())
		return (client->WritePrefix(ERR_NORECIPIENT(client._nick(), _name)));
	else if (args.size() == 1)
		return (client->WritePrefix(ERR_NOTEXTTOSEND(client->_nick())));

	std::string message = args[1];
	std::string target;
	std::stringstream ss(args[0]);

	while (std::getline(ss, target, ','))
	{
		Client *dest = NULL;
		Channel *chan = NULL;

		if (target.find('!') != std::string::npos)
			dest = server.clients().Search(target);
		else
		{
			std::string nick = target.substr(0, target.find('!'));
			dest = server.clients().Search(nick);
		}

		if (dest)
			std::cout << dest.client.mask() << _name + << " " + << client._nick() + << " :" + << message;

		else if ((chan = server.channels().Search(target)))
			std::cout << chan.client << _name + << " " + << chan._name() + << " :" + << message;
		else
			client.WritePrefix(ERR_NOSUCHNICK(client._nick(), target));
	}
}
