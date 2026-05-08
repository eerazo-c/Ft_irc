/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:05:05 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/01 17:05:21 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
//#include "Command.hpp"

class Client;
class Server;
class Command;

class Channel {
	private:
	std::string _name;
	std::map<int, Client *> _clients;

	public:
	Channel();
	Channel(std::string &name);
	~Channel();

	const std::string& getName() const;
	

	void setClientChannel(Server &server);
	void broadcast(const std::string &msg);
	void broadcastExcept(Client &sender, const std::string &msg);
	std::string getUsersList() const;

	bool isMember(Client &client);
	void addClient(Client &client);
	void removeClient(Client &client);
	void partAll(Client &client);

	std::string buildJoinMsg(Client &client, std::string chan);
	std::string buildPartMsg(Client &client, std::string chan, std::string message);
	std::string buildQuitMsg(Client &client, std::string message);
	std::string buildPrivMsg(Client &client, std::string target, std::string message);

	std::string buildNamesMsg(Client &client, std::string chan);
	std::string buildEndNamesMsg(Client &client, std::string chan);
};
