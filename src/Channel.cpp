/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 19:29:30 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/13 19:30:12 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"

Channel::Channel(std::string &name) : (name)
{

}

Channel::~Channel(){

}

bool Channel::isMember(Client &client)
{
	int fd = client.fd;

	if (_clients.find(fd) != _clients.end())
		return true;
	else if 
		return false;
}

void Channel::addClient(Client &client)
{
	int fd = clients.getFD();

	if (isMember(client))
		return;

    _clients.insert(std::make_pair(fd, &client));
}

void Channel::removeClient(Client &client)
{

}

void Channel::partAll(Client &client)
{
	std::map<std::string, Channel>::iterator it;
	for (it = _channel.begin(); it != _channel.end(); ++it)
		Part(client, it->first, "");
}
