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
/*	int fd = client.getFd();

	if (isMember(client))
		return;

    _clients.insert(std::make_pair(fd, &client));*/
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
	std::cout << "BROADCAST START" << std::endl;
	
	for (std::map<int, Client*>::iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		Client* client = it->second;

		std::cout << "sending to: " << client->getNick()
					<< " fd=" << client->getFd() << std::endl;

		if (client->getFd() != sender.getFd())
		{
			std::cout << "SENT ✔" << std::endl;
			send(client->getFd(), msg.c_str(), msg.size(), 0);
		}

/*		if (client != &sender)
			send(client->getFd(), msg.c_str(), msg.size(), 0);*/

/*		if (it->first != sender.getFd())
			send(it->first, msg.c_str(), msg.size(), 0);*/
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
/*
void Channel::setClientChannel(Server &server)
{
	this->_clients = server->getClients();
}
*/
