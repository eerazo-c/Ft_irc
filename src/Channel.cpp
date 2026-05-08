/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhea <arhea@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 19:36:25 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/08 15:36:55 by arhea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"


Channel::Channel()
	: _name(""), _clients(), _operators(), _invited(), _topic(""),
	  _modeI(false), _modeT(false), _key(""), _limit(0)
{
}

Channel::Channel(std::string &name)
	: _name(name), _clients(), _operators(), _invited(), _topic(""),
	  _modeI(false), _modeT(false), _key(""), _limit(0)
{
}

Channel::~Channel(){

}

bool Channel::isMember(Client &client) const
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
	bool firstClient = _clients.empty();

	if (isMember(client))
		return;

	_clients.insert(std::make_pair(fd, &client));
	if (firstClient)
		setOperator(client, true);
	removeInvited(client);
}


void Channel::removeClient(Client &client)
{
	int fd = client.getFd();

	std::map<int, Client*>::iterator it = _clients.find(fd);

	if (it != _clients.end())
	{
		_clients.erase(it);
		_operators.erase(fd);
		_invited.erase(fd);
	}
}


void Channel::broadcast(const std::string &msg){
	for (std::map<int, Client*>::iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		send(it->first, msg.c_str(), msg.size(), 0);
	}
}

void Channel::broadcastExcept(Client &sender, const std::string &msg){
	for (std::map<int, Client*>::iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		Client* client = it->second;

		if (client != &sender)
			send(client->getFd(), msg.c_str(), msg.size(), 0);

		/*if (it->first != sender.getFd())
			send(it->first, msg.c_str(), msg.size(), 0);*/
	}
}

std::string Channel::getUsersList() const
{
	std::string list;

	for (std::map<int, Client*>::const_iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		if (!list.empty())
			list += " ";
		if (_operators.find(it->first) != _operators.end())
			list += "@";
		list += it->second->getNick();
	}
	return list;
}

int Channel::getClientCount() const
{
	return (static_cast<int>(_clients.size()));
}

bool Channel::isOperator(Client &client) const
{
	int fd = client.getFd();

	return (_operators.find(fd) != _operators.end());
}

void Channel::setOperator(Client &client, bool status)
{
	int fd = client.getFd();

	if (!isMember(client))
		return;
	if (status)
		_operators[fd] = &client;
	else
		_operators.erase(fd);
}

bool Channel::isInvited(Client &client) const
{
	int fd = client.getFd();

	return (_invited.find(fd) != _invited.end());
}

void Channel::addInvited(Client &client)
{
	int fd = client.getFd();

	_invited[fd] = &client;
}

void Channel::removeInvited(Client &client)
{
	int fd = client.getFd();

	_invited.erase(fd);
}

const std::string& Channel::getTopic() const
{
	return (_topic);
}

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

bool Channel::getMode(char mode) const
{
	if (mode == 'i')
		return (_modeI);
	if (mode == 't')
		return (_modeT);
	if (mode == 'k')
		return (!_key.empty());
	if (mode == 'l')
		return (_limit > 0);
	return (false);
}

void Channel::setMode(char mode, bool status)
{
	if (mode == 'i')
		_modeI = status;
	else if ( mode== 't')
		_modeT = status;
	else if ( mode == 'k' && status == false)
		_key.clear();
	else if (mode == 'l' && status == false)
		_limit = 0;
}

const std::string& Channel::getKey() const
{
	return (_key);
}

void Channel::setKey(const std::string &key)
{
	_key = key;
}

int Channel::getLimit() const
{
	return (_limit);
}

void Channel::setLimit(int limit)
{
	if (limit < 0)
		_limit = 0;
	else
		_limit = limit;
}
