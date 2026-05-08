/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 19:36:25 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/30 19:37:08 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include <string>

Channel::Channel()
{

}

Channel::Channel(std::string &name) : _name(name)
{

}

Channel::~Channel(){

}

bool Channel::isMember(Client &client)
{
	int fd = client.getFd();

	if (_clients.find(fd) != _clients.end())
		return true;
	else 
		return false;
}

void Channel::addClient(Client &client)
{
	_clients[client.getFd()] = &client;
}

void Channel::removeClient(Client &client)
{
	int fd = client.getFd();

	std::map<int, Client*>::iterator it = _clients.find(fd);

	if (it != _clients.end())
		_clients.erase(it);
}

void Channel::broadcast(const std::string &msg){
	for (std::map<int, Client*>::iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		send(it->first, msg.c_str(), msg.size(), 0);
	}
}

void Channel::broadcastExcept(Client &sender, const std::string &msg)
{	
	for (std::map<int, Client*>::iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		Client* client = it->second;

		if (client->getFd() != sender.getFd())
		{
			send(client->getFd(), msg.c_str(), msg.size(), 0);
		}
	}
}

std::string Channel::getUsersList() const{
	std::string list;

	for (std::map<int, Client*>::const_iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		if (!list.empty())
			list += " ";
		list += it->second->getNick();
	}
	return list;
}

std::string Channel::buildJoinMsg(Client &client, std::string chan)
{
	std::string joinMsg = ":" + client.getNick() + "!" + 
		client.getUser() + "@localhost JOIN " + chan + "\r\n";
	return (joinMsg);
}

std::string Channel::buildPartMsg(Client &client, std::string chan, std::string message)
{
	std::string partMsg = ":" + client.getNick() + "!" +
		client.getUser() + "@localhost PART " + chan + " :" + message + "\r\n";
	return (partMsg);
}

std::string Channel::buildQuitMsg(Client &client, std::string message)
{
	std::string quitMsg = ":" + client.getNick() + " QUIT :" + message + "\r\n";
	return (quitMsg);
}

std::string Channel::buildPrivMsg(Client &client, std::string target, std::string message)
{
	std::string fullMsg = ":" + client.getNick() + "!" +
		client.getUser() + "@localhost PRIVMSG " + target + " " + message + "\r\n";
	return (fullMsg);
}

std::string Channel::buildNamesMsg(Client &client, std::string chan)
{
	std::string users = getUsersList();
	std::string namesMsg = ":localhost 353 " + client.getNick() + " = " + 
		chan + " :" + users + "\r\n";
	return (namesMsg);
}

std::string Channel::buildEndNamesMsg(Client &client, std::string chan)
{
	std::string endMsg = ":localhost 366 " + client.getNick() + " " +
		chan + " :End of /NAMES list\r\n";
	return (endMsg);
}
