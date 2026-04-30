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

Join::~Join() : command("JOIN", 1, 2, true)
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

Part::~Part() : command("Part", 1, 2, true)
{

}

void Part::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.empty() || args[0].empty())
		return (client->WritePrefix(ERR_NEEDMOREPARAMS(client->nickname(), name_)));

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

Quit::~Quit("Quit", 1, 2, true)
{

}

void Quit::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	std::string message;

	if (!args.empty())
		message = args[0];
	else 
		message = "";
	
	Log() << client->nickname() << ": " << message;
	
	server._channels().CloseClient(client.socket(), "Quit: " + message);

	std::cout << "Quit execute" << std::endl;
}
