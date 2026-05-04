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
	int fd = client.getFd();

	if (isMember(client))
		return;

    _clients.insert(std::make_pair(fd, &client));
}

void Channel::removeClient(Client &client)
{
	int fd = client.getFd();

	std::map<int, Client*>::iterator it = _clients.find(fd);

	if (it != _clients.end())
		_clients.erase(it);
}
